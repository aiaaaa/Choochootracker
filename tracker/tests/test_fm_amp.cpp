#include "doctest.h"
#include "project.h"
#include "fm_amp.h"
#include "opl_patch.h"
#include "chipnomad_lib.h"
#include "pitch_table_utils.h"
#include "synth/native_fm_amp.h"
#include <cmath>
#include <memory>
#include <cstring>
#include <vector>

TEST_CASE("FM event smoothing preserves steady signal and bridges retriggers") {
 for(float rate:{44100.f,48000.f,96000.f}) {
  CAPTURE(rate);NativeFMAmp amp;amp.init(rate);InstrumentFMAmp settings{};
  amp.configure(settings,1);amp.noteOn();
  CHECK(amp.process(1)==0);
  float previous=0;
  for(int n=1;n<int(rate*.003f);++n) {
   float value=amp.process(1);CHECK(value>=previous);CHECK(value-previous<.012f);previous=value;
  }
  CHECK(amp.process(1)==1);CHECK(amp.process(.25f)==.25f);
  amp.noteOn();CHECK(amp.process(-1)==.25f);
  previous=.25f;
  for(int n=1;n<int(rate*.003f);++n) {
   float value=amp.process(-1);CHECK(std::abs(value-previous)<.015f);previous=value;
  }
  CHECK(amp.process(-1)==-1);
 }
}
TEST_CASE("FM optional amp ADSR shapes and releases independently of its native envelope") {
 NativeFMAmp amp;amp.init(48000);InstrumentFMAmp p{};
 p.enabled=1;p.attack=36;p.decay=36;p.sustain=128;p.release=36;p.envelopeShape=128;
 amp.configure(p,1);amp.noteOn();
 CHECK(amp.process(1)==0);
 float atTen=0,held=0;
 for(int n=1;n<15000;++n){float x=amp.process(1);if(n==480)atTen=x;held=x;}
 CHECK(atTen<.12f);CHECK(held==doctest::Approx(128.f/255));
 amp.noteOff();for(int n=0;n<6000;++n)held=amp.process(1);CHECK(held==0);
 p.enabled=0;amp.configure(p,1);CHECK(amp.process(1)==1);
}
TEST_CASE("FM amp extension is strict and optional") {
 InstrumentFMAmp p{};bool seen=false;
 CHECK(loadFMAmpSetting("- FM amp: 1,12,34,56,78,128",p,seen)==1);
 CHECK(p.enabled==1);CHECK(p.attack==12);CHECK(p.sustain==56);
 CHECK(loadFMAmpSetting("- FM amp: 1,12,34,56,78,128",p,seen)==-1);
 for(const char* bad:{"- FM amp: 2,0,0,0,0,0","- FM amp: 1,-1,0,0,0,0","- FM amp: 1,0,0,256,0,0","- FM amp: 1,0,0,0,0,0junk"}) {
  seen=false;CHECK(loadFMAmpSetting(bad,p,seen)==-1);
 }
}
TEST_CASE("All native FM types save reopen their independent amp and expose ADSR modulation") {
 auto p=std::make_unique<Project>(),q=std::make_unique<Project>();projectInit(p.get());projectInit(q.get());fillFXNames();
 REQUIRE(projectLoad(p.get(),"packaging/common/projects/gm-midi-demo.cct")==0);
 for(auto type:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7}) {
  CAPTURE(int(type));getInstrumentFunctions(type).init(&p->instruments[0]);
  auto* a=instrumentFMAmpSettings(&p->instruments[0]);REQUIRE(a);CHECK(a->enabled==0);
  a->enabled=1;a->attack=17;a->decay=91;a->sustain=123;a->release=47;a->envelopeShape=99;
  auto* tone=instrumentFMToneSettings(&p->instruments[0]);tone->brightness=-27;tone->feedback=6;
  auto before=p->instruments[0];
  REQUIRE(instrumentSave(p.get(),"test_fm_amp.cni",0)==0);
  REQUIRE(instrumentLoad(q.get(),"test_fm_amp.cni",2)==0);
  CHECK(!memcmp(a,instrumentFMAmpSettings(&q->instruments[2]),sizeof(*a)));
  CHECK(!memcmp(tone,instrumentFMToneSettings(&q->instruments[2]),sizeof(*tone)));
  CHECK(!memcmp(&before,&p->instruments[0],sizeof(before)));
  const int first=getInstrumentFunctions(type).modDestinationsCount+1;
  CHECK(instrumentModDestinationAvailable(&p->instruments[0],first+genericModEnvelopeAttack));
  CHECK_FALSE(instrumentModDestinationAvailable(&p->instruments[0],first+genericModTriggerDecay));
  REQUIRE(projectSave(p.get(),"test_fm_amp.cct")==0);
  REQUIRE(projectLoad(q.get(),"test_fm_amp.cct")==0);
  CHECK(!memcmp(a,instrumentFMAmpSettings(&q->instruments[0]),sizeof(*a)));
  CHECK(!memcmp(tone,instrumentFMToneSettings(&q->instruments[0]),sizeof(*tone)));
 }
 std::remove("test_fm_amp.cni");std::remove("test_fm_amp.cct");projectFree(p.get());projectFree(q.get());
}

TEST_CASE("FM tone extension rejects out of range and duplicate controls") {
 InstrumentFMTone t{};bool seen=false;
 CHECK(loadFMToneSetting("- FM tone: -63,8",t,seen)==1);
 CHECK(t.brightness==-63);CHECK(t.feedback==8);
 CHECK(loadFMToneSetting("- FM tone: 0,0",t,seen)==-1);
 for(const char* bad:{"- FM tone: -64,0","- FM tone: 64,0","- FM tone: 0,9","- FM tone: 0,-1","- FM tone: 0,0junk"}) {
  seen=false;CHECK(loadFMToneSetting(bad,t,seen)==-1);
 }
}
TEST_CASE("Native macros append IDs and expose only supported controls") {
 CHECK(genericModFirstInsert==29);CHECK(genericModFMBrightness==45);
 CHECK(fxFBR==fxF28+1);CHECK(fxTotalCount<255);fillFXNames();
 for(int type=0;type<int(InstrumentType::totalCount);++type) {
  Instrument i{};getInstrumentFunctions(InstrumentType(type)).init(&i);
  for(int g=genericModFMBrightness;g<genericModTotalCount;++g) {
   int dest=getInstrumentFunctions(i.type).modDestinationsCount+1+g;
   const auto* d=instrumentNativeModDestination(i.type,g);
   const bool unusedOPL=isOPL(i.type)&&i.chip.opl.topology==OPLTopology::twoOperator&&g>=genericModFMOperator3&&g<=genericModFMOperator6;
      CHECK(bool(instrumentModDestinationAvailable(&i,dest))==(bool(d)&&!unusedOPL));
   REQUIRE(instrumentModDestinationName(i.type,dest));
   if(d&&!unusedOPL){CHECK(instrumentFXAvailable(i.type,d->fx));CHECK(strcmp(fxNames[d->fx].name,"---"));
    uint8_t fx;int base,range;InstrumentMotionValue encoding;
    REQUIRE(instrumentMotionDestination(&i,dest,&fx,&base,&range,&encoding));
    CHECK(fx==d->fx);CHECK(base>=0);CHECK(base<=range);
   }
  }
 }
}
