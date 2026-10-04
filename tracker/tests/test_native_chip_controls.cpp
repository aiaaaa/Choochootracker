#include "doctest.h"
#include "chipnomad_lib.h"
#include "simple_chip_presets.h"
#include "pitch_table_utils.h"
#include <memory>
#include <vector>
#include <cstring>
#include <cmath>
namespace {
void setControl(Instrument& i,int g,int value) {
 if(auto* t=instrumentFMToneSettings(&i)){if(g==genericModFMBrightness)t->brightness=value-63;else t->feedback=value;return;}
 auto& p=i.chip.simpleChip;
 switch(g){
 case genericModChipMode:p.mode=value;break;
 case genericModChipNoiseRate:p.noiseRate=value;break;
 case genericModChipNoiseDivisor:p.noiseDivisor=value;break;
 case genericModChipNoiseShift:p.noiseShift=value;break;
 case genericModChipSweepPeriod:p.sweepPeriod=value;break;
 case genericModChipSweepShift:p.sweepShift=value;break;
 case genericModChipSweepDirection:p.sweepNegate=value;break;
 case genericModChipEnvelopeInitial:p.envelopeInitial=value;break;
 case genericModChipEnvelopePeriod:p.envelopePeriod=value;break;
 case genericModChipEnvelopeDirection:p.envelopeIncrease=value;break;
 }
}
std::vector<float> render(InstrumentType type,int generic,int value,bool fx) {
 auto s=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);
 REQUIRE(projectLoad(&s->project,"packaging/common/projects/gm-midi-demo.cct")==0);
 auto& p=s->project;p.tracksCount=1;p.tickRate=50;p.linearPitch=1;calculateLinearPitchTable12TET(&p);
 for(auto& groove:p.grooves)for(auto& speed:groove.speed)speed=50;
 p.song[0][0]=0;p.chains[0].rows[0].phrase=0;p.chains[0].rows[0].transpose=0;phraseClear(&p.phrases[0]);
 auto& i=p.instruments[0];getInstrumentFunctions(type).init(&i);
 if(type==InstrumentType::SegaPSG&&generic==genericModChipNoiseRate)simpleChipApplyPreset(&i,7);
 if(auto* a=instrumentFMAmpSettings(&i)){a->enabled=1;a->sustain=255;a->release=35;}
 auto& r=p.phrases[0].rows[0];r.note=45;r.instrument=0;r.volume=15;
 if(generic==genericModEnvelopeAttack){if(fx){r.fx[0][0]=fxEAT;r.fx[0][1]=value;}else instrumentFMAmpSettings(&i)->attack=value;}
 else if(fx){r.fx[0][0]=instrumentNativeModDestination(type,generic)->fx;r.fx[0][1]=value;}
 else setControl(i,generic,value);
 auto before=std::make_unique<Project>(p);
 chipnomadInitChips(s.get(),48000,nullptr);chipnomadReserveRenderBuffers(s.get(),480);
 REQUIRE(chipnomadQueueProjectRefresh(s.get()));REQUIRE(chipnomadQueuePlaybackStartSong(s.get(),0,0,1));
 std::vector<float> out(48000);for(int block=0;block<50;++block)REQUIRE(chipnomadRender(s.get(),out.data()+block*960,480)==480);
 CHECK(!memcmp(before.get(),&p,sizeof(Project)));return out;
}
}
TEST_CASE("Native phrase macros reach the real voice without modifying saved parameters") {
 for(auto t:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7,InstrumentType::SegaPSG,InstrumentType::GBPulse,InstrumentType::GBNoise}) {
  CAPTURE(int(t));
  for(int g=genericModFMBrightness;g<genericModTotalCount;++g)if(auto* d=instrumentNativeModDestination(t,g)) {
   CAPTURE(g);int value=d->range==1?1:d->range/2;
   if(g==genericModFMBrightness)value=30;
   auto fx=render(t,g,value,true),saved=render(t,g,value,false);
   REQUIRE(fx.size()==saved.size());CHECK(fx==saved);
   double energy=0;for(float x:fx){REQUIRE(std::isfinite(x));energy+=x*x;}CHECK(energy>1e-8);
  }
  Instrument i{};getInstrumentFunctions(t).init(&i);
  if(instrumentFMAmpSettings(&i))CHECK(render(t,genericModEnvelopeAttack,80,true)==render(t,genericModEnvelopeAttack,80,false));
 }
}
