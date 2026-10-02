#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "corelib/corelib_mainloop.h"
#include "corelib/corelib_input.h"

void appSetup(void);
void appCleanup(void);
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
void appModLuckyImported(void);
#endif
void appDraw(void);
void appSetStickLiveMode(StickLiveMode mode);
void appOnEvent(MainLoopEventData eventData);

// Raw input callback for key mapping screen
extern void (*inputRawCallback)(InputCode input, int isDown);


#ifdef __cplusplus
}
#endif

#endif
