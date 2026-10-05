#include "doctest.h"
#include "project.h"
#include "opll_presets.h"
#include "opl_patch.h"
#include "fm_macros.h"
#include "synth/opll_voice.h"
#include "synth/opl_voice.h"
#include "synth/four_op_voice.h"
#include "synth/dx7_voice.h"
#include "external/msfa/env.h"
#include <memory>
#include <vector>
#include <cstring>
#include <cmath>

namespace {
// Exercise the public adapter, including mid-note automation and a return to
// neutral, with arbitrary caller block lengths and a separate reference voice.
std::vector<float> audition(InstrumentType type,int macro,int offset,bool split) {
 Instrument i{};getInstrumentFunctions(type).init(&i);
 // Explicit modulation/decay fixtures: a held modulator or a disabled LFO
 // legitimately ignores its corresponding rate control.
 if(isOPLL(type)){i.chip.opll.patch[4]=0xf2;i.chip.opll.patch[6]=0x85;}
 if(type==InstrumentType::GenesisFM||type==InstrumentType::ArcadeFM) {
  auto& p=i.chip.fourOp;p.lfoEnabled=1;p.lfoRate=type==InstrumentType::GenesisFM?3:180;
  p.lfoWave=2;p.pitchDepth=50;p.amplitudeDepth=30;p.pitchSensitivity=4;p.amplitudeSensitivity=2;
 }
 if(type==InstrumentType::DX7){auto* p=i.chip.dx7.voice;p[137]=50;p[138]=0;p[139]=30;p[143]=4;p[142]=4;}
 auto* tone=instrumentFMToneSettings(&i);
 OPLLVoice opll;OPLVoice opl;FourOpVoice four;DX7Part dx;
 if(isOPLL(type))opll.init(48000);
 else if(isOPL(type))opl.init(48000);
 else if(type==InstrumentType::DX7)dx.init(48000);
 else four.init(48000);
 auto configure=[&]{
  if(isOPLL(type))opll.configure(&i.chip.opll,5700,.3f);
  else if(isOPL(type))opl.configure(type,&i.chip.opl,5700,.3f);
  else if(type==InstrumentType::DX7)dx.voices[0].configure(&i.chip.dx7,5700,.3f);
  else four.configure(type,&i.chip.fourOp,5700,.3f);
 };
 configure();
 if(isOPLL(type))opll.noteOn();else if(isOPL(type))opl.noteOn();else if(type==InstrumentType::DX7)dx.voices[0].noteOn();else four.noteOn();
 const bool stereo=isOPL(type)||type==InstrumentType::GenesisFM||type==InstrumentType::ArcadeFM;
 const int channels=stereo?2:1;
 std::vector<float> out(24000*channels);
 for(int section=0;section<3;++section) {
  tone->macro[macro]=section==1?offset:0;configure();
  for(int frame=0;frame<8000;) {
   int n=split?std::min(113,8000-frame):8000;
   auto* buffer=out.data()+(section*8000+frame)*channels;
   if(isOPLL(type))opll.render(buffer,n);else if(isOPL(type))opl.render(buffer,n);else if(type==InstrumentType::DX7)dx.render(buffer,n);else four.render(buffer,n);
   frame+=n;
  }
 }
 for(float sample:out)REQUIRE(std::isfinite(sample));
 return out;
}
}

TEST_CASE("FM macro support reflects native engine capabilities and neutral defaults") {
 fillFXNames();
 for(auto type:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7}) {
  Instrument i{};getInstrumentFunctions(type).init(&i);CAPTURE(int(type));
  for(int m=0;m<6;++m) {
   bool extended=type==InstrumentType::GenesisFM||type==InstrumentType::ArcadeFM||type==InstrumentType::DX7;
   bool supported=extended||m==fmTime||m==fmDecay||m==fmRatio;
   CHECK(bool(instrumentNativeModDestination(type,genericModFMTime+m))==supported);
   CHECK(bool(instrumentFXAvailable(type,fxFET+m))==supported);
   if(supported)CHECK(instrumentNativeControlValue(&i,genericModFMTime+m)==128);
  }
 }
 for(auto type:{InstrumentType::SegaPSG,InstrumentType::GBPulse,InstrumentType::GBNoise,InstrumentType::SID})
  for(int fx=fxFET;fx<=fxFLD;++fx)CHECK_FALSE(instrumentFXAvailable(type,fx));
 CHECK(fmMacroDelta(-128,15)==-15);CHECK(fmMacroDelta(127,15)==15);
 CHECK(fmMacroDelta(0,99)==0);
 InstrumentFMTone t{};t.macro[fmTime]=127;
 CHECK(fmMacroRate(0,15,t,false)==0);CHECK(fmMacroRate(8,15,t,false)==1);
 t.macro[fmTime]=-128;CHECK(fmMacroRate(8,15,t,false)==15);
}

TEST_CASE("Live FM macros preserve block independence at endpoints and neutral") {
 for(auto type:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7})
  for(int m=0;m<6;++m)if(instrumentNativeModDestination(type,genericModFMTime+m))
   for(int offset:{-128,0,127}) {
    CAPTURE(int(type));CAPTURE(m);CAPTURE(offset);
    CHECK(audition(type,m,offset,false)==audition(type,m,offset,true));
   }
}

TEST_CASE("DX7 native envelope rate changes keep the active stage and level") {
 using namespace choochoo_msfa;
 Env::init_sr(44100);Env env;
 int rates[]={30,25,20,35},levels[]={99,85,70,0};
 env.init(rates,levels,99*32,0);
 for(int i=0;i<50;++i)env.getsample();
 char before,after;env.getPosition(&before);
 int32_t level=env.getsample();
 int slow[]={0,0,0,0};env.setRates(slow);env.getPosition(&after);
 CHECK(before==after);CHECK(std::abs(int64_t(env.getsample())-level)<(1<<20));
 env.keydown(false);env.getPosition(&before);REQUIRE(before==3);
 env.setRates(rates);env.getPosition(&after);CHECK(after==3);
}

TEST_CASE("New FM and SID phrase and table commands survive portable saves") {
 fillFXNames();auto a=std::make_unique<Project>(),b=std::make_unique<Project>();
 projectInit(a.get());projectInit(b.get());
 REQUIRE(!projectLoad(a.get(),"packaging/common/projects/gm-midi-demo.cct"));
 getInstrumentFunctions(InstrumentType::DX7).init(&a->instruments[0]);
 for(int fx=fxFET;fx<=fxSPR;++fx){int row=fx-fxFET;a->phrases[0].rows[row].fx[0][0]=fx;a->phrases[0].rows[row].fx[0][1]=row*17;a->tables[0].rows[row].fx[0][0]=fx;a->tables[0].rows[row].fx[0][1]=row*17;}
 for(const char* file:{"build/tests/fm-macros.cct","build/tests/fm-macros.zip"}) {
  REQUIRE(!projectSave(a.get(),file));REQUIRE(!projectLoad(b.get(),file));
  for(int fx=fxFET;fx<=fxSPR;++fx){int row=fx-fxFET;CHECK(!memcmp(a->phrases[0].rows[row].fx,b->phrases[0].rows[row].fx,sizeof(a->phrases[0].rows[row].fx)));CHECK(!memcmp(a->tables[0].rows[row].fx,b->tables[0].rows[row].fx,sizeof(a->tables[0].rows[row].fx)));}
 }
 REQUIRE(!instrumentSave(a.get(),"build/tests/fm-macros.cni",0));REQUIRE(!instrumentLoad(b.get(),"build/tests/fm-macros.cni",1));
 for(int fx=fxFET;fx<=fxSPR;++fx){int row=fx-fxFET;CHECK(!memcmp(a->tables[0].rows[row].fx,b->tables[1].rows[row].fx,sizeof(a->tables[0].rows[row].fx)));}
 projectFree(a.get());projectFree(b.get());
}

TEST_CASE("Supported FM macros produce an audible change with active decay and LFO") {
 for(auto type:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7}) {
  auto neutral=audition(type,0,0,false);
  for(int m=0;m<6;++m)if(instrumentNativeModDestination(type,genericModFMTime+m)) {
   CAPTURE(int(type));CAPTURE(m);
   CHECK((audition(type,m,-128,false)!=neutral||audition(type,m,127,false)!=neutral));
  }
 }
}
