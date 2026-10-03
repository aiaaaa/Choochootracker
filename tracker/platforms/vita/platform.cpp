#include "platform.h"
#include "seed_assets.h"
#include <SDL2/SDL.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/power.h>
#include <psp2/sysmodule.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <filesystem>
#include <atomic>
#include <malloc.h>
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
static unsigned char netMemory[512 * 1024];
static bool netStarted, netctlStarted;
#endif
// Newlib reserves ONE fixed block before main. A 256 MiB request failed on
// hardware with the ordinary application memory budget, leaving malloc unusable.
// Keep space outside this block for SDL/GXM, threads and system libraries.
static constexpr unsigned heapBytes = 192 * 1024 * 1024;
static constexpr unsigned heapReserveBytes = 8 * 1024 * 1024;
extern "C" {
  unsigned int _newlib_heap_size_user = heapBytes;
  unsigned int sceUserMainThreadStackSize = 2 * 1024 * 1024;
  unsigned int _pthread_stack_default_user = 2 * 1024 * 1024;
}
static SceUID powerCallback = -1;
static bool powerRegistered;
static std::atomic<uint32_t> audioCallbacks, audioOverBudget, audioMaxUs;
std::size_t vitaHeapAvailable() {
  const auto used=mallinfo().uordblks;
  // Also reserve room for allocator overhead and ordinary project/UI activity.
  constexpr std::size_t budget=heapBytes - heapReserveBytes;
  return used>=0 && std::size_t(used)<budget ? budget-used : 0;
}
void vitaAudioRecord(uint64_t started, unsigned frames, unsigned rate) {
  const auto elapsed=SDL_GetPerformanceCounter()-started;
  const auto frequency=SDL_GetPerformanceFrequency();
  const uint32_t us=uint32_t(elapsed*1000000/frequency);
  auto previous=audioMaxUs.load(std::memory_order_relaxed);
  while(previous<us && !audioMaxUs.compare_exchange_weak(previous,us,std::memory_order_relaxed)) {}
  audioCallbacks.fetch_add(1,std::memory_order_relaxed);
  if(elapsed*rate>uint64_t(frames)*frequency) audioOverBudget.fetch_add(1,std::memory_order_relaxed);
}
static int onPower(int, int, int flags, void*) {
  SDL_Event event{};
  if (flags & (SCE_POWER_CB_APP_SUSPEND | SCE_POWER_CB_SYSTEM_SUSPEND)) event.type = SDL_APP_WILLENTERBACKGROUND;
  else if (flags & (SCE_POWER_CB_APP_RESUME | SCE_POWER_CB_SYSTEM_RESUME)) event.type = SDL_APP_DIDENTERFOREGROUND;
  if (event.type) SDL_PushEvent(&event);
  return 0;
}
// Native I/O and stack-only formatting: leave evidence even if newlib's heap
// could not be reserved. stdio/filesystem/exception reporting need that heap.
static void startupValue(SceUID fd, const char* label, uint32_t value) {
  if (fd < 0) return;
  unsigned length = 0;
  while (label[length]) ++length;
  sceIoWrite(fd, label, length);
  char hex[11] = {'0', 'x'};
  for (unsigned i = 0; i < 8; ++i)
    hex[2 + i] = "0123456789abcdef"[(value >> (28 - i * 4)) & 15];
  hex[10] = '\n';
  sceIoWrite(fd, hex, sizeof(hex));
}
static bool checkStartupHeap() {
  sceIoMkdir("ux0:data", 0777);
  const int mkdirResult = sceIoMkdir("ux0:data/choochootracker", 0777);
  const SceUID fd = sceIoOpen("ux0:data/choochootracker/startup-memory.log",
                            SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0666);
  SceKernelFreeMemorySizeInfo freeMemory{};
  freeMemory.size = sizeof(freeMemory);
  const int memoryResult = sceKernelGetFreeMemorySize(&freeMemory);
  void* const currentBreak = sbrk(0);
  void* const probe = malloc(32);
  const bool usable = currentBreak != reinterpret_cast<void*>(-1) && probe;
  startupValue(fd, "mkdir_result=", uint32_t(mkdirResult));
  startupValue(fd, "heap_requested_bytes=", heapBytes);
  startupValue(fd, "heap_break=", uint32_t(reinterpret_cast<uintptr_t>(currentBreak)));
  startupValue(fd, "malloc_32_ok=", probe != nullptr);
  startupValue(fd, "memory_query_result=", uint32_t(memoryResult));
  startupValue(fd, "system_free_bytes=", uint32_t(freeMemory.size_user));
  startupValue(fd, "cdram_free_bytes=", uint32_t(freeMemory.size_cdram));
  if (fd >= 0) sceIoClose(fd);
  std::free(probe);
  return usable;
}
bool vitaPlatformInit() {
  if (!checkStartupHeap()) return false;
  // Open diagnostics before asset setup, so filesystem failures are visible.
  freopen("ux0:data/choochootracker/vita.log", "w", stderr);
  setvbuf(stderr, nullptr, _IONBF, 0);
  fprintf(stderr, "Vita startup: heap=%u reserve=%u; copying missing assets\n",
          heapBytes, heapReserveBytes);
  namespace fs = std::filesystem;
  try {
    fs::create_directories("ux0:data/choochootracker");
    // Copy seeds only when absent. Never replace user settings, fonts or samples.
    for (const auto& entry : fs::recursive_directory_iterator("app0:/assets")) {
      auto dest = fs::path("ux0:data/choochootracker") / entry.path().lexically_relative("app0:/assets");
      if (entry.is_directory()) fs::create_directories(dest);
      else if (entry.is_regular_file()) {
        const int error = vitaCopySeedIfMissing(entry.path().c_str(), dest.c_str());
        if (error) throw fs::filesystem_error("Vita seed copy", entry.path(), dest,
                                             std::error_code(error, std::generic_category()));
      }
    }
  } catch (const std::exception& e) { fprintf(stderr, "Vita asset setup: %s\n", e.what()); return false; }
  if (chdir("ux0:data/choochootracker") != 0) {
    fprintf(stderr, "Vita data directory: errno=%d\n", errno);
    return false;
  }
  fprintf(stderr, "Vita assets ready\n");
  SDL_SetHint(SDL_HINT_THREAD_STACK_SIZE, "1048576");
  SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (sceSysmoduleLoadModule(SCE_SYSMODULE_NET) >= 0) {
    SceNetInitParam param{netMemory, sizeof(netMemory), 0};
    netStarted = sceNetInit(&param) >= 0;
    if (netStarted) netctlStarted = sceNetCtlInit() >= 0;
  }
  fprintf(stderr, "Vita network init: net=%d netctl=%d (connectivity untested)\n", netStarted, netctlStarted);
#endif
  powerCallback = sceKernelCreateCallback("CCTPower", 0, onPower, nullptr);
  if (powerCallback >= 0) powerRegistered = scePowerRegisterCallback(powerCallback) >= 0;
  return true;
}
void vitaPlatformPoll() {
  sceKernelCheckCallback();
  static uint32_t previous=0;
  const auto now=SDL_GetTicks();
  if(now-previous<5000) return;
  previous=now;
  SceKernelFreeMemorySizeInfo free{}; free.size=sizeof(free);
  sceKernelGetFreeMemorySize(&free);
  fprintf(stderr,"Vita stats: callbacks=%u render_max_us=%u over_budget=%u heap_room=%u system_free=%d cdram_free=%d\n",
    audioCallbacks.load(),audioMaxUs.exchange(0),audioOverBudget.load(),unsigned(vitaHeapAvailable()),free.size_user,free.size_cdram);
  fflush(stderr); // UI thread only; never in the audio callback.
}
void vitaPlatformQuit() {
  if (powerRegistered) scePowerUnregisterCallback(powerCallback);
  if (powerCallback >= 0) sceKernelDeleteCallback(powerCallback);
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (netctlStarted) sceNetCtlTerm();
  if (netStarted) sceNetTerm();
#endif
}
