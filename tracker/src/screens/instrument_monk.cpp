#include "screen_instrument.h"
#include "corelib_gfx.h"
#include "utils.h"

static InstrumentMonk* current() { return &chipnomadState->project.instruments[cInstrument].chip.monk; }
static uint8_t* parameter(int row) {
  auto* m = current();
  uint8_t* values[] = {&m->vowel, &m->character, &m->breath, &m->glide, &m->vibrato, &m->rate};
  return row >= 3 && row <= 8 ? values[row - 3] : nullptr;
}
static int columns(int row) {
  return row < 3 ? instrumentCommonColumnCount(row) : row == 3 ? 1 : row == 9 ? 5 : 2;
}
static void drawStatic() {
  instrumentCommonDrawStatic();
  gfxSetFgColor(appSettings.colorScheme.textTitles);
  gfxPrint(0, 6, "Vowel");
  gfxPrint(0, 7, "SOURCE");
  gfxSetFgColor(appSettings.colorScheme.textDefault);
  const char* labels[] = {"Voice", "Breath", "Glide", "Vib depth", "Vib rate"};
  for (int i = 0; i < 5; ++i) gfxPrint(0, 8 + i, labels[i]);
  instrumentCommonDrawVoicePostStatic(1);
  gfxSetFgColor(appSettings.colorScheme.textInfo);
  gfxPrint(16, 6, "A   E   I   O   U");
}
static void drawCursor(int col, int row) {
  if (row < 3) instrumentCommonDrawCursor(col, row);
  else if (row == 9 || col) instrumentCommonDrawVoicePostCursor(col, row);
  else gfxCursor(11, row == 3 ? 6 : row + 4, 2);
}
static void drawField(int col, int row, CellState state) {
  if (row < 3) { instrumentCommonDrawField(col, row, state); return; }
  if (row == 9 || col) { instrumentCommonDrawVoicePostField(col, row, state, current()); return; }
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textValue : appSettings.colorScheme.textDefault);
  if (auto* value = parameter(row)) {
    gfxPrint(11, row == 3 ? 6 : row + 4, byteToHex(*value));
    if (row == 3) {
      const char* vowels[] = {"A", "E", "I", "O", "U"};
      const int nearest = (*value * 4 + 127) / 255;
      for (int i = 0; i < 5; ++i) {
        gfxSetFgColor(i == nearest ? appSettings.colorScheme.textValue : appSettings.colorScheme.textInfo);
        gfxPrint(16 + i * 4, 6, vowels[i]);
      }
    }
  }
}
static int edit(int col, int row, CellEditAction action) {
  if (row < 3) return instrumentCommonOnEdit(col, row, action);
  int changed = row == 9 || col ? instrumentCommonOnEditVoicePost(col, row, action, current()) :
    (parameter(row) ? edit8noLast(action, parameter(row), 16, 0, 255) : 0);
  if (changed) projectModified = 1;
  return changed;
}
ScreenData screenInstrumentMonk = {
  .rows = 10, .cursorRow = 0, .cursorCol = 0, .topRow = 0, .selectMode = -1,
  .selectStartRow = 0, .selectStartCol = 0, .selectAnchorRow = 0, .selectAnchorCol = 0,
  .playbackLevel = ScreenPlaybackLevel::none, .getColumnCount = columns, .drawStatic = drawStatic,
  .drawCursor = drawCursor, .drawSelection = nullptr, .drawRowHeader = nullptr, .drawColHeader = nullptr,
  .drawField = drawField, .onEdit = edit, .onInput = nullptr, .onRawInput = nullptr,
  .isCellValid = nullptr, .getLoopRange = nullptr,
};
