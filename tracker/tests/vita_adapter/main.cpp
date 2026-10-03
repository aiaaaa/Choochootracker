#include <SDL2/SDL.h>
#include "touch_modulation.h"
#include "../../platforms/vita/platform.h"
#include <cassert>
#include <cstdio>

static SDL_EventFilter installed;
static int enabled=1;
void SDL_SetEventFilter(SDL_EventFilter filter,void*) { installed=filter; }
int chipnomadLiveStickIsEnabled() { return enabled; }
void chipnomadSetLiveStickAxes(float a,float b,float c,float d) { assert(a==0 && b==0 && c==0 && d==0); }
static int touch(uint32_t event,int panel,int id,float x=1,float y=0) {
  SDL_Event e; e.type=event; e.tfinger.touchId=panel; e.tfinger.fingerId=id; e.tfinger.x=x; e.tfinger.y=y;
  return installed(nullptr,&e);
}
int main() {
  vitaInputInit(); assert(installed);
  assert(touch(SDL_FINGERDOWN,1,3)==0); assert(touch(SDL_FINGERDOWN,2,3,0,1)==0);
  assert(touchModOutput(0,0,127)==32385); assert(touchModOutput(1,0,127)==-32385);
  assert(touch(SDL_FINGERMOTION,1,3,0.5f,0.5f)==0); assert(touchModOutput(0,0,127)==0);
  SDL_Event event; event.type=SDL_MOUSEBUTTONDOWN; event.button.which=SDL_TOUCH_MOUSEID;
  assert(installed(nullptr,&event)==0);
  event.type=SDL_MOUSEMOTION; event.motion.which=SDL_TOUCH_MOUSEID; assert(installed(nullptr,&event)==0);
  event.type=0x650; assert(installed(nullptr,&event)==1); // Controller events remain usable.
  event.type=SDL_WINDOWEVENT; event.window.event=SDL_WINDOWEVENT_FOCUS_LOST;
  assert(installed(nullptr,&event)==1); assert(touchModOutput(1,2,127)==0);
  touch(SDL_FINGERUP,1,3); touch(SDL_FINGERUP,2,3);
  touch(SDL_FINGERDOWN,1,4); assert(touchModOutput(0,2,127)==0);
  event.type=SDL_APP_DIDENTERFOREGROUND; installed(nullptr,&event);
  touch(SDL_FINGERMOTION,1,4); assert(touchModOutput(0,2,127)==0); // No mid-gesture ownership.
  touch(SDL_FINGERUP,1,4); touch(SDL_FINGERDOWN,1,5); assert(touchModOutput(0,2,127)==32385);
  event.type=SDL_APP_WILLENTERBACKGROUND; installed(nullptr,&event);
  assert(touchModOutput(0,2,127)==0);
  event.type=SDL_APP_DIDENTERFOREGROUND; installed(nullptr,&event);
  touch(SDL_FINGERUP,1,5); touch(SDL_FINGERDOWN,2,6); assert(touchModOutput(1,2,127)==32385);
  vitaInputReset(); assert(touchModOutput(1,2,127)==0);
  touch(SDL_FINGERMOTION,2,6); assert(touchModOutput(1,2,127)==0);
  assert(touch(SDL_FINGERDOWN,77,8)==0); // Unknown panels still cannot reach the UI.
  puts("Vita SDL adapter: separation, ownership, UI suppression and lifecycle passed");
}
