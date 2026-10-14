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
  for (int i = 0; i < 1024; ++i) {
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
