#include "doctest.h"
#include "soundbank.h"
#include "soundbank_memory.h"
#include "soundbank_browser.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <cstring>

namespace {
struct TemporaryAudio {
  std::filesystem::path path = std::filesystem::temp_directory_path() / "choochoo-codec-regression";
  TemporaryAudio() { std::filesystem::create_directories(path); }
  ~TemporaryAudio() { std::filesystem::remove_all(path); }
};
}
TEST_CASE("Sample cache preflight preserves pinned audio on a rejected load") {
  soundbank::SampleCache cache(768);
  auto first=cache.prepare(soundbank::parseBank({"tests/fixtures/soundbanks/tone.sf2",""}),"0:0");
  CHECK(cache.bytes()==512);
  CHECK_THROWS(cache.prepare(soundbank::parseBank({"tests/fixtures/soundbanks/tone.wav",""}),"sample"));
  CHECK(cache.bytes()==512);CHECK(first->zones[0].audio->frames()==256);
  cache.setLimit(0); // Memory pressure must not reject a zero-allocation reuse.
  CHECK_NOTHROW(cache.prepare(soundbank::parseBank({"tests/fixtures/soundbanks/tone.sf2",""}),"0:0"));
}
TEST_CASE("Logic AIFF alignment padding is accepted only with an exact COMM frame count") {
  TemporaryAudio temporary;
  auto path=temporary.path/"logic.aif";
  auto write=[&](int pad,int frames) {
    std::string data="FORM";
    auto word=[&](uint32_t n){for(int shift=24;shift>=0;shift-=8)data+=char(n>>shift);};
    word(50);data+="AIFFCOMM";word(18);
    data+=char(0);data+=char(1);word(frames);data+=char(0);data+=char(24);
    const unsigned char rate[10]={0x40,0x0e,0xbb,0x80,0,0,0,0,0,0};
    data.append(reinterpret_cast<const char*>(rate),10);
    data+="SSND";word(12);word(0);word(0);
    data+=char(0);data+=char(0x20);data+=char(0);data+=char(pad);
    std::ofstream out(path,std::ios::binary);out.write(data.data(),data.size());
  };
  write(0,1);
  soundbank::Asset asset{{path.string(),""}};
  CHECK(soundbank::decodedAudioBytes(asset)==2);
  auto audio=soundbank::decodeAudio(asset,2);
  REQUIRE(audio->pcm.size()==1);CHECK(audio->pcm[0]==32);CHECK_FALSE(audio->warnings.empty());
  write(1,1);CHECK_THROWS(soundbank::decodeAudio(asset,2));
  write(0,2);CHECK_THROWS(soundbank::decodeAudio(asset,2));
}
TEST_CASE("Automatic sample memory budgets reserve RAM and respect pressure") {
  constexpr uint64_t mib=1024*1024;
  CHECK(soundbank::automaticCacheBudget({})==64*mib);
  CHECK(soundbank::automaticCacheBudget({896*mib,640*mib})==224*mib);
  CHECK(soundbank::automaticCacheBudget({8192*mib,6144*mib})==2048*mib);
  CHECK(soundbank::automaticCacheBudget({8192*mib,128*mib})==64*mib);
  CHECK(soundbank::automaticCacheBudget({8192*mib,0})==0);
  CHECK(soundbank::automaticCacheBudget({8192*mib,128*mib},128*mib)==128*mib);
}
TEST_CASE("Static SFZ dynamic envelopes retain sound without a false unsupported warning") {
  TemporaryAudio temporary;
  std::filesystem::copy_file("tests/fixtures/soundbanks/tone.wav",temporary.path/"tone.wav",
    std::filesystem::copy_options::overwrite_existing);
  auto path=temporary.path/"dynamic.sfz";
  {std::ofstream out(path);out<<"<global> ampeg_dynamic=1 ampeg_attack=0.02 <region> sample=tone.wav key=60";}
  auto bank=soundbank::parseBank({path.string(),""});
  CHECK(bank.presets[0].warnings.empty());
  CHECK(bank.presets[0].zones[0].envelope.attack==doctest::Approx(.02f));
  {std::ofstream out(path);out<<"<global> ampeg_dynamic=1 ampeg_attackcc1=0.02 <region> sample=tone.wav key=60";}
  bank=soundbank::parseBank({path.string(),""});
  CHECK_FALSE(bank.presets[0].warnings.empty());
}
TEST_CASE("Collection browsing retains nested physical folders and hides ZIP sample trees") {
  std::vector<soundbank::BankEntry> banks;
  for(const char* id : {"Reddit/1offs/drumkits/Kick.sf2","Reddit/1offs/instruments/Organ.sf2",
      "Reddit/House/Bank/Collection.sf2","Sideloaded/VSCO/VSCO.zip::publisher/Strings/Violin.sfz"}) {
    soundbank::BankEntry b;b.id=id;b.format="sf2";b.presets.push_back({"0:0","Sound"});banks.push_back(b);
  }
  auto root=soundbank::browseCollections(banks,"");REQUIRE(root.size()==2);
  CHECK(root[0].name=="Reddit");CHECK(root[0].hasFolders);CHECK(root[0].presets.empty());
  auto reddit=soundbank::browseCollections(banks,"Reddit");REQUIRE(reddit.size()==2);
  CHECK(reddit[0].name=="1offs");CHECK(reddit[0].hasFolders);
  auto singles=soundbank::browseCollections(banks,"Reddit/1offs");REQUIRE(singles.size()==2);
  CHECK(singles[0].path=="Reddit/1offs/drumkits");CHECK(singles[0].presets.size()==1);
  auto vsco=soundbank::browseCollections(banks,"Sideloaded");REQUIRE(vsco.size()==1);
  CHECK_FALSE(vsco[0].hasFolders);CHECK(vsco[0].presets.size()==1);
}
TEST_CASE("An umbrella collection does not hide unrelated one-off samples") {
  std::vector<soundbank::BankEntry> banks;
  for(auto item : {std::pair{"Open/Gugak/Strings.exs","exs"},
      std::pair{"Open/Gugak/Samples/string.wav","wav"},
      std::pair{"Open/StargateDrums/kick.wav","wav"},
      std::pair{"Open/Fluid/Fluid.sf2","sf2"}}) {
    soundbank::BankEntry b;b.id=item.first;b.format=item.second;b.presets.push_back({"sample","Sound"});banks.push_back(b);
  }
  const soundbank::SupportingSamples supporting(banks);
  CHECK(supporting.hides(banks[1]));CHECK_FALSE(supporting.hides(banks[2]));
  const auto groups=soundbank::browseCollections(banks,"Open");
  REQUIRE(groups.size()==3);CHECK(groups[2].name=="StargateDrums");CHECK(groups[2].presets.size()==1);
}
