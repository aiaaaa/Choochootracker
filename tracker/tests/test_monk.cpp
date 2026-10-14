#include "doctest.h"
#include "chipnomad_lib.h"
#include "pitch_table_utils.h"
#include "synth/monk_voice.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

static Instrument monkInstrument() {
  Instrument i{}; getInstrumentFunctions(InstrumentType::Monk).init(&i); return i;
}
static double energy(const std::vector<float>& audio) {
  double result=0;
  for (float sample : audio) { REQUIRE(std::isfinite(sample)); result += sample*sample; }
  return result;
}

TEST_CASE("MonkSynth renders all vowel landmarks at supported playback rates") {
  auto voice = std::make_unique<MonkSynthVoice>();
  auto i = monkInstrument();
  for (int rate : {22050, 44100, 48000, 96000}) for (int vowel : {0,64,128,191,255}) {
    CAPTURE(rate); CAPTURE(vowel);
    voice->init(rate); i.chip.monk.vowel=vowel;
    voice->configure(&i.chip.monk,6000,1,20000,0); voice->noteOn();
    std::vector<float> audio(rate/5); voice->render(audio.data(),audio.size());
    CHECK(energy(audio) > .001);
    CHECK(*std::max_element(audio.begin(),audio.end()) < 4);
    voice->noteOff(); audio.resize(rate);
    voice->render(audio.data(),audio.size());
    CHECK_FALSE(voice->active());
    CHECK(std::all_of(audio.end()-128,audio.end(),[](float x){return x==0;}));
  }
}

TEST_CASE("MonkSynth tone and motion controls alter the native sound") {
  for (int parameter=0;parameter<6;++parameter) {
    CAPTURE(parameter);
    auto a=std::make_unique<MonkSynthVoice>(), b=std::make_unique<MonkSynthVoice>();
    auto low=monkInstrument(), high=low;
    uint8_t* lo[]={&low.chip.monk.vowel,&low.chip.monk.character,&low.chip.monk.breath,&low.chip.monk.glide,&low.chip.monk.vibrato,&low.chip.monk.rate};
    uint8_t* hi[]={&high.chip.monk.vowel,&high.chip.monk.character,&high.chip.monk.breath,&high.chip.monk.glide,&high.chip.monk.vibrato,&high.chip.monk.rate};
    *lo[parameter]=0;*hi[parameter]=255;
    a->init(48000); b->init(48000);
    a->configure(&low.chip.monk,4800,1,20000,0); b->configure(&high.chip.monk,4800,1,20000,0);
    a->noteOn();b->noteOn();
    std::vector<float> x(12000),y(x.size());a->render(x.data(),x.size());b->render(y.data(),y.size());
    a->configure(&low.chip.monk,6000,1,20000,0); b->configure(&high.chip.monk,6000,1,20000,0);
    a->render(x.data(),x.size());b->render(y.data(),y.size());
    CHECK(energy(y) > .001);
    double difference=0;for(size_t n=0;n<x.size();++n)difference+=std::abs(x[n]-y[n]);
    CHECK(difference>.01);
  }
}

TEST_CASE("MonkSynth unchanged tick updates do not restart native glide") {
  auto a=std::make_unique<MonkSynthVoice>(), b=std::make_unique<MonkSynthVoice>();
  auto i=monkInstrument();i.chip.monk.glide=180;
  std::vector<float> warm(4800),x(48000),y(x.size());
  for(auto* voice : {a.get(),b.get()}) {
    voice->init(48000);voice->configure(&i.chip.monk,4800,1,20000,0);voice->noteOn();voice->render(warm.data(),warm.size());
    voice->configure(&i.chip.monk,6000,1,20000,0);
  }
  a->render(x.data(),x.size());
  for(size_t n=0;n<y.size();n+=480){b->configure(&i.chip.monk,6000,1,20000,0);b->render(y.data()+n,480);}
  CHECK(x==y);
  b->kill();b->render(y.data(),y.size());CHECK(energy(y)==0);
  b->noteOn();b->render(y.data(),y.size());CHECK(energy(y)>.001);
  b->init(44100);b->render(y.data(),y.size());CHECK(energy(y)==0);
}

TEST_CASE("MonkSynth instrument and song save every control and preserve old IDs") {
  CHECK(int(InstrumentType::SQ80ESQ)==28);CHECK(int(InstrumentType::Monk)==29);
  CHECK(fxTotalCount==254);
  fillFXNames();
  auto p=std::make_unique<Project>(),q=std::make_unique<Project>();projectInit(p.get());projectInit(q.get());
  REQUIRE(projectLoad(p.get(),"packaging/common/projects/gm-midi-demo.cct")==0);
  p->instruments[0]=monkInstrument();auto& m=p->instruments[0].chip.monk;
  std::strcpy(p->instruments[0].name,"Monk vowel");
  m.vowel=23;m.character=211;m.breath=99;m.glide=17;m.vibrato=166;m.rate=252;
  m.attack=31;m.decay=49;m.sustain=190;m.release=77;m.envelopeShape=2;
  m.filterMode=2;m.filterCutoffHz=4321;m.filterResonance=99;
  p->instruments[0].modulation[0].destination=3;
  p->phrases[0].rows[0].fx[0][0]=fxMWV;p->phrases[0].rows[0].fx[0][1]=255;
  REQUIRE(instrumentSave(p.get(),"test_monk.cni",0)==0);
  REQUIRE(instrumentLoad(q.get(),"test_monk.cni",0)==0);std::remove("test_monk.cni");
  CHECK(q->instruments[0].type==InstrumentType::Monk);
  CHECK(std::memcmp(&m,&q->instruments[0].chip.monk,sizeof(m))==0);
  REQUIRE(projectSave(p.get(),"test_monk.cct")==0);
  REQUIRE(projectLoad(q.get(),"test_monk.cct")==0);std::remove("test_monk.cct");
  CHECK(std::memcmp(&m,&q->instruments[0].chip.monk,sizeof(m))==0);
  CHECK(q->phrases[0].rows[0].fx[0][0]==fxMWV);
  CHECK(q->phrases[0].rows[0].fx[0][1]==255);
  projectFree(p.get());projectFree(q.get());
}

TEST_CASE("MonkSynth effect catalog starts with vowel and voice with correct preset values") {
  auto i=monkInstrument();const auto* d=getInstrumentDefinition(i.type);
  REQUIRE(d->fxCount==8);CHECK(d->fxList[0].fx==fxMWV);CHECK(d->fxList[1].fx==fxMIN);
  for(int n=0;n<8;++n) {
    NativeFXInfo info{};REQUIRE(instrumentNativeFXInfo(&i,d->fxList[n].fx,&info));
    CHECK(info.maximum==255);CHECK(info.minimum==0);CHECK(info.label!=nullptr);
    CHECK(instrumentFXAvailableForInstrument(&i,d->fxList[n].fx));
  }
  CHECK_FALSE(instrumentFXAvailable(i.type,fxMMD));
  CHECK(instrumentSetCCDestination(&i,3,127));CHECK(i.chip.monk.vowel==255);
  CHECK(instrumentSetCCDestination(&i,4,0));CHECK(i.chip.monk.character==0);
}

static std::vector<float> renderMonkTracker(bool fx, bool chord) {
  auto s=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);
  auto& p=s->project;
  REQUIRE(projectLoad(&p,"packaging/common/projects/gm-midi-demo.cct")==0);
  p.tracksCount=1;p.tickRate=50;p.linearPitch=1;calculateLinearPitchTable12TET(&p);
  for(auto& g:p.grooves)for(auto& speed:g.speed)speed=50;
  p.song[0][0]=0;p.chains[0].rows[0].phrase=0;p.chains[0].rows[0].transpose=0;
  phraseClear(&p.phrases[0]);p.instruments[0]=monkInstrument();
  auto& row=p.phrases[0].rows[0];row.note=45;row.instrument=0;row.volume=PHRASE_VOLUME_MAX;
  if(fx){row.fx[0][0]=fxMWV;row.fx[0][1]=240;}else p.instruments[0].chip.monk.vowel=240;
  if(chord){row.fx[1][0]=fxCRD;row.fx[1][1]=1;}
  auto saved=std::make_unique<Project>(p);
  chipnomadInitChips(s.get(),48000,nullptr);chipnomadReserveRenderBuffers(s.get(),480);
  REQUIRE(chipnomadQueueProjectRefresh(s.get()));REQUIRE(chipnomadQueuePlaybackStartSong(s.get(),0,0,1));
  std::vector<float> audio(9600);
  for(int b=0;b<10;++b)REQUIRE(chipnomadRender(s.get(),audio.data()+b*960,480)==480);
  CHECK(std::memcmp(saved.get(),&p,sizeof(Project))==0);
  CHECK(s->monkVoices[0][0]->active());
  if(chord)CHECK(s->playbackState.tracks[0].chordVoiceCount>1);
  chipnomadQueuePlaybackStop(s.get());
  std::vector<float> stop(960);chipnomadRender(s.get(),stop.data(),480);
  CHECK_FALSE(s->monkVoices[0][0]->active());
  return audio;
}
TEST_CASE("MonkSynth tracker FX matches saved vowel and chords render through the mixer") {
  auto saved=renderMonkTracker(false,false), fx=renderMonkTracker(true,false);
  CHECK(energy(saved)>.001);CHECK(saved==fx);
  auto chord=renderMonkTracker(false,true);CHECK(energy(chord)>.001);CHECK(chord!=saved);
}
