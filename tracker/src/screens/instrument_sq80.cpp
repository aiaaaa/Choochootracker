#include "screen_instrument.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "user_preset_browser.h"
#include "selection_popup.h"
#include "corelib_file.h"
#include "corelib_gfx.h"
#include "project_utils.h"
#include "sq80_patch.h"
#include <string>
#include <filesystem>
#include <fstream>
#include "preset_zip.h"
#ifdef WEB_BUILD
#include <emscripten.h>
#endif
namespace {
Instrument* current(){return &chipnomadState->project.instruments[cInstrument];}
int held=0;
int columns(int r){return r<3?instrumentCommonColumnCount(r):1;}
int y(int r){return r+3;}
void drawStatic(){instrumentCommonDrawStatic();gfxSetFgColor(appSettings.colorScheme.textDefault);
 gfxPrint(0,6,"Bank:");gfxPrint(0,7,"Preset:");gfxPrint(0,8,"Editor:");gfxPrint(0,9,"Waves:");gfxPrint(0,10,"Load:");gfxPrint(0,11,"Velocity:");gfxPrint(0,12,"Profile:");
 gfxSetFgColor(appSettings.colorScheme.textInfo);gfxPrint(0,14,"Experimental SQ80/ESQ engine");gfxPrint(0,15,"EDIT+PLAY: audition");gfxPrint(0,16,"Patch edits apply on next note");}
void drawCursor(int c,int r){if(r<3)instrumentCommonDrawCursor(c,r);else gfxCursor(10,y(r),r==6?20:24);}
void drawField(int c,int r,CellState s){if(r<3){instrumentCommonDrawField(c,r,s);return;}
 const auto&p=current()->chip.sq80;gfxClearRect(10,y(r),28,1);gfxSetFgColor(s==CellState::focus?appSettings.colorScheme.textValue:appSettings.colorScheme.textDefault);
 if(r==3)gfxPrint(10,y(r),"USER");if(r==4)gfxPrintf(10,y(r),"%.24s",current()->name);if(r==5)gfxPrint(10,y(r),"EDIT PRESET >");
 if(r==6)gfxPrintf(10,y(r),"%s %s",p.bank?"ESQ-1":"SQ-80",sq80Bank(p.bank)?"ready":"MISSING");
 if(r==7)gfxPrint(10,y(r),"Reload USER banks");if(r==8)gfxPrintf(10,y(r),"%03u",p.velocity);
 if(r==9)gfxPrint(10,y(r),p.profile?"Hardware import":"Calibrated import");}
int edit(int c,int r,CellEditAction a){if(r<3)return instrumentCommonOnEdit(c,r,a);
 if(r==3||r==4){instrumentSQ80LoadBanks();openUserPresetBrowser();return 1;}
 if(r==5){screenSetup(&screenSQ80Edit,cInstrument);return 1;}
 if(r==7){instrumentSQ80LoadBanks();screenMessage(MESSAGE_TIME, sq80Bank(current()->chip.sq80.bank)?"Wave bank ready":"Missing bank: import hardware-80.hwb");return 1;}
 auto&p=current()->chip.sq80;uint8_t* v=r==6?&p.bank:r==8?&p.velocity:r==9?&p.profile:nullptr;
 if(v&&edit8noLast(a,v,r==8?10:1,r==8?1:0,r==8?127:1)){projectModified=1;return 1;}return 0;
}
int input(int down,int keys,int){if(screenInstrumentSQ80.cursorRow!=4){held=0;return 0;}auto action=popupEditInput(down,keys,&held);
 if(action==PopupEditInput::cycle){cycleUserPreset(keys==(keyEdit|keyRight)?1:-1);return 1;}if(action==PopupEditInput::hold)return 1;
 if(action==PopupEditInput::open){instrumentSQ80LoadBanks();openUserPresetBrowser();return 1;}return 0;}
}
void instrumentSQ80LoadBanks(){
 std::string root="instruments/USER/sq80esq";
#ifdef WEB_BUILD
 root="/user/instruments/USER/sq80esq";
#else
 if(fileIsRunningFromAppImage()){char dir[1024];if(!fileGetDefaultDirectory(dir,sizeof(dir)))root=std::string(dir)+"/"+root;}
#endif
 std::string error;
 namespace fs=std::filesystem;
 std::error_code ec;unsigned visited=0;
 for(fs::directory_iterator it(root,ec),end;!ec&&it!=end&&visited++<64;it.increment(ec)){
  if(it->path().extension()!=".zip")continue;
  PresetZip zip;if(!zip.open(it->path().string(),error))continue;
  for(const auto& entry:zip.entries()){
   const auto leaf=fs::path(entry.name).filename().string();
   if(leaf!="hardware-80.hwb"&&leaf!="hardware-1.hwb")continue;
   auto dest=fs::path(root)/leaf;if(fs::exists(dest,ec))continue;
   std::vector<uint8_t> bytes;if(!zip.read(entry.name,bytes,error))continue;
   auto temp=dest.string()+".importing";{std::ofstream f(temp,std::ios::binary);f.write((const char*)bytes.data(),bytes.size());}
   hybrid::WaveBank checked;
   if(checked.load(temp,error)&&checked.romSize()==(leaf=="hardware-80.hwb"?262144u:65536u))fs::rename(temp,dest,ec);
   else fs::remove(temp,ec);
  }
 }
 sq80LoadBanks(root,error);
}
#ifdef WEB_BUILD
extern "C" EMSCRIPTEN_KEEPALIVE int webLoadSQ80Banks(){instrumentSQ80LoadBanks();return (sq80Bank(0)?1:0)|(sq80Bank(1)?2:0);}
#endif
ScreenData screenInstrumentSQ80={.rows=10,.cursorRow=0,.cursorCol=0,.topRow=0,.selectMode=-1,.selectStartRow=0,.selectStartCol=0,.selectAnchorRow=0,.selectAnchorCol=0,.playbackLevel=ScreenPlaybackLevel::none,.getColumnCount=columns,.drawStatic=drawStatic,.drawCursor=drawCursor,.drawSelection=nullptr,.drawRowHeader=nullptr,.drawColHeader=nullptr,.drawField=drawField,.onEdit=edit,.onInput=input,.onRawInput=nullptr,.isCellValid=nullptr,.getLoopRange=nullptr};
#endif
