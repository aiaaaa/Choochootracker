// Developer-only device audio callback check; never linked into the application.
#include <SDL2/SDL.h>
#include "chipnomad_lib.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <vector>

struct AudioCheck {
  ChipNomadState* state = nullptr;
  double times[16000]{};
  unsigned callbacks = 0;
  unsigned bad = 0;
  double energy = 0;
  float peak = 0;
};

static void callback(void* opaque, Uint8* bytes, int length) {
  auto& check = *static_cast<AudioCheck*>(opaque);
  auto* audio = reinterpret_cast<float*>(bytes);
  const auto start = std::chrono::steady_clock::now();
  chipnomadRender(check.state, audio, length / (2 * sizeof(float)));
  const double elapsed = std::chrono::duration<double, std::micro>(
      std::chrono::steady_clock::now() - start).count();
  if (check.callbacks < 16000) check.times[check.callbacks] = elapsed;
  ++check.callbacks;
  for (unsigned i = 0; i < length / sizeof(float); ++i) {
    if (!std::isfinite(audio[i])) ++check.bad;
    else {
      check.energy += double(audio[i]) * audio[i];
      check.peak = std::max(check.peak, std::abs(audio[i]));
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2 || SDL_Init(SDL_INIT_AUDIO) != 0) {
    std::fprintf(stderr, "Audio setup: %s\n", SDL_GetError());
    return 1;
  }
  AudioCheck check;
  check.state = chipnomadCreate();
  if (!check.state || projectLoad(&check.state->project, argv[1])) return 2;
  SDL_AudioSpec desired{}, obtained{};
  desired.freq = 48000;
  desired.format = AUDIO_F32SYS;
  desired.channels = 2;
  desired.samples = 512;
  desired.callback = callback;
  desired.userdata = &check;
  const auto device = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
  if (!device) {
    std::fprintf(stderr, "Open audio: %s\n", SDL_GetError());
    chipnomadDestroy(check.state);
    SDL_Quit();
    return 3;
  }
  chipnomadInitChips(check.state, obtained.freq, nullptr);
  chipnomadReserveRenderBuffers(check.state, obtained.samples);
  chipnomadQueueProjectRefresh(check.state);
  chipnomadQueuePlaybackStartSong(check.state, 0, 0, 1);
  SDL_PauseAudioDevice(device, 0);
  SDL_Delay(70000);
  SDL_PauseAudioDevice(device, 1);
  SDL_CloseAudioDevice(device);
  const unsigned count = std::min(check.callbacks, 16000u);
  std::vector<double> times;
  if (count > 100) times.assign(check.times + 100, check.times + count);
  std::sort(times.begin(), times.end());
  const double deadline = obtained.samples * 1e6 / obtained.freq;
  unsigned misses = 0;
  for (double time : times) misses += time > deadline;
  if (!times.empty()) std::printf(
      "driver=%s rate=%d frames=%d callbacks=%u p95_us=%.3f p99_us=%.3f "
      "worst_us=%.3f render_deadline_misses=%u energy=%.6f peak=%.6f nonfinite=%u\n",
      SDL_GetCurrentAudioDriver(), obtained.freq, obtained.samples, check.callbacks,
      times[size_t(times.size() * .95)], times[size_t(times.size() * .99)],
      times.back(), misses, check.energy, check.peak, check.bad);
  chipnomadDestroy(check.state);
  SDL_Quit();
  return times.empty() || check.bad || check.energy < .001 ? 4 : 0;
}
