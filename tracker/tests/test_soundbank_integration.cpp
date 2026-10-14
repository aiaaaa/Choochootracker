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
#include "mocks/app_ui_mock.h"

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
  const SoundbankControls controls{-125, 2400, 55, 20, 150, 30, 250, 65, 900};
  i.chip.soundfont.controls = controls;
  auto path =
      (std::filesystem::temp_directory_path() / "choochoo-soundbank-roundtrip.cct").string();
  REQUIRE(projectSave(p.get(), path.c_str()) == 0);
  REQUIRE(projectLoad(q.get(), path.c_str()) == 0);
  CHECK(q->instruments[0].type == InstrumentType::SoundFont);
  CHECK(std::string(q->instruments[0].chip.soundfont.bank) == i.chip.soundfont.bank);
  CHECK(std::string(q->instruments[0].chip.soundfont.preset) == "exs");
  CHECK(q->instruments[0].chip.soundfont.prepared == 0);
  CHECK(memcmp(&q->instruments[0].chip.soundfont.controls, &controls, sizeof(controls)) == 0);
  REQUIRE(instrumentSave(p.get(), path.c_str(), 0) == 0);
  REQUIRE(instrumentLoad(q.get(), path.c_str(), 3) == 0);
  CHECK(std::string(q->instruments[3].chip.soundfont.bank) == i.chip.soundfont.bank);
  CHECK(q->instruments[3].chip.soundfont.prepared == 0);
  CHECK(memcmp(&q->instruments[3].chip.soundfont.controls, &controls, sizeof(controls)) == 0);
  CHECK(int(InstrumentType::SQ80ESQ) == 28);
  CHECK(int(InstrumentType::Monk) == 29);
  CHECK(int(InstrumentType::SoundFont) == 30);
  std::ifstream saved(path);
  std::string text((std::istreambuf_iterator<char>(saved)), {});
  saved.close();
  auto legacy = text;
  auto start = legacy.find("- Sound controls: ");
  REQUIRE(start != std::string::npos);
  legacy.erase(start, legacy.find('\n', start) - start + 1);
  { std::ofstream rewritten(path); rewritten << legacy; }
  REQUIRE(instrumentLoad(q.get(), path.c_str(), 4) == 0);
  auto defaults = soundbankDefaultControls();
  CHECK(memcmp(&q->instruments[4].chip.soundfont.controls, &defaults, sizeof(defaults)) == 0);
  auto invalid = text;
  start = invalid.find("- Sound controls: ");
  invalid.replace(start, invalid.find('\n', start) - start, "- Sound controls: 0 99999 -1 -1 -1 -1 -1 -1 -1");
  { std::ofstream rewritten(path); rewritten << invalid; }
  CHECK(instrumentLoad(q.get(), path.c_str(), 4) != 0);
  auto typeLine = text.find("- Type: 30");
  REQUIRE(typeLine != std::string::npos);
  text.replace(typeLine, 10, "- Type: 255");
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
  i.chip.soundfont.controls.cutoffHz = 100;
  REQUIRE(chipnomadQueueProjectRefresh(state.get()));
  REQUIRE(chipnomadQueuePlaybackPreviewNote(state.get(), 0, 48, 0));
  double filteredEnergy = 0;
  for (int n = 0; n < 10; ++n) {
    REQUIRE(chipnomadRender(state.get(), out, 480));
    for (float x : out) { REQUIRE(std::isfinite(x)); filteredEnergy += x*x; }
  }
  CHECK(filteredEnergy < energy * .5);

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
  CHECK(screenInstrumentSoundFont.getColumnCount(4) == 2);
}

TEST_CASE("SoundFont editor restores Bank defaults without changing the preset identity") {
  auto* savedState = chipnomadState;
  auto* savedScreen = currentScreen;
  const int savedInstrument = cInstrument, savedModified = projectModified;
  chipnomadState = chipnomadCreate(); cInstrument = 0;
  auto& i = chipnomadState->project.instruments[0];
  getInstrumentFunctions(InstrumentType::SoundFont).init(&i);
  strcpy(i.chip.soundfont.bank, "tone.sf2"); strcpy(i.chip.soundfont.preset, "0:0");
  i.chip.soundfont.prepared = 123;
  screenInstrumentSoundFont.onEdit(1,4,CellEditAction::tap);
  CHECK(currentScreen == &screenSoundFontEdit);
  screenSoundFontEdit.fullRedraw(); auto* table = mockScreenData;
  REQUIRE(table != nullptr);
  table->onEdit(0,1,CellEditAction::increase);
  CHECK(i.chip.soundfont.controls.cutoffHz == 20);
  table->onEdit(0,1,CellEditAction::decrease);
  CHECK(i.chip.soundfont.controls.cutoffHz == -1);
  table->onEdit(0,0,CellEditAction::decreaseBig);
  CHECK(i.chip.soundfont.controls.tuneCents == -100);
  table->onEdit(0,0,CellEditAction::clear);
  CHECK(i.chip.soundfont.controls.tuneCents == 0);
  i.chip.soundfont.controls.attackMs = 1800;
  table->onEdit(0,4,CellEditAction::clear);
  CHECK(i.chip.soundfont.controls.attackMs == -1);
  CHECK(std::string(i.chip.soundfont.bank) == "tone.sf2");
  CHECK(i.chip.soundfont.prepared == 123);
  chipnomadDestroy(chipnomadState);chipnomadState=savedState;currentScreen=savedScreen;
  cInstrument=savedInstrument;projectModified=savedModified;
}

TEST_CASE("SoundFont overrides shape audio without mutating bank zones or retargeting keys") {
  using namespace soundbank;
  auto bank = parseBank({"tests/fixtures/soundbanks/tone.sf2", ""});
  auto& zone = bank.presets[0].zones[0];
  zone.loop = Loop::continuous; zone.loKey = zone.hiKey = 60;
  zone.envelope = {}; zone.envelope.sustain=1;zone.envelope.release=.01;
  zone.cutoff=20000;zone.resonance=0;
  SampleCache cache;auto preset=cache.prepare(bank,"0:0");auto handle=publish(preset);
  SoundbankVoice voice;voice.init(48000);
  auto render = [&](const SoundbankControls& controls) {
    voice.kill();voice.configure(handle,6000,1,127,20000,controls);voice.noteOn();
    std::vector<float> out(9600);voice.render(out.data(),out.size()/2);
    for (float sample:out) REQUIRE(std::isfinite(sample));
    return out;
  };
  auto defaults=soundbankDefaultControls();auto original=render(defaults);
  auto changed=defaults;changed.cutoffHz=100;
  auto filtered=render(changed);
  auto energy=[](const auto& out) { double e=0;for(float x:out)e+=x*x;return e; };
  CHECK(energy(filtered)<energy(original)*.5);
  changed=defaults;changed.tuneCents=1200;auto tuned=render(changed);
  CHECK(energy(tuned)>0);CHECK(tuned!=original); // Key 60 still selects the same zone.
  changed=defaults;changed.delayMs=200;CHECK(energy(render(changed))==0);
  changed=defaults;changed.attackMs=1000;changed.releaseMs=200;
  auto slow=render(changed);CHECK(energy(slow)<energy(original)*.1);
  CHECK(voice.envelopeLevel()==doctest::Approx(.1).epsilon(.01));
  // Envelope changes on a held note wait for the next trigger.
  voice.configure(handle,6000,1,127,20000,defaults);
  voice.noteOff();std::vector<float> tail(4800);voice.render(tail.data(),2400);
  CHECK(voice.active());voice.render(tail.data(),2400);voice.render(tail.data(),2400);
  voice.render(tail.data(),2400);voice.render(tail.data(),1);CHECK_FALSE(voice.active());
  CHECK(preset->zones[0].zone.envelope.attack==0);
  CHECK(preset->zones[0].zone.cutoff==20000);
  changed=defaults;changed.cutoffHz=20;changed.resonanceTenths=400;
  render(changed); // Extreme filter settings must remain finite.
  changed=defaults;changed.attackMs=0;changed.decayMs=0;changed.holdMs=0;changed.sustainPercent=0;
  CHECK(energy(render(changed))==0);
  CHECK(render(defaults)==original);
  voice.kill();collect([](uint32_t){return false;});
}
