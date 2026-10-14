#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>

#include "chipnomad_lib.h"
#include "doctest.h"
#include "project_utils.h"
#include "screen_instrument.h"
#include "soundbank.h"
#include "soundbank_runtime.h"
#include "synth/soundbank_voice.h"

TEST_CASE("SoundFont project and instrument files retain identities without runtime handles") {
  auto p = std::make_unique<Project>(), q = std::make_unique<Project>();
  projectInitAY(p.get());
  projectInit(q.get());
  fillFXNames();
  auto& i = p->instruments[0];
  getInstrumentFunctions(InstrumentType::SoundFont).init(&i);
  strcpy(i.chip.soundfont.bank, "Korean_Gugak/Wind/Flute.exs");
  strcpy(i.chip.soundfont.preset, "exs");
  i.chip.soundfont.prepared = 12345;
  auto path =
      (std::filesystem::temp_directory_path() / "choochoo-soundbank-roundtrip.cct").string();
  REQUIRE(projectSave(p.get(), path.c_str()) == 0);
  REQUIRE(projectLoad(q.get(), path.c_str()) == 0);
  CHECK(q->instruments[0].type == InstrumentType::SoundFont);
  CHECK(std::string(q->instruments[0].chip.soundfont.bank) == i.chip.soundfont.bank);
  CHECK(std::string(q->instruments[0].chip.soundfont.preset) == "exs");
  CHECK(q->instruments[0].chip.soundfont.prepared == 0);
  REQUIRE(instrumentSave(p.get(), path.c_str(), 0) == 0);
  REQUIRE(instrumentLoad(q.get(), path.c_str(), 3) == 0);
  CHECK(std::string(q->instruments[3].chip.soundfont.bank) == i.chip.soundfont.bank);
  CHECK(q->instruments[3].chip.soundfont.prepared == 0);
  CHECK(int(InstrumentType::SoundFont) == 29);
  std::ifstream saved(path);
  std::string text((std::istreambuf_iterator<char>(saved)), {});
  saved.close();
  auto typeLine = text.find("- Type: 29");
  REQUIRE(typeLine != std::string::npos);
  text.replace(typeLine, 10, "- Type: 28");
  { std::ofstream rewritten(path); rewritten << text; }
  CHECK(instrumentLoad(q.get(), path.c_str(), 4) != 0);
  std::filesystem::remove(path);
  projectFree(p.get());
  projectFree(q.get());
}
TEST_CASE("SoundFont tracker preview renders then stops and pins queued assets") {
  auto state = std::unique_ptr<ChipNomadState, decltype(&chipnomadDestroy)>(chipnomadCreate(),
                                                                            chipnomadDestroy);
  projectInitAY(&state->project);
  auto& i = state->project.instruments[0];
  getInstrumentFunctions(InstrumentType::SoundFont).init(&i);
  soundbank::SampleCache cache;
  auto b = soundbank::parseBank({"tests/fixtures/soundbanks/tone.sf2", ""});
  auto p = cache.prepare(b, "0:0");
  auto h = soundbank::publish(p);
  i.chip.soundfont.prepared = h;
  chipnomadInitChips(state.get(), 48000, nullptr);
  chipnomadReserveRenderBuffers(state.get(), 480);
  REQUIRE(chipnomadQueueProjectRefresh(state.get()));
  chipnomadCollectSoundbanks(state.get());
  CHECK(soundbank::prepared(h) != nullptr);
  REQUIRE(chipnomadQueuePlaybackPreviewNote(state.get(), 0, 48, 0));
  float out[960];
  double energy = 0;
  for (int n = 0; n < 10; ++n) {
    REQUIRE(chipnomadRender(state.get(), out, 480));
    for (float x : out) {
      REQUIRE(std::isfinite(x));
      energy += x * x;
    }
  }
  CHECK(energy > 0.01);
  CHECK(state->soundbankVoices[0][0]->active());
  REQUIRE(chipnomadQueuePlaybackStopPreview(state.get(), 0));
  for (int n = 0; n < 20; ++n) chipnomadRender(state.get(), out, 480);
  CHECK_FALSE(state->soundbankVoices[0][0]->active());
  state.reset();
  soundbank::collect([](uint32_t) { return false; });
}
TEST_CASE("SoundFont uses the compact Instrument page and sample category") {
  CHECK(getInstrumentDefinition(InstrumentType::SoundFont)->category == InstrumentCategory::sample);
  CHECK(screenInstrumentSoundFont.rows == 5);
  CHECK(screenInstrumentSoundFont.getColumnCount(3) == 1);
  CHECK(screenInstrumentSoundFont.getColumnCount(4) == 1);
}
