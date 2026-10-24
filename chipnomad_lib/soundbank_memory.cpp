#include "soundbank_memory.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/heap.h>
#elif defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#include <mach/mach.h>
#endif
namespace soundbank {
MemorySnapshot systemMemory() {
  MemorySnapshot m;
#if defined(__EMSCRIPTEN__)
  // Browser RAM reporting is coarse or unavailable. Reserve half of the Wasm
  // address space for files, ZIP members, decoder scratch and the rest of the app.
  const uint64_t maximum = emscripten_get_heap_max();
  m.total = maximum;
  m.available = std::min<uint64_t>(m.total, maximum - emscripten_get_heap_size());
#elif defined(_WIN32)
  MEMORYSTATUSEX status{};
  status.dwLength = sizeof(status);
  if (GlobalMemoryStatusEx(&status)) { m.total = status.ullTotalPhys; m.available = status.ullAvailPhys; }
#elif defined(__APPLE__)
  size_t length = sizeof(m.total);
  if (sysctlbyname("hw.memsize", &m.total, &length, nullptr, 0)) m.total = 0;
  vm_statistics64_data_t stats{};
  mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
  const mach_port_t host = mach_host_self();
  vm_size_t page = 0;
  if (host_page_size(host, &page) == KERN_SUCCESS &&
      host_statistics64(host, HOST_VM_INFO64, reinterpret_cast<host_info64_t>(&stats), &count) == KERN_SUCCESS)
    m.available = uint64_t(page) * (uint64_t(stats.free_count) + stats.inactive_count);
  mach_port_deallocate(mach_task_self(), host);
#elif defined(__linux__)
  std::ifstream in("/proc/meminfo");
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream row(line); std::string key; uint64_t value;
    if (!(row >> key >> value)) continue;
    if (key == "MemTotal:") m.total = value * 1024;
    else if (key == "MemAvailable:") m.available = value * 1024;
  }
  // Respect a container's cgroup v2 limit where present.
  uint64_t limit = 0, used = 0;
  std::ifstream cap("/sys/fs/cgroup/memory.max"), usage("/sys/fs/cgroup/memory.current");
  if (cap >> limit && usage >> used && limit < m.total) {
    m.total = limit;
    m.available = std::min(m.available, limit > used ? limit - used : 0);
  }
#endif
  return m;
}
size_t automaticCacheBudget(MemorySnapshot m, size_t loaded) {
  constexpr uint64_t mib = 1024 * 1024;
  if (!m.total) return 64 * mib;
  // At most a quarter of physical RAM and half of currently available RAM
  // (including the PCM already owned here). Re-evaluate before each load.
  const uint64_t available = std::min(m.available, m.total);
  const uint64_t reclaimable = available + std::min<uint64_t>(loaded, m.total - available);
  uint64_t budget = std::min({m.total / 4, reclaimable / 2, uint64_t(2) * 1024 * mib});
  budget = budget / mib * mib;
  return size_t(std::min<uint64_t>(budget, std::numeric_limits<size_t>::max()));
}
} // namespace soundbank
