#include "screen_instrument.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "user_preset_browser.h"
#include "selection_popup.h"
#include "corelib_file.h"
#include "corelib_gfx.h"
#include "project_utils.h"
#include "sq80_patch.h"
#include "external_presets.h"
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
void chooseBank(int n){if(n>=0&&n<=1){current()->chip.sq80.factory=n;projectModified=1;}screenSetup(&screenInstrument,cInstrument);}
void cancelBank(){screenSetup(&screenInstrument,cInstrument);}
void openBanks(){static const SelectionItem items[]={{"FACTORY",1,nullptr,0},{"USER",0,nullptr,0}};selectionPopupSetup("SQ80/ESQ BANK",items,2,current()->chip.sq80.factory,chooseBank,cancelBank);screenSetup(&screenSelectionPopup,0);}
int columns(int r){return r<3?instrumentCommonColumnCount(r):(r==3||r==7?2:1);}
int y(int r){return r+3;}
int fieldFX(int c,int r){return r==3?(c?fxVEL:fxWHL):r==4?fxPED:r==7?(c?fxQRS:fxQCF):-1;}
void drawStatic(){instrumentCommonDrawStatic();gfxSetFgColor(appSettings.colorScheme.textDefault);
 gfxPrint(0,6,"Wheel");gfxPrint(19,6,"Velocity");gfxPrint(0,7,"Pedal");gfxPrint(0,8,"Bank");gfxPrint(0,9,"Preset");gfxPrint(0,10,"Cutoff");gfxPrint(19,10,"Resonance");gfxPrint(0,11,"Editor");
 gfxSetFgColor(appSettings.colorScheme.textInfo);gfxPrint(0,14,"* = playback override");gfxPrint(0,15,"EDIT+PLAY: audition");}
void drawCursor(int c,int r){if(r<3)instrumentCommonDrawCursor(c,r);else gfxCursor(c?30:9,y(r),r==6||r==8?24:4);}
void drawField(int c,int r,CellState s){if(r<3){instrumentCommonDrawField(c,r,s);return;}
 const auto&p=current()->chip.sq80;int x=c?30:9;gfxClearRect(x,y(r),r==6||r==8?28:9,1);gfxSetFgColor(s==CellState::focus?appSettings.colorScheme.textValue:appSettings.colorScheme.textDefault);
 const char* star=instrumentSQ80Override(fieldFX(c,r))?"*":"";
 if(r==3)gfxPrintf(x,y(r),"%03u%s",c?p.velocity:p.wheel,star);
 if(r==4)gfxPrintf(x,y(r),"%03u%s",p.pedal,star);
 if(r==5)gfxPrint(x,y(r),p.factory?"FACTORY":"USER");
 if(r==6)gfxPrintf(x,y(r),"%.24s",current()->name);
 if(r==7)gfxPrintf(x,y(r),"%03u%s",p.raw[c?90:89]&(c?31:127),star);
 if(r==8)gfxPrint(x,y(r),"EDIT PRESET >");
}
int edit(int c,int r,CellEditAction a){if(r<3)return instrumentCommonOnEdit(c,r,a);
 if(r==5&&convertMultiAction(a)==CellEditAction::tap){openBanks();return 1;}
 if(r==6){instrumentSQ80LoadBanks();openUserPresetBrowser();return 1;}
 if(r==8){screenSetup(&screenSQ80Edit,cInstrument);return 1;}
 auto&p=current()->chip.sq80;
 if(r==7){uint8_t value=p.raw[c?90:89]&(c?31:127);if(edit8noLast(a,&value,10,0,c?31:127)){p.raw[c?90:89]=(p.raw[c?90:89]&~(c?31:127))|value;projectModified=1;return 1;}return 0;}
 uint8_t*v=r==3?(c?&p.velocity:&p.wheel):r==4?&p.pedal:r==5?&p.factory:nullptr;
 if(v&&edit8noLast(a,v,r==5?1:10,r==3&&c?1:0,r==5?1:127)){projectModified=1;return 1;}return 0;
}
int input(int down,int keys,int){if(screenInstrumentSQ80.cursorRow!=6){held=0;return 0;}auto action=popupEditInput(down,keys,&held);
 if(action==PopupEditInput::cycle){cycleUserPreset(keys==(keyEdit|keyRight)?1:-1);return 1;}if(action==PopupEditInput::hold)return 1;
 if(action==PopupEditInput::open){instrumentSQ80LoadBanks();openUserPresetBrowser();return 1;}return 0;}
}
bool instrumentSQ80Override(int fx){
 if(fx<0)return false;const auto*s=chipnomadGetPlaybackStatus(chipnomadState);if(!s||!s->isPlaying)return false;
 for(int t=0;t<chipnomadState->project.tracksCount;++t){const auto&tr=s->tracks[t];
  if(s->trackEnabled[t]&&tr.mode!=PlaybackMode::stopped&&tr.mode!=PlaybackMode::none&&tr.note.instrument==cInstrument&&!tr.note.noteKilled&&tr.note.fx[fx].isOn)return true;
 }return false;
}
namespace {
std::vector<ExternalPreset> imported;std::vector<SelectionItem> importChoices;
void cancelImport(){chipnomadQueueSQ80Preview(chipnomadState,*pSongTrack,nullptr);screenSetup(&screenInstrument,cInstrument);}
void selectImport(int n){cancelImport();if(n<0||size_t(n)>=imported.size())return;auto before=current()->chip.sq80;instrumentClear(current());*current()=imported[n].instrument;current()->chip.sq80.velocity=before.velocity;current()->chip.sq80.factory=before.factory;projectModified=1;}
void previewImport(int n,bool held){if(held&&n>=0&&size_t(n)<imported.size()){auto p=imported[n].instrument.chip.sq80;p.velocity=current()->chip.sq80.velocity;chipnomadQueueSQ80Preview(chipnomadState,*pSongTrack,&p);}else chipnomadQueueSQ80Preview(chipnomadState,*pSongTrack,nullptr);}
}
void instrumentSQ80Import(const char* path){
 std::vector<uint8_t> bytes;std::string error;instrumentSQ80LoadBanks();
 if(!readPresetFile(path,bytes,error)||!importExternalPresets(InstrumentType::SQ80ESQ,path,bytes,imported,error)){screenSetup(&screenInstrument,cInstrument);screenMessage(MESSAGE_TIME_ERROR,"%s",error.c_str());return;}
 importChoices.clear();for(size_t n=0;n<imported.size();++n)importChoices.push_back({imported[n].name.c_str(),int(n),nullptr,0});
 selectionPopupSetup("IMPORT SQ80/ESQ",importChoices.data(),int(importChoices.size()),0,selectImport,cancelImport,true,previewImport);screenSetup(&screenSelectionPopup,0);
 if(!error.empty())screenMessage(MESSAGE_TIME_ERROR,"%s",error.c_str());
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
extern "C" EMSCRIPTEN_KEEPALIVE int webValidateSQ80Bank(const char* path,int model){hybrid::WaveBank bank;std::string error;return path&&bank.load(path,error)&&bank.romSize()==(model?65536u:262144u);}
extern "C" EMSCRIPTEN_KEEPALIVE int webLoadSQ80Banks(){instrumentSQ80LoadBanks();return (sq80Bank(0)?1:0)|(sq80Bank(1)?2:0);}
#endif
ScreenData screenInstrumentSQ80={.rows=9,.cursorRow=0,.cursorCol=0,.topRow=0,.selectMode=-1,.selectStartRow=0,.selectStartCol=0,.selectAnchorRow=0,.selectAnchorCol=0,.playbackLevel=ScreenPlaybackLevel::none,.getColumnCount=columns,.drawStatic=drawStatic,.drawCursor=drawCursor,.drawSelection=nullptr,.drawRowHeader=nullptr,.drawColHeader=nullptr,.drawField=drawField,.onEdit=edit,.onInput=input,.onRawInput=nullptr,.isCellValid=nullptr,.getLoopRange=nullptr};
#endif
