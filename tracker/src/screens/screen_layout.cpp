#include "screen_layout.h"
#include "screens.h"
#include "corelib_gfx.h"

int screenScopeRows(const AppScreen* screen) {
  return screen == &screenSong || screen == &screenChain ||
    screen == &screenPhrase || screen == &screenTable ||
    screen == &screenInstrument || screen == &screenInstrumentPool ||
    screen == &screenModulation || screen == &screenMixer ||
    screen == &screenGroove ? 2 : 0;
}

int screenVisibleRows(void) {
  // Modulation has two compact blocks of eight logical fields, not a list.
  return currentScreen == &screenModulation ? 16 : 16 - screenScopeRows(currentScreen);
}

ScreenOverlayCoordinates::ScreenOverlayCoordinates()
  : previous_(gfxGetContentRowOffset()) { gfxSetContentRowOffset(0); }
ScreenOverlayCoordinates::~ScreenOverlayCoordinates() { gfxSetContentRowOffset(previous_); }
