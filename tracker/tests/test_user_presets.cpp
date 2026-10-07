#include "doctest.h"
#include "user_presets.h"
#include "packaged_presets.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <cstring>
#include "screens/user_preset_browser.h"
#include "screens/selection_popup.h"
#include "screens/screen_instrument.h"
#include "chipnomad_lib.h"
#include "app_ui_mock.h"

namespace fs = std::filesystem;
namespace {
const char* compressed = "tests/fixtures/preset-packs/compressed.zip";
const char* stored = "tests/fixtures/preset-packs/stored.zip";
size_t find(const UserPresets& browser, const std::string& name) {
  for (size_t i=0;i<browser.items().size();++i) if(browser.items()[i].name==name) return i;
  FAIL("Missing browser item: " << name); return 0;
}
struct Fixture {
  fs::path root="test-user-preset-library";
  Fixture(){fs::remove_all(root);fs::create_directories(root/"My sounds");fs::copy_file(compressed,root/"Collection.zip");}
  ~Fixture(){fs::remove_all(root);}
};
void write(const fs::path& path,const std::vector<uint8_t>& bytes){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());}
}

TEST_CASE("Preset ZIP stored and deflated entries agree without extracting") {
  PresetZip a,b;std::string error;REQUIRE(a.open(stored,error));REQUIRE(b.open(compressed,error));
  REQUIRE(a.entries().size()==b.entries().size());
  for(const auto& e:a.entries()){
    std::vector<uint8_t>x,y;REQUIRE(a.read(e.name,x,error));REQUIRE(b.read(e.name,y,error));CHECK(x==y);
  }
  std::vector<uint8_t> unchanged{1,2,3};CHECK_FALSE(b.read("missing.cni",unchanged,error));CHECK(unchanged==std::vector<uint8_t>{1,2,3});
  for(const auto* path:{"../outside.cni","/absolute.cni","a/../b.cni","C:/x.cni","a\\b.cni","a//b.cni"})CHECK_FALSE(PresetZip::safePath(path));
  CHECK(PresetZip::safePath("Collection/Bass/Preset.cni"));
}

TEST_CASE("Preset ZIP rejects truncated and damaged input transactionally") {
  Fixture f;std::vector<uint8_t> data;std::string error;REQUIRE(readPresetFile(stored,data,error));
  auto corrupted=data;corrupted[40]^=1;write(f.root/"bad.zip",corrupted);
  PresetZip zip;std::vector<uint8_t> output{42};
  CHECK_FALSE(zip.open((f.root/"bad.zip").string(),error));
  REQUIRE(zip.open(stored,error));
  const auto entry=zip.entries().front();
  REQUIRE(entry.size>0);
  corrupted=data;corrupted[entry.offset]^=1;write(f.root/"crc.zip",corrupted);
  REQUIRE(zip.open((f.root/"crc.zip").string(),error));
  CHECK_FALSE(zip.read(entry.name,output,error));
  CHECK(error=="Preset ZIP checksum mismatch");
  CHECK(output==std::vector<uint8_t>{42});
  data.resize(data.size()-10);write(f.root/"short.zip",data);CHECK_FALSE(zip.open((f.root/"short.zip").string(),error));CHECK(zip.entries().empty());
}

TEST_CASE("USER browses a complete factory collection ZIP without installing its catalog") {
  Fixture f;
  fs::copy_file("packaging/common/instruments/FACTORY/2-the-fat-man-4-op.zip",f.root/"Hundreds of sounds.zip");
  UserPresets b;std::string error;b.setup(f.root.string(),InstrumentType::OPL3);REQUIRE(b.refresh(error));
  REQUIRE(b.enter(find(b,"Hundreds of sounds.zip"),error));
  REQUIRE(b.items().size()>100);
  auto p=std::make_unique<Project>();projectInit(p.get());
  size_t loaded=0;
  for(size_t i=0;i<b.items().size();++i)if(b.items()[i].kind==UserPresets::Kind::preset){
    REQUIRE(b.load(i,p.get(),0,error));CHECK(p->instruments[0].type==InstrumentType::OPL3);++loaded;
  }
  CHECK(loaded>100);projectFree(p.get());
}

TEST_CASE("USER follows real folders and ZIP folders and filters CNI engine types") {
  Fixture f;UserPresets b;std::string error;b.setup(f.root.string(),InstrumentType::OPL3);REQUIRE(b.refresh(error));
  REQUIRE(b.enter(find(b,"My sounds/"),error));CHECK(b.items().empty());CHECK_FALSE(b.atRoot());REQUIRE(b.back(error));
  REQUIRE(b.enter(find(b,"Collection.zip"),error));REQUIRE(b.enter(find(b,"Collection/"),error));
  REQUIRE(b.items().size()==2);REQUIRE(b.enter(find(b,"Bass/"),error));REQUIRE(b.items().size()==1);
  auto p=std::make_unique<Project>();projectInit(p.get());
  REQUIRE(b.load(0,p.get(),3,error));CHECK(p->instruments[3].type==InstrumentType::OPL3);
  CHECK(std::string(p->instruments[3].name)=="Acoustic Grand");
  auto saved=p->instruments[3];
  REQUIRE(b.back(error));REQUIRE(b.back(error));REQUIRE(b.enter(find(b,"Other/"),error));CHECK(b.items().empty());
  CHECK_FALSE(b.load(0,p.get(),3,error));CHECK(!memcmp(&saved,&p->instruments[3],sizeof(saved)));
  fs::remove(f.root/"Collection.zip");CHECK(!memcmp(&saved,&p->instruments[3],sizeof(saved)));projectFree(p.get());
}

TEST_CASE("USER DX7 bank opens into 32 patches and native files remain private folders") {
  Fixture f;UserPresets b;std::string error;b.setup(f.root.string(),InstrumentType::DX7);REQUIRE(b.refresh(error));
  REQUIRE(b.enter(find(b,"Collection.zip"),error));REQUIRE(b.enter(find(b,"DX7/"),error));
  REQUIRE(b.enter(find(b,"Test.syx"),error));CHECK(b.items().size()==32);
  auto p=std::make_unique<Project>();projectInit(p.get());
  REQUIRE(b.load(31,p.get(),0,error));CHECK(p->instruments[0].type==InstrumentType::DX7);CHECK(p->instruments[0].chip.dx7.sourceProgram==31);
  REQUIRE(b.back(error));CHECK(b.items().size()==1);projectFree(p.get());
}

TEST_CASE("USER loose CNI discovery does not require a TSV or follow symlinks") {
  Fixture f;PresetZip zip;std::vector<uint8_t> data;std::string error;REQUIRE(zip.open(stored,error));REQUIRE(zip.read("Collection/OPL3.cni",data,error));
  write(f.root/"My sounds"/"Sound.cni",data);
  write(f.root/"bad.cni",std::vector<uint8_t>{'b','a','d'});
  std::error_code ec;fs::create_directory_symlink(f.root,f.root/"loop",ec);
  UserPresets b;b.setup(f.root.string(),InstrumentType::OPL3);REQUIRE(b.refresh(error));CHECK(b.items().size()==2);
  REQUIRE(b.enter(find(b,"My sounds/"),error));REQUIRE(b.items().size()==1);
  b.setup(f.root.string(),InstrumentType::OPL3);REQUIRE(b.refresh(error));CHECK(b.label()=="My sounds");
  auto p=std::make_unique<Project>();projectInit(p.get());REQUIRE(b.load(0,p.get(),0,error));projectFree(p.get());
  b.setup(f.root.string(),InstrumentType::OPL2);REQUIRE(b.refresh(error));REQUIRE(b.enter(find(b,"My sounds/"),error));CHECK(b.items().empty());
}

TEST_CASE("USER preserves access to previous banks alongside the new USER folder") {
  Fixture f;UserPresets b;std::string error;
  b.setup((f.root/"USER").string(),InstrumentType::OPL3,f.root.string());
  REQUIRE(b.refresh(error));REQUIRE(b.items().size()==1);
  REQUIRE(b.enter(find(b,"Previous banks folder/"),error));
  REQUIRE(b.enter(find(b,"Collection.zip"),error));
  REQUIRE(b.enter(find(b,"Collection/"),error));
  REQUIRE(b.enter(find(b,"Bass/"),error));
  auto p=std::make_unique<Project>();projectInit(p.get());
  REQUIRE(b.load(0,p.get(),0,error));CHECK(p->instruments[0].type==InstrumentType::OPL3);
  projectFree(p.get());
  while(!b.atRoot())REQUIRE(b.back(error));
  fs::create_directories(f.root/"USER");
  fs::copy_file(compressed,f.root/"USER"/"New pack.zip");
  REQUIRE(b.refresh(error));REQUIRE(b.items().size()==2);
  CHECK(b.items()[find(b,"Previous banks folder/")].legacy);
  REQUIRE(b.enter(find(b,"New pack.zip"),error));
  REQUIRE(b.enter(find(b,"Collection/"),error));
  REQUIRE(b.enter(find(b,"Bass/"),error));
  CHECK(b.items().size()==1);CHECK(fs::exists(f.root/"Collection.zip"));
}

TEST_CASE("Every factory CNI remains loadable from its portable ZIP collection") {
  auto p=std::make_unique<Project>();projectInit(p.get());fillFXNames();
  auto entries=packagedPresets();REQUIRE(entries.size()==1196);
  for(const auto& e:entries){CAPTURE(e.path);REQUIRE(!e.archive.empty());REQUIRE(loadFMPreset("packaging/common/instruments/FACTORY",e,p.get(),0));CHECK(int(p->instruments[0].type)==e.type);}
  projectFree(p.get());
}

TEST_CASE("USER popup selects a ZIP preset and backs through folders without changing a song") {
  Fixture f;
  const auto original=fs::current_path();
  fs::create_directories(f.root/"instruments/USER/opl3");
  fs::copy_file(compressed,f.root/"instruments/USER/opl3/Pack.zip");
  struct Restore {fs::path path;~Restore(){fs::current_path(path);}} restore{original};
  fs::current_path(f.root);
  auto* prior=chipnomadState;auto* state=chipnomadCreate();chipnomadState=state;
  screensInitAll();cInstrument=0;getInstrumentFunctions(InstrumentType::OPL3).init(&state->project.instruments[0]);
  state->project.instruments[0].type=InstrumentType::OPL3;
  auto before=state->project.instruments[0];
  auto select=[] {screenSelectionPopup.onInput(1,keyEdit,0);screenSelectionPopup.onInput(0,keyEdit,0);};
  openUserPresetBrowser();CHECK(currentScreen==&screenSelectionPopup);CHECK(selectionPopupIsFullWidth());
  select(); // Pack.zip
  select(); // Collection/
  select(); // Bass/
  CHECK(!memcmp(&before,&state->project.instruments[0],sizeof(before)));
  select(); // Acoustic Grand
  CHECK(currentScreen==&screenInstrument);CHECK(std::string(state->project.instruments[0].name)=="Acoustic Grand");
  openUserPresetBrowser(); // Return to Bass/, not to the root.
  auto selected=state->project.instruments[0];
  for(int i=0;i<4;++i)screenSelectionPopup.onInput(1,keyOpt,0);
  CHECK(currentScreen==&screenInstrument);CHECK(!memcmp(&selected,&state->project.instruments[0],sizeof(selected)));
  chipnomadDestroy(state);chipnomadState=prior;
}
