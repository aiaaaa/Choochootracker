#include "platform.h"
#include "chipnomad_lib.h"
#include "chipnomad_lib_live_stick.h"
#include "touch_modulation.h"
#include <SDL2/SDL.h>

static TouchModInput touch;
static bool foreground=true;
// SDL 2.32.8 Vita: Front=1, Back=2, active-area coordinates. The filter runs
// before SDL's renderer can transform coordinates into the letterboxed viewport.
// Vita has controller-only UI: even unarmed touches never enter UI/mouse paths.
static int filter(void*, SDL_Event* e) {
  if(e->type==SDL_APP_WILLENTERBACKGROUND || (e->type==SDL_WINDOWEVENT && e->window.event==SDL_WINDOWEVENT_FOCUS_LOST)) {
    foreground=false; touchModReset();
  }
  if(e->type==SDL_APP_DIDENTERFOREGROUND || (e->type==SDL_WINDOWEVENT && e->window.event==SDL_WINDOWEVENT_FOCUS_GAINED)) {
    foreground=true; touchModReset();
  }
  if(e->type==SDL_FINGERDOWN || e->type==SDL_FINGERMOTION || e->type==SDL_FINGERUP) {
    auto& f=e->tfinger;
    if(f.touchId==1 || f.touchId==2)
      touch.event(unsigned(f.touchId-1), f.fingerId,
        e->type==SDL_FINGERDOWN ? TouchModInput::Event::down :
        e->type==SDL_FINGERUP ? TouchModInput::Event::up : TouchModInput::Event::motion,
        f.x, f.y, foreground && chipnomadLiveStickIsEnabled());
    return 0;
  }
  if((e->type==SDL_MOUSEBUTTONDOWN || e->type==SDL_MOUSEBUTTONUP) && e->button.which==SDL_TOUCH_MOUSEID) return 0;
  if(e->type==SDL_MOUSEMOTION && e->motion.which==SDL_TOUCH_MOUSEID) return 0;
  return 1;
}
void vitaInputInit() { touchModReset(); SDL_SetEventFilter(filter,nullptr); }
void vitaInputReset() { touchModReset(); chipnomadSetLiveStickAxes(0,0,0,0); }
