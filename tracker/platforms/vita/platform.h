#pragma once
#include <cstddef>
#include <cstdint>
// UI-thread lifetime, before settings and after the Lucky worker is joined.
bool vitaPlatformInit();
// After gfxSetup, before project/audio setup. Shows first-run Unpacking text.
bool vitaPlatformPrepareAssets(int foreground, int background);
void vitaPlatformQuit();
void vitaPlatformPoll();

void vitaInputInit();
void vitaInputReset();
std::size_t vitaHeapAvailable();
void vitaAudioRecord(uint64_t started, unsigned frames, unsigned rate);
