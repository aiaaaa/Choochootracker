#include "screens.h"
#include "common.h"
#include "corelib_gfx.h"

static constexpr int fieldX[] = {4, 13, 19, 24};
static constexpr int fieldWidth[] = {6, 3, 3, 3};
static constexpr int allRow = PROJECT_MAX_TRACKS;
static constexpr int doneRow = allRow + 1;
static void fullRedraw();

static int columnCount(int row) { return row < allRow ? 4 : row == allRow ? 2 : 1; }
static void drawStatic() {
  gfxSetFgColor(appSettings.colorScheme.textTitles);
  gfxPrint(0, 0, "TRACK VISUALS");
  gfxPrint(0, 2, "TRK");
  gfxSetFgColor(appSettings.colorScheme.textInfo);
  gfxPrint(0, 16, "DETAIL: waveform + overlays");
  gfxPrint(0, 17, "AUDIO: summed track output");
  gfxPrint(0, 18, "EDIT toggles; SHIFT+LEFT back");
}
static void drawCursor(int col, int row) {
  if (row < allRow) gfxCursor(fieldX[col], 3 + row, fieldWidth[col]);
  else if (row == allRow) gfxCursor(col ? 15 : 0, 12, col ? 9 : 12);
  else gfxCursor(0, 14, 4);
}
static void drawRowHeader(int row, CellState state) {
  if (row >= allRow) return;
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textDefault : appSettings.colorScheme.textInfo);
  gfxPrintf(0, 3 + row, "%d", row + 1);
}
static void drawColHeader(int col, CellState state) {
  static const char* labels[] = {"MODE", "WAVE", "ENV", "NOISE"};
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textDefault : appSettings.colorScheme.textTitles);
  gfxPrint(fieldX[col], 2, labels[col]);
}
static void drawField(int col, int row, CellState state) {
  const auto cs = appSettings.colorScheme;
  gfxSetFgColor(state == CellState::focus ? cs.textDefault : cs.textValue);
  if (row < allRow) {
    const auto& visual = appSettings.trackVisuals[row];
    if (col == 0) gfxPrint(fieldX[col], 3 + row, visual.mode == TrackVisualMode::audio ? "AUDIO " : "DETAIL");
    else if (col > 1 && visual.mode == TrackVisualMode::audio) {
      gfxSetFgColor(cs.textEmpty);
      gfxPrint(fieldX[col], 3 + row, "---");
    } else {
      bool enabled = col == 1 ? visual.wave : col == 2 ? visual.envelope : visual.noise;
      gfxPrint(fieldX[col], 3 + row, enabled ? "ON " : "OFF");
    }
  } else if (row == allRow) gfxPrint(col ? 15 : 0, 12, col ? "All audio" : "All detailed");
  else gfxPrint(0, 14, "Done");
}
static void done() {
  if (settingsSave() == 0) screenSetup(&screenSettings, 0);
  else screenMessage(MESSAGE_TIME, "Could not save track visuals");
}
static int onEdit(int col, int row, CellEditAction action) {
  if (row == doneRow) {
    if (action != CellEditAction::tap) return 0;
    done();
    return 1;
  }
  if (row == allRow) {
    if (action != CellEditAction::tap) return 0;
    for (auto& visual : appSettings.trackVisuals) {
      visual.mode = col ? TrackVisualMode::audio : TrackVisualMode::detailed;
      if (col) visual.wave = 1;
    }
    fullRedraw();
    return 1;
  }
  auto& visual = appSettings.trackVisuals[row];
  if (col > 1 && visual.mode == TrackVisualMode::audio) return 0;
  uint8_t value = col == 0 ? (uint8_t)visual.mode : col == 1 ? visual.wave : col == 2 ? visual.envelope : visual.noise;
  if (action == CellEditAction::tap || action == CellEditAction::doubleTap) value ^= 1;
  else if (!edit8noLast(action, &value, 1, 0, 1)) return 0;
  if (col == 0) visual.mode = (TrackVisualMode)value;
  else if (col == 1) visual.wave = value;
  else if (col == 2) visual.envelope = value;
  else visual.noise = value;
  if (col == 0) fullRedraw();
  return 1;
}
static ScreenData screen = {
  .rows = doneRow + 1,
  .cursorRow = 0, .cursorCol = 0, .topRow = 0, .selectMode = -1,
  .selectStartRow = 0, .selectStartCol = 0, .selectAnchorRow = 0, .selectAnchorCol = 0,
  .playbackLevel = ScreenPlaybackLevel::none,
  .getColumnCount = columnCount, .drawStatic = drawStatic, .drawCursor = drawCursor,
  .drawSelection = nullptr, .drawRowHeader = drawRowHeader, .drawColHeader = drawColHeader,
  .drawField = drawField, .onEdit = onEdit, .onInput = nullptr, .onRawInput = nullptr,
  .isCellValid = nullptr, .getLoopRange = nullptr,
};
static void setup(int) {}
static void fullRedraw() { screenFullRedraw(&screen); }
static void draw() {}
static int onInput(int isKeyDown, int keys, int tapCount) {
  if (keys == (keyShift | keyLeft)) {
    if (isKeyDown) done();
    return 1;
  }
  return screenInput(&screen, isKeyDown, keys, tapCount);
}
static ScreenPlaybackLevel playbackLevel() { return ScreenPlaybackLevel::song; }
const AppScreen screenTrackVisuals = {nullptr, setup, fullRedraw, draw, onInput, playbackLevel};
