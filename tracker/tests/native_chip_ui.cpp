// Developer-only offscreen production UI integration; never shipped.
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include "chipnomad_lib.h"
#include "app.h"
#include "corelib_font.h"
#include "corelib_gfx.h"
#include "screens.h"
#include "screen_instrument.h"
#include "selection_popup.h"
#include "waveform_display.h"
#include "monitor_display.h"
#include "dx7_patch.h"
#include "copy_paste.h"
extern SDL_Renderer* renderer;
static const char* output;
static void require(bool condition,const char* why){if(!condition){fprintf(stderr,"FAIL: %s\n",why);exit(2);}}
static void capture(const char* name){appDraw();auto* s=SDL_CreateRGBSurfaceWithFormat(0,640,480,32,SDL_PIXELFORMAT_ARGB8888);require(s,"surface");require(SDL_RenderReadPixels(renderer,nullptr,s->format->format,s->pixels,s->pitch)==0,"pixels");char path[2048];snprintf(path,sizeof(path),"%s/%s.bmp",output,name);require(SDL_SaveBMP(s,path)==0,"capture");SDL_FreeSurface(s);}
static void key(int down,int keys){currentScreen->onInput(down,keys,1);appDraw();}
static void tapEdit(){key(1,keyEdit);key(0,keyEdit);}
int main(int argc,char** argv){
  if(argc!=2)return 1;output=argv[1];initDefaultAppSettings();appSettings.screenWidth=640;appSettings.screenHeight=480;fontSetCurrent(fontGetDefault());require(!gfxSetup(&appSettings.screenWidth,&appSettings.screenHeight),"SDL dummy setup");
  chipnomadState=chipnomadCreate();require(chipnomadState,"state");require(!projectLoad(&chipnomadState->project,"projects/gm-midi-demo.cct"),"fixture");
  chipnomadInitChips(chipnomadState,48000,nullptr);chipnomadReserveRenderBuffers(chipnomadState,1024);screensInitAll();waveformDisplayInit();monitorDisplayInit();
  for(auto type:{InstrumentType::OPLL,InstrumentType::VRC7,InstrumentType::OPL2,InstrumentType::OPL3,InstrumentType::SegaPSG,InstrumentType::GBPulse,InstrumentType::GBNoise,InstrumentType::GenesisFM,InstrumentType::ArcadeFM,InstrumentType::DX7}){
    getInstrumentFunctions(type).init(&chipnomadState->project.instruments[0]);screenSetup(&screenInstrument,0);if(type==InstrumentType::SegaPSG)require(!screenInstrumentSimpleChip.isCellValid(0,6),"read-only Sega clock skipped");char name[40];snprintf(name,sizeof(name),"instrument-%d",int(type));capture(name);
  }
  auto before=std::make_unique<Project>(chipnomadState->project);screenInstrumentOPL.onEdit(0,4,CellEditAction::tap);appDraw();require(currentScreen==&screenSelectionPopup,"shared FM browser");capture("dx7-categories");key(1,keyRight);capture("dx7-presets");
  key(1,keyEdit);require(currentScreen==&screenSelectionPopup,"EDIT waits to permit preview chord");key(1,keyEdit|keyPlay);
  std::vector<float> audio(2048);double energy=0;for(int n=0;n<12;++n){chipnomadRender(chipnomadState,audio.data(),1024);for(float x:audio)energy+=x*x;}require(energy>1e-5,"audition audio");require(!memcmp(before.get(),&chipnomadState->project,sizeof(Project)),"audition mutation");key(0,keyPlay);key(1,keyOpt);require(!memcmp(before.get(),&chipnomadState->project,sizeof(Project)),"cancel mutation");
  screenInstrumentOPL.onEdit(0,4,CellEditAction::tap);appDraw();key(1,keyRight);tapEdit();require(currentScreen==&screenInstrument,"confirm returns");require(!memcmp(before->tables,chipnomadState->project.tables,sizeof(before->tables)),"table changed");require(!memcmp(before->trackInserts,chipnomadState->project.trackInserts,sizeof(before->trackInserts)),"insert changed");capture("dx7-loaded");
  // Local SysEx opens the same transactional browser from another instrument.
  InstrumentDX7 patch{};initDX7Patch(&patch);char path[2048];snprintf(path,sizeof(path),"%s/original-test.syx",output);FILE* f=fopen(path,"wb");require(f,"syx fixture");uint8_t h[]={240,67,0,0,1,27};fwrite(h,1,6,f);fwrite(patch.voice,1,155,f);unsigned sum=0;for(auto b:patch.voice)sum+=b;fputc((-sum)&127,f);fputc(247,f);fclose(f);
  getInstrumentFunctions(InstrumentType::MME).init(&chipnomadState->project.instruments[0]);*before=chipnomadState->project;instrumentFMImportSysEx(path);appDraw();require(currentScreen==&screenSelectionPopup,"local import browser");require(!memcmp(before.get(),&chipnomadState->project,sizeof(Project)),"import must not commit");capture("dx7-user-bank");key(1,keyRight);tapEdit();require(chipnomadState->project.instruments[0].type==InstrumentType::DX7,"import commit type");require(!memcmp(patch.voice,chipnomadState->project.instruments[0].chip.dx7.voice,155),"import patch");
  auto original=chipnomadState->project.instruments[0];copyInstrument(0);pasteInstrument(3);require(!memcmp(&original,&chipnomadState->project.instruments[3],sizeof(Instrument)),"DX7 paste");
  require(cloneInstrument(0,4),"DX7 clone");require(!memcmp(&original,&chipnomadState->project.instruments[4],sizeof(Instrument)),"DX7 clone payload");
  for(auto type:{InstrumentType::GenesisFM,InstrumentType::ArcadeFM}){getInstrumentFunctions(type).init(&chipnomadState->project.instruments[0]);screenSetup(&screenInstrument,0);appDraw();screenInstrumentOPL.onEdit(0,4,CellEditAction::tap);appDraw();require(currentScreen==&screenSelectionPopup,"four-op browser");key(1,keyRight);key(1,keyEdit);key(1,keyEdit|keyPlay);for(int n=0;n<8;++n)chipnomadRender(chipnomadState,audio.data(),1024);key(0,keyPlay);tapEdit();require(currentScreen==&screenInstrument,"four-op confirm");char name[64];snprintf(name,sizeof(name),"four-op-%d-loaded",int(type));capture(name);}
  // Actual sequencer playback with UI rendering and the existing song sends.
  for(auto& i:chipnomadState->project.instruments)if(i.type==InstrumentType::Midi)getInstrumentFunctions(InstrumentType::DX7).init(&i);
  require(chipnomadQueueProjectRefresh(chipnomadState),"snapshot");chipnomadQueuePlaybackStartSong(chipnomadState,0,0,1);energy=0;std::vector<double> timings;
  for(int n=0;n<500;++n){auto start=std::chrono::steady_clock::now();chipnomadRender(chipnomadState,audio.data(),1024);appDraw();SDL_RenderFlush(renderer);timings.push_back(std::chrono::duration<double,std::micro>(std::chrono::steady_clock::now()-start).count());for(float x:audio)energy+=x*x;}
  require(energy>0.001,"sequencer playback");capture("dx7-song-playing");std::sort(timings.begin(),timings.end());printf("UI smoke passed; song+UI 48k/1024 us p95=%.3f p99=%.3f worst=%.3f energy=%.6f\n",timings[475],timings[495],timings.back(),energy);
  screenSetup(&screenInstrument,0);appDraw();screenInstrumentOPL.onEdit(0,4,CellEditAction::tap);appDraw();
  require(currentScreen==&screenSelectionPopup,"browser during playback");capture("fm-presets-during-playback");
  key(1,keyRight);key(1,keyEdit);key(1,keyEdit|keyPlay);chipnomadRender(chipnomadState,audio.data(),1024);
  require(chipnomadState->opllPreviewTrack<0,"playback cannot admit extra preview voice");key(0,keyPlay);key(1,keyOpt);
  chipnomadDestroy(chipnomadState);SDL_Quit();return 0;
}
