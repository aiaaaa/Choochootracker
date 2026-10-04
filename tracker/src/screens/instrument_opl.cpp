#include "screen_instrument.h"
#include "selection_popup.h"
#include "corelib_gfx.h"
#include "corelib_file.h"
#include "chipnomad_lib.h"
#include "opl_patch.h"
#include "four_op_patch.h"
#include "dx7_patch.h"
#include "fm_catalog.h"
#include "utils.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace {
using Entry=FMPresetEntry;
std::vector<Entry> catalog;
std::vector<InstrumentDX7> importedDX7;
std::vector<SelectionItem> bankItems,categoryItems;
std::vector<std::vector<SelectionItem>> sounds;
std::vector<std::string> categoryNames;
std::string folder;
int bankFilter=0,buttonDown=0;
bool importing=false;
Instrument* current(){return &chipnomadState->project.instruments[cInstrument];}
bool dx7(){return current()->type==InstrumentType::DX7;}
bool fourOp(){return isFourOp(current()->type);}
int bankId(){return fourOp()?current()->chip.fourOp.bankId:dx7()?current()->chip.dx7.bankId:current()->chip.opl.bankId;}
const char* presetName(){return fourOp()?current()->chip.fourOp.presetName:dx7()?current()->chip.dx7.presetName:current()->chip.opl.presetName;}
int8_t& fineTune(){return fourOp()?current()->chip.fourOp.fineTune:dx7()?current()->chip.dx7.fineTune:current()->chip.opl.fineTune;}
bool compatible(const Entry& e){if(importing)return e.bank==bankFilter;return e.type==int(current()->type)||(current()->type==InstrumentType::OPL3&&e.type==int(InstrumentType::OPL2));}
bool readCatalog(){
  if(!catalog.empty())return true;
  folder="instruments/chips/";
  if(fileIsRunningFromAppImage()){char root[1024];if(fileGetDefaultDirectory(root,sizeof(root)))return false;folder=std::string(root)+"/instruments/chips/";}
  return loadFMCatalog((folder+"catalog.tsv").c_str(),catalog);
}
bool candidate(int index,Instrument& result){
  if(index<0||index>=int(catalog.size())||!compatible(catalog[index]))return false;
  if(catalog[index].imported>=0) {
    getInstrumentFunctions(InstrumentType::DX7).init(&result);result.chip.dx7=importedDX7[catalog[index].imported];
    strncpy(result.name,result.chip.dx7.presetName,PROJECT_INSTRUMENT_NAME_LENGTH);return true;
  }
  auto p=std::make_unique<Project>();projectInit(p.get());
  if(instrumentLoad(p.get(),(folder+catalog[index].path).c_str(),0)){projectFree(p.get());return false;}
  bool ok=isFourOp(p->instruments[0].type)?validFourOp(p->instruments[0].type,p->instruments[0].chip.fourOp):p->instruments[0].type==InstrumentType::DX7?validDX7(p->instruments[0].chip.dx7):isOPL(p->instruments[0].type)&&validOPL(p->instruments[0].type,p->instruments[0].chip.opl);
  if(ok){result=p->instruments[0];result.type=current()->type;}
  projectFree(p.get());return ok;
}
void stopPreview(){if(fourOp()&&!importing){chipnomadQueueFourOpPreview(chipnomadState,*pSongTrack,current()->type,nullptr);return;}if(dx7()||importing){chipnomadQueueDX7Preview(chipnomadState,*pSongTrack,nullptr);return;}chipnomadQueueOPLPreview(chipnomadState,*pSongTrack,current()->type,nullptr);}
void preview(int index,bool held){
  Instrument patch{};
  if(held&&candidate(index,patch)){if(isFourOp(patch.type))chipnomadQueueFourOpPreview(chipnomadState,*pSongTrack,patch.type,&patch.chip.fourOp);else if(patch.type==InstrumentType::DX7)chipnomadQueueDX7Preview(chipnomadState,*pSongTrack,&patch.chip.dx7);else chipnomadQueueOPLPreview(chipnomadState,*pSongTrack,patch.type,&patch.chip.opl);}
  else stopPreview();
}
void cancel(){stopPreview();importing=false;screenSetup(&screenInstrument,cInstrument);}
void select(int index){
  stopPreview();Instrument patch{};
  if(candidate(index,patch)){instrumentClear(current());*current()=patch;projectModified=1;}
  else screenMessage(MESSAGE_TIME_ERROR,"Preset could not load");
  importing=false;screenSetup(&screenInstrument,cInstrument);
}
int selected(){for(size_t i=0;i<catalog.size();++i){const auto& e=catalog[i];if(e.bank==bankId()&&e.name==presetName()&&compatible(e))return i;}return -1;}
void selectBank(int value){bankFilter=value;screenSetup(&screenInstrument,cInstrument);}
void openBanks(){
  if(!readCatalog()){screenMessage(MESSAGE_TIME_ERROR,"Factory catalog missing");return;}
  bankItems.clear();bankItems.push_back({"All banks",0,nullptr,0});
  for(const auto& e:catalog)if(compatible(e)&&std::none_of(bankItems.begin(),bankItems.end(),[&](const auto& b){return b.value==e.bank;}))bankItems.push_back({e.bankName.c_str(),e.bank,nullptr,0});
  selectionPopupSetup("FM BANK",bankItems.data(),bankItems.size(),bankFilter,selectBank,cancel,true);screenSetup(&screenSelectionPopup,0);
}
void openSounds(){
  if(!readCatalog()){screenMessage(MESSAGE_TIME_ERROR,"Factory catalog missing");return;}
  if(!importing&&bankFilter&&std::none_of(catalog.begin(),catalog.end(),[](const auto& e){return compatible(e)&&e.bank==bankFilter;}))bankFilter=0;
  categoryNames={"All"};
  for(const auto& e:catalog)if(compatible(e)&&(!bankFilter||e.bank==bankFilter)&&std::find(categoryNames.begin(),categoryNames.end(),e.category)==categoryNames.end())categoryNames.push_back(e.category);
  sounds.clear();sounds.resize(categoryNames.size());categoryItems.clear();
  for(size_t i=0;i<catalog.size();++i){const auto& e=catalog[i];if(!compatible(e)||(bankFilter&&e.bank!=bankFilter))continue;
    for(size_t c=0;c<categoryNames.size();++c)if(c==0||categoryNames[c]==e.category)sounds[c].push_back({e.name.c_str(),int(i),nullptr,0,e.name.c_str()});}
  for(size_t c=0;c<categoryNames.size();++c)categoryItems.push_back({categoryNames[c].c_str(),-1,sounds[c].data(),int(sounds[c].size())});
  selectionPopupSetup("FM PRESETS",categoryItems.data(),categoryItems.size(),selected(),select,cancel,false,preview);screenSetup(&screenSelectionPopup,0);
}
int columns(int row){return row<3?instrumentCommonColumnCount(row):1;}
void drawStatic(){instrumentCommonDrawStatic();gfxSetFgColor(appSettings.colorScheme.textDefault);gfxPrint(0,6,"Bank");gfxPrint(0,7,"Preset");gfxPrint(0,9,"Fine ct");gfxPrint(0,11,"Mode");const char* mode=fourOp()?"4 operator":dx7()?"6 operator":current()->chip.opl.topology==OPLTopology::fourOperator?"4 operator":current()->chip.opl.topology==OPLTopology::dualVoice?"Dual voice":"2 operator";gfxPrint(9,11,mode);}
void drawCursor(int col,int row){if(row<3)instrumentCommonDrawCursor(col,row);else gfxCursor(9,row==3?6:row==4?7:9,row==5?4:28);}
void drawField(int col,int row,CellState state){
  if(row<3){instrumentCommonDrawField(col,row,state);return;}
  gfxSetFgColor(state==CellState::focus?appSettings.colorScheme.textValue:appSettings.colorScheme.textDefault);int y=row==3?6:row==4?7:9;gfxClearRect(9,y,30,1);
  if(row==3){const char* name="All banks";for(const auto& e:catalog)if(e.bank==bankFilter){name=e.bankName.c_str();break;}gfxPrintf(9,y,"%.30s",name);}
  else if(row==4)gfxPrintf(9,y,"%.30s",presetName());else gfxPrintf(9,y,"%+04d",fineTune());
}
int onEdit(int col,int row,CellEditAction action){
  if(row<3)return instrumentCommonOnEdit(col,row,action);
  if(row==3){openBanks();return 1;}if(row==4){openSounds();return 1;}
  action=convertMultiAction(action);int v=fineTune();
  if(action==CellEditAction::clear)v=0;else if(action==CellEditAction::increase)++v;else if(action==CellEditAction::decrease)--v;else if(action==CellEditAction::increaseBig)v+=10;else if(action==CellEditAction::decreaseBig)v-=10;else return 0;
  fineTune()=std::clamp(v,-100,100);projectModified=1;return 1;
}
int onInput(int down,int keys,int){
  int row=screenInstrumentOPL.cursorRow;if(row!=3&&row!=4){buttonDown=0;return 0;}
  PopupEditInput input=popupEditInput(down,keys,&buttonDown);
  if(input==PopupEditInput::cycle){
    if(row==4&&readCatalog()){
      int index=selected(),direction=keys==(keyEdit|keyRight)?1:-1;
      for(size_t tries=0;tries<catalog.size();++tries){index=(index+direction+catalog.size())%catalog.size();if(compatible(catalog[index])&&(!bankFilter||catalog[index].bank==bankFilter)){select(index);break;}}
    }return 1;
  }
  if(input==PopupEditInput::hold)return 1;
  if(input==PopupEditInput::open){if(row==3)openBanks();else openSounds();return 1;}return 0;
}
}
ScreenData screenInstrumentOPL={
 .rows=6,.cursorRow=0,.cursorCol=0,.topRow=0,.selectMode=-1,.selectStartRow=0,.selectStartCol=0,.selectAnchorRow=0,.selectAnchorCol=0,
 .playbackLevel=ScreenPlaybackLevel::none,.getColumnCount=columns,.drawStatic=drawStatic,.drawCursor=drawCursor,.drawSelection=nullptr,.drawRowHeader=nullptr,.drawColHeader=nullptr,
 .drawField=drawField,.onEdit=onEdit,.onInput=onInput,.onRawInput=nullptr,.isCellValid=nullptr,.getLoopRange=nullptr,
};

void instrumentFMImportSysEx(const char* path) {
  FILE* f=fopen(path,"rb");
  if(!f){screenMessage(MESSAGE_TIME_ERROR,"Could not open SysEx");screenSetup(&screenInstrument,cInstrument);return;}
  std::vector<uint8_t> bytes(1024*1024+1);size_t size=fread(bytes.data(),1,bytes.size(),f);bool ioError=ferror(f);fclose(f);
  std::vector<InstrumentDX7> patches;std::string error;
  if(ioError||!importDX7SysEx(bytes.data(),size,patches,error)) {
    screenMessage(MESSAGE_TIME_ERROR,"%s",ioError?"SysEx read failed":error.c_str());screenSetup(&screenInstrument,cInstrument);return;
  }
  readCatalog();
  // Metadata browsing and patch materialization happen on the UI thread only.
  if(catalog.size()+patches.size()>65536){screenMessage(MESSAGE_TIME_ERROR,"FM catalog limit reached");screenSetup(&screenInstrument,cInstrument);return;}
  int bank=32768;for(const auto& e:catalog)bank=std::max(bank,e.bank+1);
  if(bank>65535){screenMessage(MESSAGE_TIME_ERROR,"FM bank limit reached");return;}
  const char* name=strrchr(path,PATH_SEPARATOR);name=name?name+1:path;
  for(auto& p:patches){p.bankId=bank;int index=importedDX7.size();importedDX7.push_back(p);catalog.push_back({int(InstrumentType::DX7),bank,name,"Unsorted",p.presetName,"",index});}
  // Opening a bank is transactional even when the current instrument isn't DX7.
  // Compatible imported entries are offered; selection performs the type change.
  bankFilter=bank;importing=true;openSounds();
}
