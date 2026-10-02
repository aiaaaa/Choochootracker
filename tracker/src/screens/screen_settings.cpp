#include "screen_settings.h"
#include "screen_keymapping.h"
#include "screen_midi.h"
#include "common.h"
#include "app.h"
#include "corelib_gfx.h"
#include "corelib_mainloop.h"
#include "screens.h"
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
#include "experimental/mod_lucky/service.h"
#include "audio_manager.h"
#endif

#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
static bool luckyEditHeld, luckyFocusPending;
static uint64_t luckyRevision;
static unsigned luckyFrame;
static constexpr int luckyX[] = {18, 23, 28};
static int columnCount(int row) { return row == 8 ? 3 : 1; }
static void setup(int) {
  modLucky::service().enter(); luckyFocusPending = false;
  luckyRevision = 0;
}
static void draw(void);
static void luckyActivate(int column) {
  auto& lucky = modLucky::service();
  auto state = lucky.status();
  if (state.busy()) return;
  try {
  if (column == 0) {
    luckyFocusPending = lucky.next(appSettings.audioSampleRate);
    if (!luckyFocusPending) screenMessage(MESSAGE_TIME_ERROR, "Please wait before NEXT");
  } else if (column == 1 && state.candidate) {
    // Existing transport stop, never a project edit or a physical PLAY remap.
    if (audioManager.pause) audioManager.pause();
    chipnomadQueuePlaybackStop(chipnomadState);
    if (audioManager.stopSamplePreview) audioManager.stopSamplePreview();
    lucky.play();
    if (audioManager.resume) audioManager.resume();
  } else if (column == 2 && state.canLoad()) {
    lucky.import(chipnomadState->project, appSettings.samplePath);
  }
  } catch (const std::exception& e) { screenMessage(MESSAGE_TIME_ERROR, "%s", e.what()); }
}
#else
static int columnCount(int) { return 1; }
static void setup(int) {}
static void draw(void) {}
#endif
static void drawStatic(void) { gfxSetFgColor(appSettings.colorScheme.textTitles); gfxPrint(0, 0, "SETTINGS"); }
static void drawCursor(int col, int row) {
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (row == 8) {
    gfxSetCursorColor(appSettings.colorScheme.cursor);
    gfxCursor(luckyX[col], 17, 4);
    return;
  }
#endif
  if (row < 2) gfxCursor(23, 2 + row, 3);
  else if (row == 2) gfxCursor(23, 4, 6);
  else if (row < 8) { static const int widths[] = {4, 11, 6, 5, 8}; gfxCursor(0, 2 + row, widths[row - 3]); }
  else if (row == 9) gfxCursor(0, 18, 19);
}
static void noHeader(int, CellState) {}
static void drawField(int col, int row, CellState state) {
  const ColorScheme cs = appSettings.colorScheme;
  gfxSetFgColor(cs.textDefault);
  if (row == 0) { gfxPrint(0, 2, "Repeat delay"); gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrintf(23, 2, "%03d", appSettings.keyRepeatDelay); }
  else if (row == 1) { gfxPrint(0, 3, "Repeat speed"); gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrintf(23, 3, "%03d", appSettings.keyRepeatSpeed); }
  else if (row == 2) { gfxPrint(0, 4, "Stick live mode"); gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(23, 4, appSettings.stickLiveMode == StickLiveMode::free ? "FREE  " : appSettings.stickLiveMode == StickLiveMode::toggle ? "TOGGLE" : "HOLD  "); }
  else if (row >= 3 && row <= 7) { static const char* labels[] = {"MIDI", "Key mapping", "Synths", "Mixer", "Graphics"}; gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(0, 2 + row, labels[row - 3]); }
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  else if (row == 8) {
    auto lucky = modLucky::service().status();
    gfxPrint(0, 17, "I'm Feeling Lucky");
    const char* labels[] = {"NEXT", "PLAY", "LOAD"};
    for (int i = 0; i < 3; ++i) {
      bool enabled = !lucky.busy() && (i == 0 || (i == 1 && lucky.candidate) || (i == 2 && lucky.canLoad()));
      int color = enabled ? (state == CellState::focus && i == col ? cs.textValue : cs.textDefault) : cs.textEmpty;
      gfxSetFgColor(color); gfxPrint(luckyX[i], 17, labels[i]);
    }
  }
#endif
  else if (row == 9) { gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(0, 18, "Quit ChooChooTracker"); }
}
static int onEdit(int col, int row, CellEditAction action) {
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (row == 8) {
    // Logical key activation is edge-gated in onInput; touch taps use this path.
    if (action == CellEditAction::tap && !luckyEditHeld) luckyActivate(col);
    return 1;
  }
#endif
  if (row == 0) { uint8_t value = appSettings.keyRepeatDelay; int handled = edit8noLast(action, &value, 4, 4, 30); if (handled) appSettings.keyRepeatDelay = value; return handled; }
  if (row == 1) { uint8_t value = appSettings.keyRepeatSpeed; int handled = edit8noLast(action, &value, 2, 1, 12); if (handled) appSettings.keyRepeatSpeed = value; return handled; }
  if (row == 2) { uint8_t value = (uint8_t)appSettings.stickLiveMode; int handled = edit8noLast(action, &value, 1, 0, 2); if (handled) appSetStickLiveMode((StickLiveMode)value); return handled; }
  if (action != CellEditAction::tap) return 0;
  if (row == 3) screenSetup(&screenMidi, 0);
  else if (row == 4) screenSetup(&screenKeyMapping, 0);
  else if (row == 5) screenSetup(&screenSynthSettings, 0);
  else if (row == 6) screenSetup(&screenMixerSettings, 0);
  else if (row == 7) screenSetup(&screenGraphicsSettings, 0);
  else if (row == 9) { mainLoopTriggerQuit(); return 1; }
  return 0;
}
static ScreenData data = {
  .rows = 10, .cursorRow = 0, .cursorCol = 0, .topRow = 0, .selectMode = -1,
  .selectStartRow = 0, .selectStartCol = 0, .selectAnchorRow = 0, .selectAnchorCol = 0,
  .playbackLevel = ScreenPlaybackLevel::none, .getColumnCount = columnCount,
  .drawStatic = drawStatic, .drawCursor = drawCursor, .drawSelection = NULL,
  .drawRowHeader = noHeader, .drawColHeader = noHeader, .drawField = drawField,
  .onEdit = onEdit, .onInput = NULL, .onRawInput = NULL, .isCellValid = NULL, .getLoopRange = NULL,
};
static void fullRedraw(void) { screenFullRedraw(&data); }
static int onInput(int isKeyDown, int keys, int taps) {
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  const bool fresh = isKeyDown && keys == keyEdit && !luckyEditHeld;
  luckyEditHeld = (keys & keyEdit) != 0;
  if (data.cursorRow == 8 && keys == keyEdit) {
    if (fresh) luckyActivate(data.cursorCol);
    return 1;
  }
  if (keys & (keyUp | keyDown | keyLeft | keyRight)) luckyFocusPending = false;
#endif
  if (keys == (keyUp | keyShift)) { screenSetup(&screenSong, 0); return 1; }
  return screenInput(&data, isKeyDown, keys, taps);
}
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
static void draw(void) {
  auto& lucky = modLucky::service();
  auto state = lucky.status();
  if (state.bankReady) {
    if (audioManager.pause) audioManager.pause();
    const bool committed = lucky.commit(chipnomadState->project);
    if (committed) appModLuckyImported();
    if (audioManager.resume) audioManager.resume();
    state = lucky.status();
  }
  if (state.revision != luckyRevision) {
    if (luckyFocusPending && data.cursorRow == 8 && data.cursorCol == 0 &&
        state.phase == modLucky::Phase::ready) data.cursorCol = 1;
    if (state.phase != modLucky::Phase::loading) luckyFocusPending = false;
    if (!state.message.empty()) screenMessage(state.phase == modLucky::Phase::failed ? MESSAGE_TIME_ERROR : MESSAGE_TIME,
                                             "%s", state.message.c_str());
    luckyRevision = state.revision;
  }
  drawField(data.cursorCol, 8, data.cursorRow == 8 ? CellState::focus : CellState::normal);
  if (data.cursorRow == 8) drawCursor(data.cursorCol, 8);
  if (state.phase == modLucky::Phase::loading && data.cursorRow == 8 && data.cursorCol == 0) {
    // Four theme-colored steps: indeterminate brightness only, never a percentage.
    const int head = (luckyFrame++ / 3) % 4;
    const unsigned background = appSettings.colorScheme.background;
    const unsigned foreground = appSettings.colorScheme.cursor;
    for (int i = 0; i < 4; ++i) {
      const unsigned amount = 64 + ((i - head + 4) % 4) * 63;
      unsigned color = 0;
      for (unsigned shift : {0U, 8U, 16U}) {
        unsigned a = (background >> shift) & 255, b = (foreground >> shift) & 255;
        color |= ((a * (255 - amount) + b * amount) / 255) << shift;
      }
      gfxSetCursorColor(color); gfxCursor(luckyX[0] + i, 17, 1);
    }
    gfxSetCursorColor(appSettings.colorScheme.cursor);
  }
}
#endif
static ScreenPlaybackLevel playbackLevel(void) { return ScreenPlaybackLevel::song; }
const AppScreen screenSettings = {NULL, setup, fullRedraw, draw, onInput, playbackLevel};
