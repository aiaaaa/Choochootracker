#include "platform.h"
#include <SDL2/SDL.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/power.h>
#include <psp2/sysmodule.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
static unsigned char netMemory[512 * 1024];
static bool netStarted, netctlStarted;
#endif
// Leave GPU/OS headroom; no clock override. Peak use still needs hardware measurement.
extern "C" {
  unsigned int _newlib_heap_size_user = 256 * 1024 * 1024;
  unsigned int sceUserMainThreadStackSize = 2 * 1024 * 1024;
  unsigned int _pthread_stack_default_user = 2 * 1024 * 1024;
}
static SceUID powerCallback = -1;
static bool powerRegistered;
static int onPower(int, int, int flags, void*) {
  SDL_Event event{};
  if (flags & (SCE_POWER_CB_APP_SUSPEND | SCE_POWER_CB_SYSTEM_SUSPEND)) event.type = SDL_APP_WILLENTERBACKGROUND;
  else if (flags & (SCE_POWER_CB_APP_RESUME | SCE_POWER_CB_SYSTEM_RESUME)) event.type = SDL_APP_DIDENTERFOREGROUND;
  if (event.type) SDL_PushEvent(&event);
  return 0;
}
bool vitaPlatformInit() {
  namespace fs = std::filesystem;
  try {
    fs::create_directories("ux0:data/choochootracker");
    // Copy seeds only when absent. Never replace user settings, fonts or samples.
    for (const auto& entry : fs::recursive_directory_iterator("app0:/assets")) {
      auto dest = fs::path("ux0:data/choochootracker") / entry.path().lexically_relative("app0:/assets");
      if (entry.is_directory()) fs::create_directories(dest);
      else if (entry.is_regular_file()) fs::copy_file(entry.path(), dest, fs::copy_options::skip_existing);
    }
  } catch (const std::exception& e) { fprintf(stderr, "Vita asset setup: %s\n", e.what()); return false; }
  if (chdir("ux0:data/choochootracker") != 0) return false;
  freopen("vita.log", "w", stderr);
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
void vitaPlatformPoll() { sceKernelCheckCallback(); }
void vitaPlatformQuit() {
  if (powerRegistered) scePowerUnregisterCallback(powerCallback);
  if (powerCallback >= 0) sceKernelDeleteCallback(powerCallback);
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (netctlStarted) sceNetCtlTerm();
  if (netStarted) sceNetTerm();
#endif
}
