#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "corelib_gfx.h"
#include "corelib_font.h"
#include "corelib_mainloop.h"
#include "app.h"
#include "common.h"

#ifdef VITA_BUILD
#include "../vita/platform.h"
#include "../vita/controls.h"
#endif

int main(int argv, char** args) {
#ifdef VITA_BUILD
  if (!vitaPlatformInit()) return 1;
#endif
  settingsLoad();
#ifdef VITA_BUILD
  if (vitaMigrateDefaultControls(appSettings.keyMapping)) {
    fprintf(stderr, "Vita controls: migrated original defaults to R36H NESW\n");
    settingsSave();
  }
#endif

  // Load custom font before gfxSetup so it uses the correct font
  if (appSettings.fontPath[0] != '\0') {
    Font* font = fontLoad(appSettings.fontPath);
    if (font) {
      fontSetCurrent(font);
    } else {
      appSettings.fontPath[0] = '\0';
      fontSetCurrent(NULL);
    }
  }

  if (gfxSetup(&appSettings.screenWidth, &appSettings.screenHeight) != 0) return 1;

#ifdef VITA_BUILD
  if (!vitaPlatformPrepareAssets(appSettings.colorScheme.textDefault, appSettings.colorScheme.background)) {
    gfxCleanup();
    vitaPlatformQuit();
    return 1;
  }
#endif
  appSetup();
  mainLoopRun(appDraw, appOnEvent);
#ifndef WEB_BUILD
  appCleanup();
  gfxCleanup();
  mainLoopQuit();
#endif

  #ifdef VITA_BUILD
  vitaPlatformQuit();
  #endif
  return 0;
}

extern "C" int SDL_main(int argv, char** args) {
  return main(argv, args);
}
