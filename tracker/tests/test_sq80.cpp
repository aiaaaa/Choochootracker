#include "sq80_patch.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "doctest.h"
#include "sq80_parameters.h"
#include "synth/sq80_voice.h"
#include "external_presets.h"
#include "chipnomad_lib.h"
#include "user_presets.h"
#include "screen_instrument.h"
#include "project_utils.h"
#include "mocks/app_ui_mock.h"
#include <filesystem>
#include <fstream>
#include <cmath>
#include <cstring>
#include <memory>
namespace {
namespace fs=std::filesystem;
const fs::path root=fs::temp_directory_path()/"choochoo-sq80-synthetic-tests";
void bank(){
 fs::create_directories(root);auto path=root/"hardware-80.hwb";
 std::ofstream f(path,std::ios::binary);
 auto put=[&](unsigned v,int n){while(n--){f.put(v&255);v>>=8;}};
 f.write("HYBWAV01",8);put(256,4);put(75,4);put(262144,4);put(0,4);
 for(int i=0;i<256;++i){f.write("SYNTHETIC TEST  ",16);for(int z=0;z<16;++z){put(0,4);put(1,1);put(0,1);put(0,1);put(0,1);}}
 for(unsigned i=0;i<262144;++i)put(unsigned(std::lround(128+100*std::sin(6.283185307179586*(i%256)/256))),1);
 f.close();std::string error;REQUIRE(sq80LoadBanks(root.string(),error));REQUIRE(sq80Bank(0));
}
std::vector<uint8_t> sysex(const InstrumentSQ80& p,int count=1){std::vector<uint8_t>b={240,15,2,0,uint8_t(count==1?1:2)};for(int k=0;k<count;++k)for(auto v:p.raw){b.push_back(v&15);b.push_back(v>>4);}b.push_back(247);return b;}
}
TEST_CASE("SQ80 native program import is transactional and retains extension wave IDs"){
 InstrumentSQ80 p;initSQ80Patch(p);p.raw[63]=255;
 std::vector<ExternalPreset> out;std::string error;
 REQUIRE(importExternalPresets(InstrumentType::SQ80ESQ,"bank.syx",sysex(p,40),out,error));REQUIRE(out.size()==40);CHECK(out[39].sourceIndex==39);CHECK(out[0].instrument.chip.sq80.raw[63]==255);
 auto bad=sysex(p);bad[5]=16;CHECK_FALSE(importExternalPresets(InstrumentType::SQ80ESQ,"bad.syx",bad,out,error));CHECK(out.size()==40);
 bad=sysex(p);bad.pop_back();CHECK_FALSE(importExternalPresets(InstrumentType::SQ80ESQ,"bad.syx",bad,out,error));CHECK(out.size()==40);
 CHECK(std::string(userPresetFolder(InstrumentType::SQ80ESQ))=="sq80esq");
}
TEST_CASE("SQ80 editor packed fields preserve neighboring flags and cover both profiles"){
 InstrumentSQ80 p;initSQ80Patch(p);
 for(int profile=0;profile<2;++profile){p.profile=profile;for(const auto&f:sq80Parameters()){
  CAPTURE(f.group);CAPTURE(f.label);
  for(int v:{f.minimum,0,f.maximum}){auto before=p;sq80Set(p,f,v);CHECK(sq80Get(p,f)==v);
   if(f.byte<102){for(int n=0;n<102;++n)if(n!=f.byte)CHECK(p.raw[n]==before.raw[n]);CHECK((p.raw[f.byte]&~f.mask)==(before.raw[f.byte]&~f.mask));}
  }
 }}
 p.raw[63]=201;CHECK(decodeSQ80(p).oscillators[0].wave==201);
}
TEST_CASE("SQ80 CNI and CCT preserve owned patches without waveform data"){
 fillFXNames();fs::create_directories(root);auto p=std::make_unique<Project>();projectInitAY(p.get());getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p->instruments[0]);auto& patch=p->instruments[0].chip.sq80;patch.raw[63]=255;patch.bank=1;patch.profile=1;
 auto expected=patch;auto cni=(root/"voice.cni").string();REQUIRE(instrumentSave(p.get(),cni.c_str(),0)==0);int result=instrumentLoad(p.get(),cni.c_str(),1);INFO(projectFileError);REQUIRE(result==0);CHECK(memcmp(&expected,&p->instruments[1].chip.sq80,sizeof(expected))==0);
 auto cct=(root/"song.cct").string();REQUIRE(projectSave(p.get(),cct.c_str())==0);auto q=std::make_unique<Project>();projectInit(q.get());REQUIRE(projectLoad(q.get(),cct.c_str())==0);CHECK(memcmp(&expected,&q->instruments[0].chip.sq80,sizeof(expected))==0);projectFree(p.get());projectFree(q.get());
}
TEST_CASE("SQ80 adapter renders stereo with stable block partition and releases"){
 bank();InstrumentSQ80 p;initSQ80Patch(p);p.raw[42]=0; // env4 release
 auto render=[&](size_t block){SQ80Voice v;v.init(48000);v.configure(&p,6000,1);v.noteOn();std::vector<float>out(4096*2);for(size_t at=0;at<4096;at+=block)v.render(out.data()+2*at,std::min(block,4096-at));return out;};
 auto a=render(4096),b=render(17);CHECK(a==b);double energy=0;for(float x:a){REQUIRE(std::isfinite(x));energy+=x*x;}CHECK(energy>0.0001);
 SQ80Voice v;v.init(48000);v.configure(&p,6000,1);v.noteOn();float out[512];v.render(out,256);v.noteOff();for(int i=0;i<400&&v.active();++i)v.render(out,256);CHECK_FALSE(v.active());
 v.noteOn();v.kill();v.render(out,256);for(float x:out)CHECK(x==0);
}
TEST_CASE("SQ80 editor opens full preset controls and leaves preset ownership intact"){
 auto*old=chipnomadState;auto*screen=currentScreen;int previous=cInstrument;
 chipnomadState=chipnomadCreate();initDefaultAppSettings();screensInitAll();cInstrument=0;getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&chipnomadState->project.instruments[0]);
 screenInstrumentSQ80.onEdit(0,5,CellEditAction::tap);CHECK(currentScreen==&screenSQ80Edit);screenSQ80Edit.fullRedraw();auto*d=mockScreenData;REQUIRE(d);CHECK(d->rows>100);
 auto before=chipnomadState->project.instruments[0].chip.sq80;d->onEdit(0,0,CellEditAction::increase);CHECK(chipnomadState->project.instruments[0].chip.sq80.raw[63]==before.raw[63]+1);
 screenSQ80Edit.onInput(1,keyOpt,1);CHECK(currentScreen==&screenInstrument);
 chipnomadDestroy(chipnomadState);chipnomadState=old;currentScreen=screen;cInstrument=previous;
}

TEST_CASE("SQ80 real tracker routing produces audio and preserves patch snapshots"){
 bank();fillFXNames();
 auto state=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);
 auto& p=state->project;projectInitAY(&p);p.tracksCount=1;p.tickRate=50;p.linearPitch=1;calculateLinearPitchTable12TET(&p);
 p.song[0][0]=0;p.chains[0].rows[0].phrase=0;p.chains[0].rows[0].transpose=0;phraseClear(&p.phrases[0]);
 getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p.instruments[0]);auto before=p.instruments[0];
 auto&row=p.phrases[0].rows[0];row.note=48;row.instrument=0;row.volume=PHRASE_VOLUME_MAX;
 chipnomadInitChips(state.get(),48000,nullptr);chipnomadReserveRenderBuffers(state.get(),480);
 REQUIRE(chipnomadQueueProjectRefresh(state.get()));REQUIRE(chipnomadQueuePlaybackStartSong(state.get(),0,0,1));
 double energy=0;float out[960];for(int n=0;n<8;++n){REQUIRE(chipnomadRender(state.get(),out,480)==480);for(float v:out){REQUIRE(std::isfinite(v));energy+=v*v;}}
 CHECK(energy>1e-5);CHECK(memcmp(&before,&p.instruments[0],sizeof(before))==0);
}
TEST_CASE("SQ80 optional private library import audit"){
 const char* assets=std::getenv("SQ80_PRIVATE_ASSETS");if(!assets)return;
 UserPresets library;library.setup(assets,InstrumentType::SQ80ESQ);std::string error;auto refs=library.scan(error);REQUIRE(refs.size()>0);
 auto p=std::make_unique<Project>();projectInitAY(p.get());unsigned checked=0;
 for(const auto& ref:refs){REQUIRE(library.load(ref,p.get(),0,error));REQUIRE(validSQ80(p->instruments[0].chip.sq80));++checked;}
 MESSAGE("Private presets parsed: ",checked);projectFree(p.get());
}
#endif
