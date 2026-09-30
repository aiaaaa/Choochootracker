#include "doctest.h"
#include "screens.h"
#include "corelib_gfx.h"
#include <initializer_list>
TEST_CASE("Scope reserves rows only on the intended screens") {
  const AppScreen* saved = currentScreen;
  for (const AppScreen* screen : {&screenSong, &screenChain, &screenPhrase, &screenTable,
       &screenInstrument, &screenInstrumentPool, &screenMixer, &screenGroove}) {
    currentScreen = screen;
    CHECK(screenScopeRows(screen) == 2);
    CHECK(screenVisibleRows() == 14);
  }
  for (const AppScreen* screen : {&screenSettings, &screenProject, &screenAYWavetable, &screenTitle})
    CHECK(screenScopeRows(screen) == 0);
  currentScreen = &screenModulation;
  CHECK(screenVisibleRows() == 16);
  currentScreen = saved;
  gfxSetContentRowOffset(2);
  { ScreenOverlayCoordinates overlay; CHECK(gfxGetContentRowOffset() == 0); }
  CHECK(gfxGetContentRowOffset() == 2);
  gfxSetContentRowOffset(0);
}
