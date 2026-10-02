// Developer-only headless integration test; never included by production targets.
#include <SDL2/SDL.h>
#include "common.h"
#include "app.h"
#include "audio_manager.h"
#include "corelib_gfx.h"
#include "corelib_font.h"
#include "screens.h"
#include "experimental/mod_lucky/service.h"
#include "mod_lucky_fixtures.h"
#include <filesystem>
#include <thread>
#include <cstdio>
#include <cstdlib>
extern SDL_Renderer* renderer;
using namespace modLucky;
static void require(bool value,const char* text) { if(!value) { std::fprintf(stderr,"FAILED: %s\n",text);std::exit(1); } }
static void input(int key) { currentScreen->onInput(1,key,1);currentScreen->onInput(0,0,0);appDraw(); }
static void capture(const char* path) {
  auto* image=SDL_CreateRGBSurfaceWithFormat(0,640,480,32,SDL_PIXELFORMAT_ARGB8888);
  require(image,"surface");require(SDL_RenderReadPixels(renderer,nullptr,image->format->format,image->pixels,image->pitch)==0,"read pixels");
  require(SDL_SaveBMP(image,path)==0,"save pixels");SDL_FreeSurface(image);
}
static uint32_t pixel(int x,int y) { uint32_t p=0;SDL_Rect r{x,y,1,1};require(SDL_RenderReadPixels(renderer,&r,SDL_PIXELFORMAT_ARGB8888,&p,4)==0,"pixel");return p&0xffffff; }
static void finish(Service& s) {
  for(int i=0;i<2000;++i){appDraw();auto status=s.status();if(!status.busy())return;std::this_thread::sleep_for(std::chrono::milliseconds(2));}
  require(false,"worker completion");
}
int main(int argc,char** argv) {
  require(argc==2,"one output directory argument");
  require(std::getenv("SDL_VIDEODRIVER") && std::string(std::getenv("SDL_VIDEODRIVER"))=="dummy","headless video required");
  auto output=std::filesystem::absolute(argv[1]);std::filesystem::create_directories(output);std::filesystem::current_path(output);
  std::atomic<bool> release{false};std::atomic<int> calls{0},sequence{0};
  Service s([&](const std::string& url,size_t,const std::atomic<bool>& cancel){
    ++calls;while(!release && !cancel)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if(cancel)throw Error("Cancelled");
    if(url.find("view_player")!=std::string::npos){std::string id=std::to_string(++sequence);std::string html="path='jsplayer.php?moduleid="+id+"'; <a href='https://api.modarchive.org/downloads.php?moduleid="+id+"#own.mod'>download</a>";return HttpResponse{200,{html.begin(),html.end()}};}
    return HttpResponse{200,luckyFixture::mod(sequence)};
  });
  testService(&s);initDefaultAppSettings();appSettings.screenWidth=640;appSettings.screenHeight=480;
  auto samples=(output/"samples").string();std::snprintf(appSettings.samplePath,sizeof(appSettings.samplePath),"%s",samples.c_str());
  fontSetCurrent(nullptr);require(gfxSetup(&appSettings.screenWidth,&appSettings.screenHeight)==0,"SDL dummy renderer");
  appSetup();screenSetup(&screenSettings,0);appDraw();projectModified=0;
  auto before=snapshotProject(chipnomadState->project);
  for(int i=0;i<8;++i)input(keyDown);
  capture("next.bmp");auto cursor=unsigned(appSettings.colorScheme.cursor)&0xffffff;
  require(pixel(18*16,18*24-1)==cursor,"NEXT steady underline");
  currentScreen->onInput(1,keyEdit,1);appDraw();require(s.status().busy(),"NEXT entered loading");
  auto first=pixel(18*16,18*24-1);capture("loading-1.bmp");for(int i=0;i<3;++i)appDraw();
  require(first!=pixel(18*16,18*24-1),"underline brightness moves");capture("loading-2.bmp");
  for(int i=0;i<100;++i)currentScreen->onInput(1,keyEdit,1);
  release=true;finish(s);require(s.status().canLoad(),"ready candidate");require(!s.status().playing,"no autoplay");
  require(pixel(23*16,18*24-1)==cursor,"PLAY readiness underline");capture("ready.bmp");
  currentScreen->onInput(1,keyEdit,1);require(!s.status().playing,"held NEXT cannot trigger PLAY");
  currentScreen->onInput(0,0,0);input(keyEdit);require(s.status().playing,"fresh PLAY activation");
  require(snapshotProject(chipnomadState->project)==before && !projectModified,"preview preserves project");capture("playing.bmp");
  MainLoopEventData playEvent{};playEvent.type=MainLoopEvent::keyDown;playEvent.data.input={InputDeviceType::logical,keyPlay};appOnEvent(playEvent);
  playEvent.type=MainLoopEvent::keyUp;appOnEvent(playEvent);
  for(int i=0;i<200 && s.status().playing;++i)std::this_thread::sleep_for(std::chrono::milliseconds(2));
  require(!s.status().playing,"tracker transport stops preview");chipnomadQueuePlaybackStop(chipnomadState);
  input(keyRight);input(keyEdit);finish(s);require(s.status().phase==Phase::imported && projectModified,"LOAD commits and marks dirty");
  require(calls==2,"LOAD uses cache without network");capture("imported.bmp");
  require(projectSave(&chipnomadState->project,"imported.cct")==0,"save imported bank");
  input(keyLeft);input(keyLeft);release=false;input(keyEdit);require(s.status().busy(),"second NEXT");
  screenSetup(&screenSong,0);appDraw();release=true;std::this_thread::sleep_for(std::chrono::milliseconds(30));
  require(!s.status().playing && !s.status().canLoad(),"leave cancels late completion");
  appCleanup();testService(nullptr);gfxCleanup();
  auto reload=std::make_unique<Project>();projectInit(reload.get());require(projectLoad(reload.get(),"imported.cct")==0,"reload after shutdown");
  require(reload->instruments[0].chip.sample.data!=nullptr,"durable WAV restored");projectFree(reload.get());
  std::puts("PASS: Settings layout/animation, logical navigation/edge activation, audio transport, exact-cache import, save/reload, leave cancellation");
}
