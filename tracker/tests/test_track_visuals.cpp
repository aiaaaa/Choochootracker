#include "doctest.h"
#include "common.h"
#include "project_utils.h"
#include "app_ui_mock.h"
#include "corelib_file.h"
#include "corelib_gfx.h"
#include "waveform_display.h"
#include "monitor_display.h"
#include "audio_monitor.h"
#include "chips/chips.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
struct VisualFixture {
  AppSettings saved = appSettings;
  ChipNomadState* state = chipnomadState;
  const AppScreen* screen = currentScreen;
  std::string settingsPath, contents;
  bool existed;
  VisualFixture() {
    char path[PATH_LENGTH];
    REQUIRE(fileGetDefaultDirectory(path, sizeof(path)) == 0);
    settingsPath = std::string(path) + "/settings.txt";
    existed = std::filesystem::exists(settingsPath);
    if (existed) { std::ifstream f(settingsPath); contents.assign(std::istreambuf_iterator<char>(f), {}); }
    initDefaultAppSettings();
    chipnomadState = chipnomadCreate();
    REQUIRE(chipnomadState != nullptr);
    projectInitAY(&chipnomadState->project);
    chipnomadInitChips(chipnomadState, 48000, nullptr);
    waveformDisplayInit();
    monitorDisplayInit();
  }
  ~VisualFixture() {
    chipnomadDestroy(chipnomadState);
    chipnomadState = state; appSettings = saved; currentScreen = screen;
    if (existed) { std::ofstream f(settingsPath); f << contents; }
    else std::filesystem::remove(settingsPath);
  }
};
bool blank(Bitmap* bitmap) {
  return bitmap && std::all_of(bitmap->data, bitmap->data + bitmap->widthPixels * bitmap->heightPixels,
    [](uint8_t value) { return value == 0; });
}
}

TEST_CASE_FIXTURE(VisualFixture, "Track visual settings preserve legacy defaults and round-trip per track") {
  { std::ofstream f(settingsPath); f << "themeName: OldTheme\n"; }
  REQUIRE(settingsLoad() == 0);
  for (const auto& visual : appSettings.trackVisuals) {
    CHECK(visual.mode == TrackVisualMode::detailed);
    CHECK(visual.wave == 1); CHECK(visual.envelope == 1); CHECK(visual.noise == 1);
  }
  appSettings.trackVisuals[0] = {TrackVisualMode::audio, 1, 0, 1};
  appSettings.trackVisuals[7] = {TrackVisualMode::detailed, 0, 1, 0};
  REQUIRE(settingsSave() == 0);
  REQUIRE(settingsLoad() == 0);
  CHECK(appSettings.trackVisuals[0].mode == TrackVisualMode::audio);
  CHECK(appSettings.trackVisuals[0].envelope == 0);
  CHECK(appSettings.trackVisuals[7].wave == 0);
  CHECK(appSettings.trackVisuals[7].noise == 0);
  CHECK(appSettings.trackVisuals[1].mode == TrackVisualMode::detailed);
  CHECK(std::string(appSettings.themeName) == "OldTheme");
  { std::ofstream f(settingsPath); f << "trackVisuals0: 1,0,0,0\ntrackVisuals9: 1,0,0,0\n"
    "trackVisuals1: 2,1,1,1\ntrackVisuals2: 1,-1,0,1\ntrackVisuals3: 1,1\n"; }
  REQUIRE(settingsLoad() == 0);
  for (const auto& visual : appSettings.trackVisuals) {
    CHECK(visual.mode == TrackVisualMode::detailed);
    CHECK(visual.wave == 1); CHECK(visual.envelope == 1); CHECK(visual.noise == 1);
  }
}

TEST_CASE_FIXTURE(VisualFixture, "Track visual table keeps detailed choices when switching modes") {
  currentScreen = &screenTrackVisuals;
  screenTrackVisuals.setup(0);
  screenTrackVisuals.fullRedraw();
  ScreenData* table = mockScreenData;
  REQUIRE(table != nullptr);
  REQUIRE(table->onEdit(2, 2, CellEditAction::tap) == 1);
  CHECK(appSettings.trackVisuals[2].envelope == 0);
  CHECK(appSettings.trackVisuals[1].envelope == 1);
  table->onEdit(0, 2, CellEditAction::tap);
  CHECK(appSettings.trackVisuals[2].mode == TrackVisualMode::audio);
  CHECK(table->onEdit(2, 2, CellEditAction::tap) == 0);
  CHECK(appSettings.trackVisuals[2].envelope == 0);
  table->drawField(2, 2, CellState::focus);
  CHECK(std::string(mockGfxCells[5] + 19, 3) == "---");
  table->onEdit(0, 2, CellEditAction::tap);
  CHECK(appSettings.trackVisuals[2].envelope == 0);
  table->onEdit(1, PROJECT_MAX_TRACKS, CellEditAction::tap);
  for (const auto& visual : appSettings.trackVisuals) {
    CHECK(visual.mode == TrackVisualMode::audio); CHECK(visual.wave == 1);
  }
  table->onEdit(0, PROJECT_MAX_TRACKS, CellEditAction::tap);
  CHECK(appSettings.trackVisuals[2].mode == TrackVisualMode::detailed);
  CHECK(appSettings.trackVisuals[2].envelope == 0);
  table->onEdit(0, PROJECT_MAX_TRACKS + 1, CellEditAction::tap);
  CHECK(currentScreen == &screenSettings);
  REQUIRE(settingsLoad() == 0);
  CHECK(appSettings.trackVisuals[2].envelope == 0);
}

TEST_CASE_FIXTURE(VisualFixture, "Detailed voice waveform and envelope are independently visible") {
  chipnomadState->project.instruments[0].type = InstrumentType::Braids;
  chipnomadState->uiPlaybackStatus.tracks[0].note.instrument = 0;
  auto& voice = chipnomadState->voiceMonitors[0];
  voice.active = 1; voice.envelope = 1;
  std::fill_n(voice.samples, VOICE_MONITOR_SAMPLES, 0.25f);
  Bitmap* bitmap = waveformDisplayGetBitmap(0);
  REQUIRE(bitmap != nullptr);
  CHECK(bitmap->data[1] == 160); // Full envelope produces the top overlay.
  appSettings.trackVisuals[0].envelope = 0;
  CHECK(waveformDisplayGetBitmap(0)->data[1] == 0);
  CHECK_FALSE(blank(bitmap));
  appSettings.trackVisuals[0].wave = 0;
  CHECK(blank(waveformDisplayGetBitmap(0)));
  appSettings.trackVisuals[0].envelope = 1;
  CHECK(waveformDisplayGetBitmap(0)->data[1] == 160);
  voice.active = 0;
  CHECK(blank(waveformDisplayGetBitmap(0)));
}

TEST_CASE_FIXTURE(VisualFixture, "AY detail reads its own track and independently toggles noise and envelope") {
  chipnomadState->project.instruments[0].type = InstrumentType::AY1;
  auto& track = chipnomadState->uiPlaybackStatus.tracks[5];
  track.note.instrument = 0; track.note.pitchFinal = 48;
  SoundChip* chip = chipnomadState->chips[5];
  REQUIRE(chip != nullptr);
  chip->setRegister(7, 0x3f); chip->setRegister(8, 15);
  Bitmap* bitmap = waveformDisplayGetBitmap(5);
  REQUIRE(bitmap != nullptr);
  CHECK(bitmap->data[0] == 255); // Track 6's own channel A, not chip 2/channel C.
  auto& visual = appSettings.trackVisuals[5];
  visual.wave = 0; visual.envelope = 0;
  chip->setRegister(7, 0x37); // Noise only.
  CHECK_FALSE(blank(waveformDisplayGetBitmap(5)));
  visual.noise = 0;
  CHECK(blank(waveformDisplayGetBitmap(5)));
  chip->setRegister(7, 0x3f); chip->setRegister(8, 0x10); chip->setRegister(13, 0);
  visual.envelope = 1;
  CHECK_FALSE(blank(waveformDisplayGetBitmap(5)));
  visual.envelope = 0;
  CHECK(blank(waveformDisplayGetBitmap(5)));
}

TEST_CASE_FIXTURE(VisualFixture, "Audio-only readout uses summed snapshots instead of detailed overlays") {
  auto* monitor = chipnomadState->audioMonitor;
  float mix[128]{};
  monitor->beginRender(); monitor->beginChunk(64);
  for (int i = 0; i < 128; ++i) monitor->add(0, i, 0.5f);
  monitor->finishChunk(mix, 64, 12000); monitor->publish(); monitorDisplayUpdate();
  appSettings.trackVisuals[0].mode = TrackVisualMode::audio;
  Bitmap* bitmap = waveformDisplayGetBitmap(0);
  REQUIRE(bitmap != nullptr);
  std::vector<uint8_t> before(bitmap->data, bitmap->data + bitmap->widthPixels * bitmap->heightPixels);
  appSettings.trackVisuals[0].envelope = appSettings.trackVisuals[0].noise = 0;
  bitmap = waveformDisplayGetBitmap(0);
  CHECK(std::equal(before.begin(), before.end(), bitmap->data));
  CHECK_FALSE(blank(bitmap));
  appSettings.trackVisuals[0].wave = 0;
  CHECK(blank(waveformDisplayGetBitmap(0)));
}

TEST_CASE("Audio mini readout preserves narrow peaks and leaves padding clear") {
  std::vector<uint8_t> pixels(16 * 24, 255);
  Bitmap bitmap{}; bitmap.widthPixels = 16; bitmap.heightPixels = 24; bitmap.data = pixels.data();
  float samples[256]{}; samples[7] = 1; samples[8] = -1; samples[250] = NAN;
  renderTrackAudioWaveform(&bitmap, samples, 256);
  CHECK(pixels[1 * 16 + 1] == 255);
  CHECK(pixels[21 * 16 + 1] == 255);
  for (int y=0;y<24;++y) { CHECK(pixels[y*16] == 0); CHECK(pixels[y*16+15] == 0); }
  for (int x=0;x<16;++x) { CHECK(pixels[x] == 0); CHECK(pixels[23*16+x] == 0); }
  std::fill_n(samples, 256, 0);
  renderTrackAudioWaveform(&bitmap, samples, 256);
  CHECK(pixels[11*16+5] == 64);
}
