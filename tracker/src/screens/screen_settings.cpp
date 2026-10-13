#include "screen_settings.h"
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
#include "support_report.h"
#include "corelib_file.h"
#include <string.h>
#include <stdio.h>

#ifdef ANDROID_BUILD
static constexpr int workingFolderRow = 7;
static constexpr int baseQuitRow = 8;
static char workingFolderStatus[256];
static void refreshWorkingFolderStatus(void) {
  if (fileGetWorkingFolderStatus(workingFolderStatus, sizeof(workingFolderStatus)) != 0)
    snprintf(workingFolderStatus, sizeof(workingFolderStatus), "Unavailable");
}
#else
static constexpr int baseQuitRow = 7;
#endif

#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
static constexpr int luckyRow = baseQuitRow;
static constexpr int quitRow = baseQuitRow + 1;
static bool luckyEditHeld, luckyFocusPending;
static uint64_t luckyRevision;
static unsigned luckyFrame;
static constexpr int luckyX[] = {18, 23, 28};
static int columnCount(int row) { return row == luckyRow ? 3 : 1; }
static void setup(int) {
#ifdef ANDROID_BUILD
  refreshWorkingFolderStatus();
#endif
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
static constexpr int quitRow = baseQuitRow;
static int columnCount(int) { return 1; }
static void setup(int) {
#ifdef ANDROID_BUILD
  refreshWorkingFolderStatus();
  screenMessage(MESSAGE_TIME, "%s", workingFolderStatus);
#endif
}
static void draw(void) {}
#endif
static void drawStatic(void) { gfxSetFgColor(appSettings.colorScheme.textTitles); gfxPrint(0, 0, "SETTINGS"); }
static void drawCursor(int col, int row) {
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (row == luckyRow) {
    gfxSetCursorColor(appSettings.colorScheme.cursor);
    gfxCursor(luckyX[col], 17, 4);
    return;
  }
#endif
  if (row < 6) { static const int widths[] = {8, 5, 4, 6, 5, 8}; gfxCursor(0, 2 + row, widths[row]); }
  else if (row == 6) gfxCursor(0, 10, 14);
#ifdef ANDROID_BUILD
  else if (row == workingFolderRow) gfxCursor(0, 12, 14);
#endif
  else if (row == quitRow) gfxCursor(0, 18, 19);
}
// Keep touch hit-testing beside the platform-dependent Settings row layout.
int screenSettingsTouchCell(int col, int row, int* targetCol, int* targetRow) {
  int field = -1, column = 0, width = 0;
  if (row >= 2 && row <= 7) {
    static const int widths[] = {8, 5, 4, 6, 5, 8};
    field = row - 2; width = widths[field];
  } else if (row == 10) { field = 6; width = 14; }
#ifdef ANDROID_BUILD
  else if (row == 12) { field = workingFolderRow; width = 30; }
#endif
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  else if (row == 17) {
    for (int i = 0; i < 3; ++i) {
      if (col >= luckyX[i] && col < luckyX[i] + 4) {
        if (targetCol) *targetCol = i;
        if (targetRow) *targetRow = luckyRow;
        return 1;
      }
    }
    return 0;
  }
#endif
  else if (row == 18) { field = quitRow; width = 19; }
  if (field < 0 || col < 0 || col >= width) return 0;
  if (targetCol) *targetCol = column;
  if (targetRow) *targetRow = field;
  return 1;
}
static void noHeader(int, CellState) {}
static void drawField(int col, int row, CellState state) {
  const ColorScheme cs = appSettings.colorScheme;
  gfxSetFgColor(cs.textDefault);
  if (row < 6) { static const char* labels[] = {"Controls", "Audio", "MIDI", "Synths", "Mixer", "Graphics"}; gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(0, 2 + row, labels[row]); }
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  else if (row == luckyRow) {
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
  else if (row == 6) { gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(0, 10, "Support report"); }
#ifdef ANDROID_BUILD
  else if (row == workingFolderRow) {
    gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault);
    gfxPrint(0, 12, "Working folder");
    gfxPrint(23, 12, strncmp(workingFolderStatus, "Folder:", 7) == 0 ? "FOLDER " :
      strcmp(workingFolderStatus, "Unavailable") == 0 ? "N/A    " : "PRIVATE");
  }
#endif
  else if (row == quitRow) { gfxSetFgColor(state == CellState::focus ? cs.textValue : cs.textDefault); gfxPrint(0, 18, "Quit ChooChooTracker"); }
}
static int onEdit(int col, int row, CellEditAction action) {
#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
  if (row == luckyRow) {
    // Logical key activation is edge-gated in onInput; touch taps use this path.
    if (action == CellEditAction::tap && !luckyEditHeld) luckyActivate(col);
    return 1;
  }
#endif
#ifdef ANDROID_BUILD
  if (row == workingFolderRow && action == CellEditAction::clear) {
    if (strcmp(workingFolderStatus, "Unavailable") == 0) { screenMessage(MESSAGE_TIME, "Not available in this build"); return 1; }
    fileClearWorkingFolder();
    return 1;
  }
#endif
  if (action != CellEditAction::tap) return 0;
  if (row == 0) screenSetup(&screenControlsSettings, 0);
  else if (row == 1) screenSetup(&screenAudioSettings, 0);
  else if (row == 2) screenSetup(&screenMidi, 0);
  else if (row == 3) screenSetup(&screenSynthSettings, 0);
  else if (row == 4) screenSetup(&screenMixerSettings, 0);
  else if (row == 5) screenSetup(&screenGraphicsSettings, 0);
  else if (row == 6) {
    char path[PATH_LENGTH + 32];
    if (supportReportSaveDefault("Settings", path, sizeof(path)) == 0)
      screenMessage(MESSAGE_TIME, "Support report saved");
    else
      screenMessage(MESSAGE_TIME_ERROR, "Support report failed");
    return 1;
  }
#ifdef ANDROID_BUILD
  else if (row == workingFolderRow) {
    if (strcmp(workingFolderStatus, "Unavailable") == 0) screenMessage(MESSAGE_TIME, "Not available in this build");
    else filePickWorkingFolder();
    return 0;
  }
#endif
  else if (row == quitRow) { mainLoopTriggerQuit(); return 1; }
  return 0;
}
static ScreenData data = {
  .rows = quitRow + 1, .cursorRow = 0, .cursorCol = 0, .topRow = 0, .selectMode = -1,
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
  if (data.cursorRow == luckyRow && keys == keyEdit) {
    if (fresh) luckyActivate(data.cursorCol);
    return 1;
  }
  if (keys & (keyUp | keyDown | keyLeft | keyRight)) luckyFocusPending = false;
#endif
  if (keys == keyOpt || keys == (keyUp | keyShift)) { screenSetup(&screenSong, 0); return 1; }
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
    if (luckyFocusPending && data.cursorRow == luckyRow && data.cursorCol == 0 &&
        state.phase == modLucky::Phase::ready) data.cursorCol = 1;
    if (state.phase != modLucky::Phase::loading) luckyFocusPending = false;
    if (!state.message.empty()) screenMessage(state.phase == modLucky::Phase::failed ? MESSAGE_TIME_ERROR : MESSAGE_TIME,
                                             "%s", state.message.c_str());
    luckyRevision = state.revision;
  }
  drawField(data.cursorCol, luckyRow, data.cursorRow == luckyRow ? CellState::focus : CellState::normal);
  if (data.cursorRow == luckyRow) drawCursor(data.cursorCol, luckyRow);
  if (state.phase == modLucky::Phase::loading && data.cursorRow == luckyRow && data.cursorCol == 0) {
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
