#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>

#include "doctest.h"
#include "soundbank.h"
#include "soundbank_index.h"
#include "synth/soundbank_voice.h"
using namespace soundbank;
static const std::string fixtures = "tests/fixtures/soundbanks/";
TEST_CASE("soundbank metadata, preparation and loops across native formats") {
  for (auto file : {"tone.wav", "tone.aiff", "tone.sf2", "tone.exs", "layers.sfz"}) {
    CAPTURE(file);
    auto b = parseBank({fixtures + file, ""});
    REQUIRE(b.presets.size() == 1);
    REQUIRE(!b.presets[0].zones.empty());
    SampleCache cache;
    auto p = cache.prepare(b, b.presets[0].id);
    REQUIRE(!p->zones.empty());
    CHECK(p->zones[0].audio->frames() == 256);
    CHECK(p->zones[0].audio->sampleRate == 48000);
    CHECK(p->zones[0].zone.root == 60);
    CHECK(cache.bytes() == 512);
  }
}
TEST_CASE("soundbank ZIP member loading is transparent and rejects traversal") {
  for (auto archive : {"stored.zip", "deflated.zip"}) {
    CAPTURE(archive);
    Archive zip(fixtures + archive);
    CHECK(zip.entries().size() == 5);
    for (auto file : {"tone.wav", "tone.aiff", "tone.sf2", "tone.exs", "layers.sfz"}) {
      auto b = parseBank({fixtures + archive, std::string("nested/") + file});
      SampleCache cache;
      CHECK_NOTHROW(cache.prepare(b, b.presets[0].id));
    }
  }
  CHECK_THROWS(Archive(fixtures + "traversal.zip"));
  CHECK_THROWS(normalizedPath("../../x"));
  CHECK_THROWS(normalizedPath("C:/x"));
  CHECK(normalizedPath("a/b/../c") == "a/c");
}
TEST_CASE("soundbank bad inputs and bounded sample cache fail explicitly") {
  CHECK_THROWS(parseBank({fixtures + "bad.sf2", ""}));
  auto missing = parseBank({fixtures + "missing.sfz", ""});
  SampleCache cache;
  CHECK_THROWS(cache.prepare(missing, "sfz"));
  auto b = parseBank({fixtures + "tone.sf2", ""});
  SampleCache small(128);
  CHECK_THROWS(small.prepare(b, "0:0"));
  CHECK_THROWS(cache.prepare(b, "missing-program"));
  auto path = std::filesystem::temp_directory_path() / "choochoo-invalid-octave.sfz";
  { std::ofstream out(path); out << "<region> sample=missing.wav key=c999999999"; }
  CHECK_THROWS(parseBank({path.string(), ""}));
  std::filesystem::remove(path);
}
TEST_CASE("soundbank selection, round robin, pan and note-off use fixed voices") {
  auto b = parseBank({fixtures + "layers.sfz", ""});
  CHECK(b.presets[0].zones[0].loKey == 48);
  CHECK(b.presets[0].zones[0].hiKey == 72);
  SampleCache cache;
  auto p = cache.prepare(b, "sfz");
  auto handle = publish(p);
  SoundbankVoice v;
  v.init(48000);
  v.configure(handle, 6000, 1, 127);
  v.noteOn();
  float out[2048]{};
  v.render(out, 1024);
  float left = 0, right = 0;
  for (int i = 0; i < 1024; ++i) {
    left += fabs(out[2 * i]);
    right += fabs(out[2 * i + 1]);
  }
  CHECK(left > 1);
  CHECK(right == 0);
  CHECK(v.active());
  v.noteOn();
  v.render(out, 1024);
  left = right = 0;
  // The outgoing pan has a short de-click tail; the new round-robin region
  // must remain right-only after that transition.
  for (int i = 192; i < 1024; ++i) {
    left += fabs(out[2 * i]);
    right += fabs(out[2 * i + 1]);
  }
  CHECK(left == 0);
  CHECK(right > 1);
  v.noteOff();
  v.render(out, 1024);
  CHECK_FALSE(v.active());
  v.configure(handle, 2000, 1, 127);
  v.noteOn();
  CHECK_FALSE(v.active());
  v.kill();
  collect([](uint32_t) { return false; });
  CHECK(prepared(handle) == nullptr);
}
TEST_CASE("soundbank cache reuses pinned samples and releases unused samples") {
  auto b = parseBank({fixtures + "tone.sf2", ""});
  SampleCache cache;
  auto a = cache.prepare(b, "0:0");
  auto c = cache.prepare(b, "0:0");
  CHECK(a->zones[0].audio.get() == c->zones[0].audio.get());
  cache.trim();
  CHECK(cache.bytes() == 512);
  a.reset();
  c.reset();
  cache.trim();
  CHECK(cache.bytes() == 0);
}

TEST_CASE("soundbank retrigger and kill join the previous output and finish their tails") {
  for (float rate : {44100.f, 48000.f, 96000.f}) {
    CAPTURE(rate);
    SampleCache cache;
    auto p = cache.prepare(parseBank({fixtures + "tone.wav", ""}), "sample");
    auto h = publish(p);
    SoundbankVoice voice;
    voice.init(rate);
    voice.configure(h, 6000, 1, 127);
    voice.noteOn();
    float out[2048];
    voice.render(out, 501);
    const float last = out[1000];
    REQUIRE(std::abs(last) > .01f);
    voice.configure(h, 4800, 1, 127);
    voice.noteOn();
    voice.render(out, 1);
    CHECK(out[0] == doctest::Approx(last));
    voice.render(out, 501);
    const float beforeKill = out[1000];
    REQUIRE(std::abs(beforeKill) > .01f);
    voice.kill();
    CHECK(voice.active());
    // A tail holds only output values; collecting the old preset is safe.
    collect([](uint32_t) { return false; });
    voice.render(out, 1);
    CHECK(out[0] == doctest::Approx(beforeKill));
    for (int frame = 0; frame < int(rate * .004f); ++frame) {
      voice.kill(); // Repeated track updates must not restart the fade.
      voice.render(out, 1);
      CHECK(std::abs(out[0]) <= std::abs(beforeKill) + 1e-6f);
    }
    CHECK(out[0] == 0);
    CHECK_FALSE(voice.active());
    voice.init(rate);
    voice.render(out, 1);
    CHECK(out[0] == 0);
  }
}

TEST_CASE("soundbank zero release and rapid retriggers remain continuous across render blocks") {
  SampleCache cache;
  auto p = cache.prepare(parseBank({fixtures + "tone.wav", ""}), "sample");
  auto h = publish(p);
  SoundbankVoice whole, split;
  auto controls = soundbankDefaultControls();
  controls.releaseMs = 0;
  for (auto* voice : {&whole, &split}) {
    voice->init(48000);
    voice->configure(h, 6000, 1, 127, 20000, controls);
    voice->noteOn();
  }
  float a[1024], b[1024], previous = 0;
  for (int event = 0; event < 8; ++event) {
    if (event == 7) { whole.noteOff(); split.noteOff(); }
    else if (event) { whole.noteOn(); split.noteOn(); }
    whole.render(a, 31);
    for (int i = 0; i < 31; ++i) split.render(b + 2 * i, 1);
    if (event) CHECK(a[0] == doctest::Approx(previous));
    for (int i = 0; i < 62; ++i) CHECK(a[i] == b[i]);
    previous = a[60];
  }
  whole.render(a, 512);
  CHECK_FALSE(whole.active());
  CHECK(a[1022] == 0);
  split.kill();
  collect([](uint32_t) { return false; });
}

TEST_CASE("soundbank authored one-shot ignores note OFF without adding a transition") {
  auto bank = parseBank({fixtures + "tone.wav", ""});
  auto& zone = bank.presets[0].zones[0];
  zone.loopExplicit = true; zone.loop = Loop::oneShot;
  SampleCache cache;
  auto h = publish(cache.prepare(bank, "sample"));
  SoundbankVoice normal, released;
  float a[256], b[256];
  for (auto* voice : {&normal, &released}) {
    voice->init(48000); voice->configure(h, 4800, 1, 127); voice->noteOn();
    voice->render(a, 100);
  }
  released.noteOff();
  normal.render(a, 128); released.render(b, 128);
  for (int n = 0; n < 256; ++n) CHECK(a[n] == b[n]);
  normal.kill(); released.kill();
  collect([](uint32_t) { return false; });
}

TEST_CASE("soundbank loop may begin before initial playback offset") {
  auto b = parseBank({fixtures + "tone.sf2", ""});
  auto& z = b.presets[0].zones[0];
  z.start = 128;
  z.loopStart = 32;
  z.loopEnd = 224;
  z.loop = Loop::sustain;
  SampleCache cache;
  auto p = cache.prepare(b, "0:0");
  auto h = publish(p);
  SoundbankVoice v;
  v.init(48000);
  v.configure(h, 6000, 1, 127);
  v.noteOn();
  float out[2048];
  v.render(out, 1024);
  CHECK(v.active());
  v.noteOff();
  v.render(out, 1024);
  CHECK_FALSE(v.active());
  v.kill();
  collect([](uint32_t) { return false; });
}
TEST_CASE("soundbank refuses excessive simultaneous layers before playback") {
  auto b = parseBank({fixtures + "tone.sf2", ""});
  auto z = b.presets[0].zones[0];
  b.presets[0].zones.assign(9, z);
  SampleCache cache;
  CHECK_THROWS(cache.prepare(b, "0:0"));
  for (int n = 0; n < 9; ++n) {
    b.presets[0].zones[n].sequenceLength = 9;
    b.presets[0].zones[n].sequencePosition = n + 1;
  }
  CHECK_NOTHROW(cache.prepare(b, "0:0"));
}
TEST_CASE("soundbank preparation chooses velocity layers and follows root pitch") {
  auto b = parseBank({fixtures + "tone.wav", ""});
  auto& z = b.presets[0].zones[0];
  z.loopExplicit = true;
  z.loop = Loop::off;
  z.loVel = 80;
  z.hiVel = 100;
  z.velocityTrack = 0;
  SampleCache cache;
  auto p = cache.prepare(b, "sample");
  auto h = publish(p);
  SoundbankVoice v;
  v.init(48000);
  v.configure(h, 6000, 1, 79);
  v.noteOn();
  CHECK_FALSE(v.active());
  v.configure(h, 6000, 1, 80);
  v.noteOn();
  float out[512];
  v.render(out, 255);
  CHECK(v.active());
  v.render(out, 2);
  CHECK_FALSE(v.active());
  v.configure(h, 7200, 1, 100);
  v.noteOn();
  v.render(out, 127);
  CHECK(v.active());
  v.render(out, 2);
  CHECK_FALSE(v.active());
  v.kill();
  collect([](uint32_t) { return false; });
}
TEST_CASE("soundbank root resolution and index survive rebuilding and detect missing samples") {
  namespace fs = std::filesystem;
  auto root = fs::temp_directory_path() / "choochoo-soundbank-index-test";
  fs::remove_all(root);
  fs::create_directories(root / "Korean_Gugak/Wind/Samples");
  fs::copy_file(fixtures + "tone.wav", root / "Korean_Gugak/Wind/Samples/Tone.WAV");
  fs::create_directories(root / "__MACOSX");
  std::ofstream(root / "__MACOSX/._Invalid.exs") << "resource fork";
  std::ofstream(root / "Korean_Gugak/Wind/._Flute.sfz") << "resource fork";
  auto sfz = root / "Korean_Gugak/Wind/Flute.sfz";
  {
    std::ofstream f(sfz);
    f << "<region> sample=samples/tone.wav key=60 unsupported_test=1";
  }
  BankIndex index(root.string());
  auto first = index.scan();
  REQUIRE(first.size() == 2);
  auto id = std::string("Korean_Gugak/Wind/Flute.sfz");
  auto bank = parseBank(index.resolve(id));
  SampleCache cache;
  CHECK_NOTHROW(cache.prepare(bank, "sfz"));
  auto cached = index.scan();
  CHECK(cached[0].id == first[0].id);
  CHECK(cached[0].warnings == first[0].warnings);
  CHECK_FALSE(cached[0].warnings.empty());
  CHECK(index.scan(true)[0].id == first[0].id);
  CHECK_THROWS(index.resolve("../outside.sfz"));
  CHECK_THROWS(index.resolve(id).relative("../../../outside.wav"));
  fs::remove(root / "Korean_Gugak/Wind/Samples/Tone.WAV");
  auto missing = index.scan();
  REQUIRE(missing.size() == 1);
  CHECK_FALSE(missing[0].warnings.empty());
  fs::remove_all(root);
}
TEST_CASE("soundbank SFZ inheritance keeps region overrides and rejects include cycles") {
  namespace fs = std::filesystem;
  auto root = fs::temp_directory_path() / "choochoo-sfz-inheritance-test";
  fs::remove_all(root);
  fs::create_directories(root);
  fs::copy_file(fixtures + "tone.wav", root / "tone.wav");
  auto file = root / "test.sfz";
  {
    std::ofstream f(file);
    f << "<control> default_path=.\n<global> sample=tone.wav lokey=10 hikey=100 "
         "volume=-6\n<master> tune=20\n<group> lovel=30 hivel=90\n<region> key=60 tune=5 "
         "loop_mode=loop_sustain\n<region> lokey=70 hikey=80";
  }
  auto bank = parseBank({file.string(), ""});
  REQUIRE(bank.presets[0].zones.size() == 2);
  auto& a = bank.presets[0].zones;
  CHECK(a[0].root == 60);
  CHECK(a[0].tune == 5);
  CHECK(a[0].loVel == 30);
  CHECK(a[1].tune == 20);
  CHECK(a[1].loKey == 70);
  SampleCache cache;
  CHECK_NOTHROW(cache.prepare(bank, "sfz"));
  {
    std::ofstream f(file);
    f << "#include \"test.sfz\"\n";
  }
  CHECK_THROWS(parseBank({file.string(), ""}));
  fs::remove_all(root);
}
TEST_CASE("soundbank SF3 decodes real mono Vorbis with both end conventions") {
  for (auto file : {"tone.sf3", "tone-exclusive.sf3"}) {
    auto b = parseBank({fixtures + file, ""});
    SampleCache cache;
    auto p = cache.prepare(b, "0:0");
    REQUIRE(p->zones.size() == 1);
    CHECK(p->zones[0].audio->channels == 1);
    CHECK(p->zones[0].audio->sampleRate == 11025);
    CHECK(p->zones[0].audio->frames() > 10000);
    CHECK(std::any_of(p->zones[0].audio->pcm.begin(), p->zones[0].audio->pcm.end(),
                      [](int16_t s) { return s != 0; }));
  }
}
TEST_CASE("soundbank release regions trigger only once on note-off") {
  auto b = parseBank({fixtures + "tone.wav", ""});
  auto& z = b.presets[0].zones[0];
  z.releaseTrigger = true;
  z.loopExplicit = true;
  z.loop = Loop::off;
  SampleCache cache;
  auto p = cache.prepare(b, "sample");
  auto h = publish(p);
  SoundbankVoice v;
  v.init(48000);
  v.configure(h, 6000, 1, 127);
  v.noteOn();
  CHECK_FALSE(v.active());
  v.noteOff();
  CHECK(v.active());
  float out[1024];
  v.render(out, 512);
  CHECK_FALSE(v.active());
  v.noteOff();
  CHECK_FALSE(v.active());
  v.kill();
  collect([](uint32_t) { return false; });
}

TEST_CASE("soundbank WAV tolerates missing odd chunk padding with a diagnostic") {
  auto original = openFile(fixtures + "tone.wav");
  auto bytes = original->bytes(0, original->size());
  const uint8_t odd[] = {'J', 'U', 'N', 'K', 1, 0, 0, 0, 42};
  bytes.insert(bytes.begin() + 12, std::begin(odd), std::end(odd));
  uint32_t size = uint32_t(bytes.size() - 8);
  for (int i = 0; i < 4; ++i) bytes[4 + i] = uint8_t(size >> (8 * i));
  auto path = std::filesystem::temp_directory_path() / "choochoo-odd-padding.wav";
  {
    std::ofstream out(path, std::ios::binary);
    out.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
  }
  SampleCache cache;
  auto prepared = cache.prepare(parseBank({path.string(), ""}), "sample");
  CHECK(prepared->bytes > 0);
  CHECK_FALSE(prepared->warnings.empty());
  std::filesystem::remove(path);
}
TEST_CASE("soundbank SFZ CC-only regions do not invalidate playable key regions") {
  auto path = std::filesystem::temp_directory_path() / "choochoo-cc-trigger.sfz";
  // Use a local sample beside the definition; absolute SFZ paths are not permitted.
  std::filesystem::copy_file(fixtures + "tone.wav", path.parent_path() / "choochoo-cc-tone.wav",
                             std::filesystem::copy_options::overwrite_existing);
  {
    std::ofstream out(path);
    out << "<region> sample=choochoo-cc-tone.wav key=60\n<region> sample=missing.wav lokey=-1 "
           "hikey=-1 on_locc64=127 on_hicc64=127\n";
  }
  auto bank = parseBank({path.string(), ""});
  CHECK(bank.presets[0].zones.size() == 1);
  CHECK_FALSE(bank.presets[0].warnings.empty());
  SampleCache cache;
  CHECK_NOTHROW(cache.prepare(bank, "sfz"));
  std::filesystem::remove(path);
  std::filesystem::remove(path.parent_path() / "choochoo-cc-tone.wav");
}

TEST_CASE("soundbank EXS relocation prefers sibling samples within its own collection") {
  namespace fs = std::filesystem;
  auto root = fs::temp_directory_path() / "choochoo-exs-relocation-test";
  fs::remove_all(root);
  fs::create_directories(root / "Gugak/Data/Sampler Instruments");
  fs::create_directories(root / "Gugak/Data/Samples");
  fs::create_directories(root / "Other/Samples");
  fs::copy_file(fixtures + "tone.exs", root / "Gugak/Data/Sampler Instruments/tone.exs");
  fs::copy_file(fixtures + "tone.wav", root / "Gugak/Data/Samples/tone.wav");
  fs::copy_file(fixtures + "tone.wav", root / "Other/Samples/tone.wav");
  auto bank = parseBank({(root / "Gugak/Data/Sampler Instruments/tone.exs").string(), "", root.string()});
  CHECK(bank.assets[0].source.relocated().path == (root / "Gugak/Data/Samples/tone.wav").string());
  SampleCache cache;
  CHECK_NOTHROW(cache.prepare(bank, "exs"));
  fs::create_directories(root / "Gugak/Data/Conflicting Samples");
  fs::copy_file(fixtures + "tone.wav", root / "Gugak/Data/Conflicting Samples/tone.wav");
  CHECK_THROWS(bank.assets[0].source.relocated());
  fs::remove_all(root);
}

TEST_CASE("soundbank EXS original directory suffix disambiguates duplicate sample names") {
  const auto file = fixtures + "exs-paths.zip";
  auto bank = parseBank({file, "Mappings/bows.exs"});
  REQUIRE(bank.assets.size() == 2);
  CHECK(bank.assets[0].source.relocated().member == "Samples/Bows/Forsythia/tone.wav");
  CHECK(bank.assets[1].source.relocated().member == "Samples/Bows/Horsehair/tone.wav");
  CHECK_THROWS((Location{file, "Missing/tone.wav"}).relocated());
  SampleCache cache;
  auto prepared = cache.prepare(bank, "exs");
  REQUIRE(prepared->zones.size() == 2);
  CHECK(prepared->zones[0].audio != prepared->zones[1].audio);

  namespace fs = std::filesystem;
  auto root = fs::temp_directory_path() / "choochoo-exs-path-hint-test";
  fs::remove_all(root);
  Archive archive(file);
  for (const auto& entry : archive.entries()) {
    auto target = root / entry.name;
    fs::create_directories(target.parent_path());
    auto data = archive.open(entry.name)->bytes(0, entry.size);
    std::ofstream out(target, std::ios::binary);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
  }
  bank = parseBank({(root / "Mappings/bows.exs").string(), "", root.string()});
  CHECK(bank.assets[0].source.relocated().path == (root / "Samples/Bows/Forsythia/tone.wav").string());
  CHECK(bank.assets[1].source.relocated().path == (root / "Samples/Bows/Horsehair/tone.wav").string());
  CHECK_NOTHROW(cache.prepare(bank, "exs"));
  fs::remove_all(root);
}

TEST_CASE("SoundFont linked sample flags do not override the authored pan generator") {
  std::ifstream in(fixtures+"tone.sf2",std::ios::binary);std::string bytes((std::istreambuf_iterator<char>(in)),{});
  auto shdr=bytes.find("shdr");REQUIRE(shdr!=std::string::npos);
  const auto path=std::filesystem::temp_directory_path()/"soundbank-linked-pan.sf2";
  for(int type:{2,4}) {
    bytes[shdr+8+44]=char(type);
    {std::ofstream out(path,std::ios::binary);out.write(bytes.data(),bytes.size());}
    auto bank=parseBank({path.string(),""});REQUIRE(bank.presets.size()==1);REQUIRE(bank.presets[0].zones.size()==1);
    CHECK(bank.presets[0].zones[0].pan==0);
    SampleCache cache;auto h=publish(cache.prepare(bank,"0:0"));SoundbankVoice voice;voice.init(48000);voice.configure(h,6000,1,127);voice.noteOn();
    float audio[2048];voice.render(audio,1024);double energy=0;
    for(int n=0;n<1024;++n){CHECK(audio[n*2]==audio[n*2+1]);energy+=audio[n*2]*audio[n*2];}
    CHECK(energy>0);voice.kill();collect([](uint32_t){return false;});
  }
  std::filesystem::remove(path);
}
