#include "screen_instrument.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "sq80_parameters.h"
#include "sq80_patch.h"
#include "project_utils.h"
#include "corelib_gfx.h"
#include <algorithm>
#include <cstring>
namespace {
ScreenData data;char hintText[128]{};
Instrument& current(){return chipnomadState->project.instruments[cInstrument];}
constexpr int headerRows=4;
const SQ80Parameter* field(int row){row-=headerRows;const auto& f=sq80Parameters();return row>=0&&size_t(row)<f.size()?&f[row]:nullptr;}
const char* overrideReason(const SQ80Parameter& f){
 int fx=f.byte==89&&f.mask==127?fxQCF:f.byte==90&&f.mask==31?fxQRS:f.byte==46&&f.mask==63?fxLFR:f.byte==95&&f.mask==63?fxGLI:-1;
 if(instrumentSQ80Override(fx))return "* Roll FX overrides this parameter";
 const auto* s=chipnomadGetPlaybackStatus(chipnomadState);if(!s||!s->isPlaying)return nullptr;
 for(int t=0;t<chipnomadState->project.tracksCount;++t){const auto& tr=s->tracks[t];
  if(!s->trackEnabled[t]||tr.mode==PlaybackMode::stopped||tr.mode==PlaybackMode::none||tr.note.instrument!=cInstrument||tr.note.noteKilled)continue;
  bool volume=f.byte==88&&f.mask==126;
  bool pitch=f.byte==58||f.byte==68||f.byte==78||f.byte==59||f.byte==69||f.byte==79;
  if(volume&&tr.note.fx[fxVOL].isOn)return "* Track volume FX shapes playback";
  if(f.byte==100&&f.mask==240&&tr.note.fx[fxPAN].isOn)return "* Track pan FX shapes playback";
  if(pitch&&(tr.note.fineOffset||tr.note.fx[fxPIT].isOn||tr.note.fx[fxPBN].isOn||tr.note.fx[fxPVB].isOn||tr.note.fx[fxPSL].isOn))return "* Track pitch offset shapes playback";
  for(const auto&m:tr.note.modulation)if(m.modulation&&((volume&&m.modulation->destination==1)||(pitch&&m.modulation->destination==2)))return "* Track modulation shapes playback";
 }return nullptr;
}
int columns(int){return 1;}
void drawStatic(){gfxSetFgColor(appSettings.colorScheme.textTitles);gfxPrintf(0,0,"SQ80/ESQ EDIT %02X",cInstrument);gfxSetFgColor(appSettings.colorScheme.textInfo);gfxPrint(0,1,current().name);gfxPrint(0,15,"* = playback override");gfxPrint(0,16,"Patch: next note; Wheel/Pedal: live");}
void drawField(int,int row,CellState s){auto*f=field(row);if(row<data.topRow||row>=data.topRow+screenVisibleRows())return;
 if(row<headerRows){int y=3+row-data.topRow;if(y>=15)return;auto&p=current().chip.sq80;gfxClearRect(0,y,39,1);gfxSetFgColor(appSettings.colorScheme.textInfo);const char*labels[]={"Wheel","Pedal","Waves","Find wave banks"};gfxPrint(0,y,labels[row]);gfxSetFgColor(s==CellState::focus?appSettings.colorScheme.textValue:appSettings.colorScheme.textDefault);
 if(row<2)gfxPrintf(23,y,"%4u%s",row?p.pedal:p.wheel,instrumentSQ80Override(row?fxPED:fxWHL)?"*":"");
 else if(row==2)gfxPrintf(23,y,"%s %s",p.bank?"ESQ1":"SQ80",sq80Bank(p.bank)?"ready":"MISSING");else gfxPrint(23,y,"LOAD >");return;}
 if(!f)return;
 int y=3+row-data.topRow;if(y>=15)return;gfxClearRect(0,y,39,1);gfxSetFgColor(appSettings.colorScheme.textInfo);gfxPrintf(0,y,"%-6s %-14s",f->group,f->label);
 gfxSetFgColor(s==CellState::focus?appSettings.colorScheme.textValue:appSettings.colorScheme.textDefault);gfxPrintf(23,y,"%4d%s",sq80Get(current().chip.sq80,*f),overrideReason(*f)?"*":"");}
void drawCursor(int,int row){if(row>=data.topRow&&row-data.topRow<12)gfxCursor(23,3+row-data.topRow,4);}
void drawHeader(int,CellState){}
int edit(int,int row,CellEditAction action){if(row<headerRows){auto&p=current().chip.sq80;if(row==3){instrumentSQ80LoadBanks();screenMessage(MESSAGE_TIME,sq80Bank(p.bank)?"Wave bank ready":"Import wave banks first");return 1;}
 uint8_t*v=row==0?&p.wheel:row==1?&p.pedal:&p.bank;if(edit8noLast(action,v,row<2?10:1,0,row<2?127:1)){projectModified=1;return 1;}return 0;}
 auto*f=field(row);if(!f)return 0;action=convertMultiAction(action);int v=sq80Get(current().chip.sq80,*f);
 switch(action){case CellEditAction::increase:++v;break;case CellEditAction::decrease:--v;break;case CellEditAction::increaseBig:v+=f->maximum>15?10:1;break;case CellEditAction::decreaseBig:v-=f->maximum>15?10:1;break;case CellEditAction::clear:v=std::max(0,f->minimum);break;default:return 0;}
 auto before=current().chip.sq80;sq80Set(current().chip.sq80,*f,v);if(memcmp(&before,&current().chip.sq80,sizeof(before)))projectModified=1;return 1;}
void setup(int){data.cursorRow=data.cursorCol=data.topRow=0;hintText[0]=0;}
void full(){screenFullRedraw(&data);}
void draw(){for(int row=data.topRow;row<std::min(data.rows,data.topRow+12);++row)drawField(0,row,row==data.cursorRow?CellState::focus:CellState::normal);drawCursor(0,data.cursorRow);
 const char*active=screenGetActiveMessage();
 if(!*active||!strcmp(active,hintText)){
  const char*hint=nullptr;
  if(data.cursorRow<headerRows){const char*hints[]={"Wheel: preset WHEEL routing, 0 neutral","Pedal: continuous PEDAL modulation","External wave bank; imported separately","Find missing banks; restart to replace"};hint=hints[data.cursorRow];}
  else if(auto*f=field(data.cursorRow)){hint=overrideReason(*f);if(!hint)hint=f->hint;}
  if(hint){screenMessage(0,"%s",hint);snprintf(hintText,sizeof(hintText),"%s",screenGetActiveMessage());}
 }
}
int input(int down,int keys,int taps){if(!keys)chipnomadQueuePlaybackStopPreview(chipnomadState,*pSongTrack);
 if(keys==keyOpt||keys==(keyLeft|keyShift)){chipnomadQueuePlaybackStopPreview(chipnomadState,*pSongTrack);screenSetup(&screenInstrument,cInstrument);return 1;}
 if(keys==(keyEdit|keyPlay)){if(down&&!chipnomadGetPlaybackStatus(chipnomadState)->isPlaying)chipnomadQueuePlaybackPreviewNote(chipnomadState,*pSongTrack,instrumentFirstNote(&chipnomadState->project,cInstrument),cInstrument);return 1;}
 return screenInput(&data,down,keys,taps);}
ScreenPlaybackLevel level(){return ScreenPlaybackLevel::phrase;}
struct Init{Init(){data={};data.rows=headerRows+sq80Parameters().size();data.selectMode=-1;data.getColumnCount=columns;data.drawStatic=drawStatic;data.drawField=drawField;data.drawCursor=drawCursor;data.drawRowHeader=drawHeader;data.drawColHeader=drawHeader;data.onEdit=edit;}}init;
}
const AppScreen screenSQ80Edit={nullptr,setup,full,draw,input,level};
#endif
