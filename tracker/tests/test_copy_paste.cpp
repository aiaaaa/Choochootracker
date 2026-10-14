#include "doctest.h"
#include "copy_paste.h"
#include "common.h"
#include "screens.h"

#include <cstring>

// copy_paste also owns selection-mode helpers; the clipboard tests do not link
// the full screen implementation.
void getSelectionBounds(ScreenData* screen, int* startCol, int* startRow, int* endCol, int* endRow) {
  *startCol = screen->selectStartCol < screen->cursorCol ? screen->selectStartCol : screen->cursorCol;
  *endCol = screen->selectStartCol > screen->cursorCol ? screen->selectStartCol : screen->cursorCol;
  *startRow = screen->selectStartRow < screen->cursorRow ? screen->selectStartRow : screen->cursorRow;
  *endRow = screen->selectStartRow > screen->cursorRow ? screen->selectStartRow : screen->cursorRow;
}

TEST_CASE("FX clipboard moves Phrase FX into Table lanes and truncates") {
  ChipNomadState state = {};
  chipnomadState = &state;
  projectInit(&state.project);
  PhraseRow& source0 = state.project.phrases[0].rows[0];
  PhraseRow& source1 = state.project.phrases[0].rows[1];
  source0.fx[0][0] = fxDMD; source0.fx[0][1] = 8;
  source1.fx[0][0] = fxDFM; source1.fx[0][1] = 255;
  copyPhrase(0, 3, 0, 4, 1, 1);
  CHECK(source0.fx[0][0] == EMPTY_VALUE_8); CHECK(source0.fx[0][1] == 0);
  CHECK(pasteTable(0, 9, 14) == 2);
  CHECK(state.project.tables[0].rows[14].fx[3][0] == fxDMD);
  CHECK(state.project.tables[0].rows[14].fx[3][1] == 8);
  CHECK(state.project.tables[0].rows[15].fx[3][0] == fxDFM);
  CHECK(state.project.tables[0].rows[15].fx[3][1] == 255);
}

TEST_CASE("FX clipboard moves Table FX into Phrase lanes and preserves partial cells") {
  ChipNomadState state = {};
  chipnomadState = &state;
  projectInit(&state.project);
  TableRow& source = state.project.tables[0].rows[0];
  source.fx[1][0] = fxDDC; source.fx[1][1] = 99;
  copyTable(0, 5, 0, 6, 0, 0);
  CHECK(pastePhrase(0, 3, 0) == 1);
  CHECK(state.project.phrases[0].rows[0].fx[0][0] == fxDDC);
  CHECK(state.project.phrases[0].rows[0].fx[0][1] == 99);
  copyTable(0, 6, 0, 6, 0, 1);
  CHECK(source.fx[1][0] == fxDDC); CHECK(source.fx[1][1] == 0);
  state.project.phrases[0].rows[1].fx[0][0] = fxDMD;
  CHECK(pastePhrase(0, 4, 1) == 1);
  CHECK(state.project.phrases[0].rows[1].fx[0][0] == fxDMD);
  CHECK(state.project.phrases[0].rows[1].fx[0][1] == 99);
}

// These exercise both the dedicated FX clipboard and the mixed-column path.
TEST_CASE("wide FX and empty commands survive Phrase/Table clipboard transfers") {
  ChipNomadState state = {};
  chipnomadState = &state;
  projectInit(&state.project);
  auto& source = state.project.phrases[0].rows[0];
  source.fx[0] = {0x100, 255};
  source.fx[1] = {FX_ID_EMPTY, 37};
  copyPhrase(0, 3, 0, 6, 0, 1);
  CHECK(source.fx[0].command == FX_ID_EMPTY);
  REQUIRE(pasteTable(0, 3, 0) == 1);
  auto& table = state.project.tables[0].rows[0];
  CHECK(table.fx[0].command == 0x100);
  CHECK(table.fx[0].value == 255);
  CHECK(table.fx[1].command == FX_ID_EMPTY);
  CHECK(table.fx[1].value == 37);
  copyTable(0, 3, 0, 6, 0, 0);
  REQUIRE(pastePhrase(0, 5, 1) == 1);
  CHECK(state.project.phrases[0].rows[1].fx[1].command == 0x100);
  CHECK(state.project.phrases[0].rows[1].fx[2].command == FX_ID_EMPTY);
  CHECK(state.project.phrases[0].rows[1].fx[2].value == 37);
}

TEST_CASE("mixed Phrase and Table selections paste FX fields including empty commands") {
  ChipNomadState state = {};
  chipnomadState = &state;
  projectInit(&state.project);
  auto& source = state.project.phrases[0].rows[0];
  source.note = 48;
  source.fx[0] = {0x100, 255};
  source.fx[1] = {FX_ID_EMPTY, 17};
  auto& target = state.project.phrases[0].rows[1];
  target.fx[0] = {fxVOL, 1};
  target.fx[1] = {fxARP, 2};
  copyPhrase(0, 0, 0, 6, 0, 0);
  REQUIRE(pastePhrase(0, 0, 1) == 1);
  CHECK(target.note == 48);
  CHECK(target.fx[0].command == 0x100);
  CHECK(target.fx[0].value == 255);
  CHECK(target.fx[1].command == FX_ID_EMPTY);
  CHECK(target.fx[1].value == 17);
  auto& table = state.project.tables[0].rows[0];
  auto& tableTarget = state.project.tables[0].rows[1];
  table.pitchOffset = 7;
  table.fx[0] = {0x101, 255};
  table.fx[1] = {FX_ID_EMPTY, 29};
  tableTarget.fx[0] = {fxVOL, 1};
  tableTarget.fx[1] = {fxARP, 2};
  copyTable(0, 0, 0, 6, 0, 0);
  REQUIRE(pasteTable(0, 0, 1) == 1);
  CHECK(tableTarget.pitchOffset == 7);
  CHECK(tableTarget.fx[0].command == 0x101);
  CHECK(tableTarget.fx[0].value == 255);
  CHECK(tableTarget.fx[1].command == FX_ID_EMPTY);
  CHECK(tableTarget.fx[1].value == 29);
}
