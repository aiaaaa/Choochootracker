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
#include "pitch_table_utils.h"
#include "playback_internal.h"
#include "user_preset_browser.h"
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
 fillFXNames();fs::create_directories(root);auto p=std::make_unique<Project>();projectInitAY(p.get());getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p->instruments[0]);auto& patch=p->instruments[0].chip.sq80;patch.raw[63]=255;patch.bank=1;patch.profile=1;patch.wheel=51;patch.pedal=73;patch.factory=1;
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
 auto*old=chipnomadState;auto*screen=currentScreen;int previous=cInstrument;auto settings=appSettings;int modified=projectModified;
 chipnomadState=chipnomadCreate();initDefaultAppSettings();screensInitAll();cInstrument=0;getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&chipnomadState->project.instruments[0]);
 screenInstrumentSQ80.onEdit(1,4,CellEditAction::tap);CHECK(currentScreen==&screenSQ80Edit);screenSQ80Edit.fullRedraw();auto*d=mockScreenData;REQUIRE(d);CHECK(d->rows>100);
 auto before=chipnomadState->project.instruments[0].chip.sq80;d->onEdit(0,4,CellEditAction::increase);CHECK(chipnomadState->project.instruments[0].chip.sq80.raw[63]==before.raw[63]+1);
 chipnomadGetPlaybackStatus(chipnomadState);auto&status=chipnomadState->uiPlaybackStatus;status={};status.isPlaying=1;status.trackEnabled[0]=1;chipnomadState->project.tracksCount=1;status.tracks[0].mode=PlaybackMode::song;status.tracks[0].note.instrument=0;status.tracks[0].note.fx[fxPIT].isOn=1;
 d->cursorRow=6;d->topRow=4;screenMessage(0,"");screenSQ80Edit.draw();CHECK(mockGfxCells[5][27]=='*');CHECK(std::string(screenGetActiveMessage()).find("Track pitch")!=std::string::npos);
 auto saved=chipnomadState->project.instruments[0].chip.sq80;d->onEdit(0,6,CellEditAction::increase);CHECK(chipnomadState->project.instruments[0].chip.sq80.raw[58]==saved.raw[58]+1);CHECK(status.tracks[0].note.fx[fxPIT].isOn==1);
 status.trackEnabled[0]=0;screenSQ80Edit.draw();CHECK(mockGfxCells[5][27]!='*');
 d->cursorRow=0;screenSQ80Edit.draw();CHECK(std::string(screenGetActiveMessage()).find("Wheel:")==0);
 d->cursorRow=1;screenSQ80Edit.draw();CHECK(std::string(screenGetActiveMessage()).find("Pedal:")==0);
 screenMessage(MESSAGE_TIME,"Wave bank ready");d->cursorRow=2;screenSQ80Edit.draw();CHECK(std::string(screenGetActiveMessage())=="Wave bank ready");
 screenSQ80Edit.onInput(1,keyOpt,1);CHECK(currentScreen==&screenInstrument);
 chipnomadDestroy(chipnomadState);chipnomadState=old;currentScreen=screen;cInstrument=previous;appSettings=settings;projectModified=modified;
}

TEST_CASE("SQ80 bank chords defer the chooser and keep preset and editor on one row"){
 auto*old=chipnomadState;auto*screen=currentScreen;int previous=cInstrument;auto settings=appSettings;int modified=projectModified;
 chipnomadState=chipnomadCreate();initDefaultAppSettings();screensInitAll();cInstrument=0;
 auto& inst=chipnomadState->project.instruments[0];getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&inst);
 auto& d=screenInstrumentSQ80;d.cursorRow=3;d.cursorCol=0;currentScreen=&screenInstrument;
 d.drawStatic();d.drawField(0,3,CellState::normal);d.drawField(1,4,CellState::normal);d.drawField(0,4,CellState::normal);
 CHECK(std::string(mockGfxCells[6],4)=="Bank");CHECK(std::string(mockGfxCells[7],6)=="Preset");CHECK(std::string(mockGfxCells[7]+19,4)=="Edit");
 CHECK(d.rows==8);CHECK(d.getColumnCount(4)==2);
 CHECK(d.onInput(1,keyEdit,1)==1);CHECK(currentScreen==&screenInstrument);
 CHECK(d.onInput(0,0,1)==1);CHECK(currentScreen==&screenSelectionPopup);
 for(int direction:{keyLeft,keyRight}){
  currentScreen=&screenInstrument;auto before=inst.chip.sq80.factory;
  d.onInput(1,keyEdit,1);d.onInput(1,keyEdit|direction,1);
  CHECK(inst.chip.sq80.factory!=before);CHECK(currentScreen==&screenInstrument);
  d.onInput(0,keyEdit,1);d.onInput(0,0,1);CHECK(currentScreen==&screenInstrument);
 }
 d.cursorRow=4;currentScreen=&screenInstrument;
 d.onInput(1,keyEdit,1);CHECK(currentScreen==&screenInstrument);
 CHECK(d.onInput(1,keyEdit|keyPlay,1)==0); // Let common instrument input audition.
 d.onInput(0,keyEdit,1);d.onInput(0,0,1);CHECK(currentScreen==&screenInstrument);
 d.onInput(1,keyEdit,1);d.onInput(0,0,1);CHECK(currentScreen!=&screenInstrument);
 d.onEdit(1,4,CellEditAction::tap);CHECK(currentScreen==&screenSQ80Edit);
 d.cursorRow=0;d.cursorCol=0;
 chipnomadDestroy(chipnomadState);chipnomadState=old;currentScreen=screen;cInstrument=previous;appSettings=settings;projectModified=modified;
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
 UserPresets library;library.setup(assets,InstrumentType::SQ80ESQ);std::string error;auto refs=library.scan(error);REQUIRE(refs.size()>0);CHECK(error.find("limit")==std::string::npos);
 auto p=std::make_unique<Project>();projectInitAY(p.get());unsigned checked=0;
 for(const auto& ref:refs){REQUIRE(library.load(ref,p.get(),0,error));REQUIRE(validSQ80(p->instruments[0].chip.sq80));++checked;}
 MESSAGE("Private presets parsed: ",checked);projectFree(p.get());
}

TEST_CASE("SQ80 preset browser audition preserves native stereo pan"){
 bank();auto state=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);projectInitAY(&state->project);state->project.tracksCount=1;
 InstrumentSQ80 patch;initSQ80Patch(patch);patch.raw[100]=15;
 chipnomadInitChips(state.get(),48000,nullptr);chipnomadReserveRenderBuffers(state.get(),480);REQUIRE(chipnomadQueueProjectRefresh(state.get()));REQUIRE(chipnomadQueueSQ80Preview(state.get(),0,&patch));
 float out[960];double left=0,right=0;for(int n=0;n<8;++n){REQUIRE(chipnomadRender(state.get(),out,480)==480);for(int k=0;k<480;++k){left+=out[k*2]*out[k*2];right+=out[k*2+1]*out[k*2+1];}}
 CHECK(left>1e-5);CHECK(left>right*10);REQUIRE(chipnomadQueueSQ80Preview(state.get(),0,nullptr));
}

namespace hybrid {
struct EngineeringProbe {
 static std::vector<double> pressures(Synth& s){std::vector<double> r;for(auto&v:s.voices_)if(v.active)r.push_back(v.notePressure);return r;}
 static std::vector<double> pressureSources(Synth& s){std::vector<double> r;for(auto&v:s.voices_)if(v.active){s.inputSources(v);r.push_back(v.sources[14]);}return r;}
};
}
TEST_CASE("SQ80 pressure targets latest voice even with overlapping identical keys"){
 bank();hybrid::Synth synth(48000);synth.setWaveBank(sq80Bank(0));InstrumentSQ80 p;initSQ80Patch(p);synth.setPatch(decodeSQ80(p));
 float l[1024],r[1024];auto event=[&](hybrid::EventKind kind,float value,int key=60){hybrid::Event e{0,kind,0,uint8_t(key),value};REQUIRE(synth.render(l,r,1,&e,1));};
 event(hybrid::EventKind::noteOn,1);event(hybrid::EventKind::latestPressure,.25f);
 event(hybrid::EventKind::noteOn,1);event(hybrid::EventKind::latestPressure,.75f);
 auto pressure=hybrid::EngineeringProbe::pressures(synth);REQUIRE(pressure.size()==2);CHECK(pressure[0]==.25);CHECK(pressure[1]==.75);
 event(hybrid::EventKind::channelPressure,.5f);auto sources=hybrid::EngineeringProbe::pressureSources(synth);CHECK(sources[0]==64./128);CHECK(sources[1]==95./128);
 event(hybrid::EventKind::noteOn,1,62);pressure=hybrid::EngineeringProbe::pressures(synth);REQUIRE(pressure.size()==3);CHECK(pressure[2]==0);
 event(hybrid::EventKind::allSoundOff,0);CHECK(synth.activeVoices()==0);
}
TEST_CASE("SQ80 live controls alter held audio and sustain survives note release"){
 bank();InstrumentSQ80 p;initSQ80Patch(p);p.raw[42]=0;
 auto energy=[](const std::vector<float>&a){double e=0;for(float x:a){CHECK(std::isfinite(x));e+=x*x;}return e;};
 SQ80Voice low,high;low.init(48000);high.init(48000);low.configure(&p,6000,1);high.configure(&p,6000,1);low.noteOn();high.noteOn();
 std::vector<float>a(9600),b(9600);low.render(a.data(),4800);high.render(b.data(),4800);CHECK(a==b);
 SQ80Performance c;c.cutoff=0;low.performance(c);low.render(a.data(),4800);high.render(b.data(),4800);CHECK(energy(a)<energy(b)*.5);
 c.cutoff=-1;c.sustain=1;low.performance(c);low.noteOff();for(int i=0;i<20;++i)low.render(a.data(),4800);CHECK(low.active());
 c.sustain=0;low.performance(c);for(int i=0;i<30&&low.active();++i)low.render(a.data(),4800);CHECK_FALSE(low.active());
 // Controller routing must be audible on held notes, independently of saved patch bytes.
 for(int source:{11,12,13,14}){
  auto patch=p;patch.raw[65]=uint8_t(240|source);patch.raw[66]=100;patch.raw[64]=128; // zero base level; controller raises DCA
  SQ80Voice v;v.init(48000);v.configure(&patch,6000,1);v.noteOn();v.render(a.data(),4800);double before=energy(a);
  SQ80Performance controls;if(source==11)controls.wheel=127;if(source==12)controls.pedal=127;if(source==13)controls.external=127;if(source==14)controls.aftertouch=127;
  v.performance(controls);v.render(a.data(),4800);CHECK(energy(a)>before+1e-5);
 }
}
TEST_CASE("SQ80 roll commands have native ranges and do not modify stored instruments"){
 fillFXNames();auto p=std::make_unique<Project>();projectInitAY(p.get());auto&i=p->instruments[0];getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&i);auto before=i;
 auto state=std::make_unique<PlaybackState>();state->p=p.get();state->tracks[0].note.instrument=0;
 for(int fx:{fxQCF,fxQRS,fxWHL,fxPED,fxSUS,fxAFT,fxNPR,fxLFR,fxGLI,fxVEL,fxEXT}){
  NativeFXInfo info{};REQUIRE(instrumentNativeFXInfo(&i,fx,&info));CHECK(instrumentFXAvailableForInstrument(&i,fx));CHECK_FALSE(instrumentFXAvailable(InstrumentType::AY1,fx));
  FXCell command{FXId(fx),255};initFX(state.get(),0,&command,nullptr,0);CHECK(state->tracks[0].note.fx[fx].isOn);CHECK(state->tracks[0].note.fx[fx].fxValue==info.maximum);
 }
 CHECK(memcmp(&before,&i,sizeof(i))==0);CHECK(std::string(instrumentTypeName(i.type))=="SQ80/ESQ");CHECK(fxTPN==243);CHECK(fxQCF==244);CHECK(fxTotalCount<FX_ID_EMPTY);
 projectFree(p.get());
}
TEST_CASE("SQ80 browsing preserves velocity but resets performance controllers"){
 fs::create_directories(root/"browse");InstrumentSQ80 patch;initSQ80Patch(patch);auto bytes=sysex(patch);std::ofstream f(root/"browse"/"voice.syx",std::ios::binary);f.write((char*)bytes.data(),bytes.size());f.close();
 UserPresets library;library.setup((root/"browse").string(),InstrumentType::SQ80ESQ);std::string error;auto refs=library.scan(error);REQUIRE(refs.size()==1);
 auto p=std::make_unique<Project>();projectInitAY(p.get());getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p->instruments[0]);auto& current=p->instruments[0].chip.sq80;current.velocity=19;current.wheel=90;current.pedal=70;current.factory=1;
 REQUIRE(library.load(refs[0],p.get(),0,error));CHECK(current.velocity==19);CHECK(current.wheel==0);CHECK(current.pedal==0);CHECK(current.factory==1);
 projectFree(p.get());
}

TEST_CASE("SQ80 same-row roll controllers reach the newly triggered tracker voice"){
 bank();fillFXNames();
 for(auto entry:std::vector<std::pair<int,int>>{{fxWHL,11},{fxPED,12},{fxEXT,13},{fxAFT,14},{fxNPR,14}}){
  CAPTURE(entry.first);auto state=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);
  auto&p=state->project;projectInitAY(&p);p.tracksCount=1;p.tickRate=50;p.linearPitch=1;calculateLinearPitchTable12TET(&p);
  p.song[0][0]=0;p.chains[0].rows[0].phrase=0;p.chains[0].rows[0].transpose=0;phraseClear(&p.phrases[0]);getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p.instruments[0]);
  auto&patch=p.instruments[0].chip.sq80;patch.raw[64]=128;patch.raw[65]=240|entry.second;patch.raw[66]=126;
  auto&row=p.phrases[0].rows[0];row.note=48;row.instrument=0;row.volume=PHRASE_VOLUME_MAX;row.fx[0][0]=entry.first;row.fx[0][1]=127;
  chipnomadInitChips(state.get(),48000,nullptr);chipnomadReserveRenderBuffers(state.get(),480);REQUIRE(chipnomadQueueProjectRefresh(state.get()));REQUIRE(chipnomadQueuePlaybackStartSong(state.get(),0,0,1));
  double energy=0;float out[960];for(int n=0;n<8;++n){REQUIRE(chipnomadRender(state.get(),out,480)==480);for(float x:out){REQUIRE(std::isfinite(x));energy+=x*x;}}
  CHECK(energy>1e-5);CHECK(p.instruments[0].chip.sq80.wheel==0);
 }
}
TEST_CASE("SQ80 sustained chord tails remain audible after a smaller chord and stop cleanly"){
 bank();fillFXNames();
 auto state=std::unique_ptr<ChipNomadState,decltype(&chipnomadDestroy)>(chipnomadCreate(),chipnomadDestroy);
 auto&p=state->project;projectInitAY(&p);p.tracksCount=1;p.tickRate=50;p.linearPitch=1;calculateLinearPitchTable12TET(&p);
 p.song[0][0]=0;p.chains[0].rows[0].phrase=0;p.chains[0].rows[0].transpose=0;phraseClear(&p.phrases[0]);
 getInstrumentFunctions(InstrumentType::SQ80ESQ).init(&p.instruments[0]);p.instruments[0].chip.sq80.raw[42]=0;
 auto&first=p.phrases[0].rows[0];first.note=48;first.instrument=0;first.volume=PHRASE_VOLUME_MAX;first.fx[0][0]=fxCRD;first.fx[0][1]=0;first.fx[1][0]=fxSUS;first.fx[1][1]=1;
 p.phrases[0].rows[1].note=50;p.phrases[0].rows[2].fx[0][0]=fxSUS;p.phrases[0].rows[2].fx[0][1]=0;
 chipnomadInitChips(state.get(),48000,nullptr);chipnomadReserveRenderBuffers(state.get(),480);
 REQUIRE(chipnomadQueueProjectRefresh(state.get()));REQUIRE(chipnomadQueuePlaybackStartSong(state.get(),0,0,1));
 float out[960];REQUIRE(chipnomadRender(state.get(),out,480)==480);REQUIRE(state->playbackState.tracks[0].chordVoiceCount==3);
 int blocks=0;while(state->playbackState.tracks[0].chordVoiceCount==3&&blocks++<40)chipnomadRender(state.get(),out,480);
 REQUIRE(state->playbackState.tracks[0].chordVoiceCount==1);CHECK(state->sq80Voices[0][1]->active());CHECK(state->sq80Voices[0][2]->active());
 state->sq80Voices[0][0]->kill(); // Isolate the former chord tones in the actual mixer.
 chipnomadRender(state.get(),out,480);double energy=0;for(float x:out)energy+=x*x;CHECK(energy>1e-5);
 // Stop before the looping phrase retriggers its first sustained chord.
 for(int n=0;n<80;++n)chipnomadRender(state.get(),out,480);
 CHECK(state->playbackState.tracks[0].note.fx[fxSUS].fxValue==0);
 CHECK_FALSE(state->sq80Voices[0][1]->active());CHECK_FALSE(state->sq80Voices[0][2]->active());
 chipnomadQueuePlaybackStop(state.get());chipnomadRender(state.get(),out,480);for(auto*v:state->sq80Voices[0])CHECK_FALSE(v->active());
}
TEST_CASE("SQ80 Factory collections contain thirteen loadable credited presets"){
 UserPresets library;library.setup("packaging/common/instruments/FACTORY/sq80esq",InstrumentType::SQ80ESQ);std::string error;auto refs=library.scan(error);INFO(error);REQUIRE(refs.size()==13);
 auto p=std::make_unique<Project>();projectInitAY(p.get());for(const auto&ref:refs){CAPTURE(ref.path);REQUIRE(library.load(ref,p.get(),0,error));CHECK(validSQ80(p->instruments[0].chip.sq80));}projectFree(p.get());
}
#endif
