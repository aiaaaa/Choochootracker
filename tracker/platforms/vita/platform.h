#pragma once
#include <cstddef>
#include <cstdint>
// UI-thread lifetime, before settings and after the Lucky worker is joined.
bool vitaPlatformInit();
void vitaPlatformQuit();
void vitaPlatformPoll();

void vitaInputInit();
void vitaInputReset();
std::size_t vitaHeapAvailable();
void vitaAudioRecord(uint64_t started, unsigned frames, unsigned rate);
