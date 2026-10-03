#include "../../platforms/vita/controls.h"
#include <cassert>
#include <cstring>
#include <cstdio>

static KeyMapping previousDefaults() {
  KeyMapping keys{};
  InputCode* rows[]={keys.keyUp,keys.keyDown,keys.keyLeft,keys.keyRight,keys.keyEdit,
    keys.keyOpt,keys.keyPlay,keys.keyShift,keys.keyMotionLive,keys.keyMotionRecord,keys.keyMotionErase};
  const int buttons[]={11,12,13,14,0,1,6,4,9,2,3}; // saved original Vita profile
  for(unsigned i=0;i<11;++i) rows[i][1]={InputDeviceType::gamepad,buttons[i]};
  keys.keyEdit[0]={InputDeviceType::keyboard,120};
  return keys;
}
int main() {
  auto keys=previousDefaults();
  assert(vitaMigrateDefaultControls(keys));
  assert(keys.keyPlay[1].code==SDL_CONTROLLER_BUTTON_Y);
  assert(keys.keyEdit[1].code==SDL_CONTROLLER_BUTTON_B);
  assert(keys.keyOpt[1].code==SDL_CONTROLLER_BUTTON_A);
  assert(keys.keyShift[1].code==SDL_CONTROLLER_BUTTON_X);
  assert(keys.keyMotionLive[1].code==SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
  assert(keys.keyMotionRecord[1].code==SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
  assert(keys.keyMotionErase[1].code==SDL_CONTROLLER_BUTTON_BACK);
  assert(keys.keyPlay[2].code==SDL_CONTROLLER_BUTTON_START);
  assert(keys.keyEdit[0].code==120);
  assert(!vitaMigrateDefaultControls(keys)); // migration is idempotent
  auto custom=previousDefaults();
  custom.keyEdit[1].code=SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
  auto saved=custom;
  assert(!vitaMigrateDefaultControls(custom));
  assert(std::memcmp(&saved,&custom,sizeof(custom))==0);
  custom=previousDefaults(); custom.keyOpt[2]={InputDeviceType::gamepad,7}; saved=custom;
  assert(!vitaMigrateDefaultControls(custom));
  assert(std::memcmp(&saved,&custom,sizeof(custom))==0);
  puts("Vita controls: NESW, motion actions, migration and custom-map preservation passed");
}
