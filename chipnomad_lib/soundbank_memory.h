#pragma once
#include <cstddef>
#include <cstdint>
namespace soundbank {
struct MemorySnapshot { uint64_t total = 0, available = 0; };
MemorySnapshot systemMemory();
size_t automaticCacheBudget(MemorySnapshot, size_t loaded = 0);
} // namespace soundbank
