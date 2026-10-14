#include <algorithm>
#include <cstring>
#include <map>

#include "chipnomad_lib.h"
#include "corelib_gfx.h"
#include "screen_instrument.h"
#include "selection_popup.h"
#include "soundbank_library.h"

namespace {
enum class Page { collections, banks, presets };
Page page = Page::collections;
std::vector<std::string> labels, collectionIds;
std::vector<int> bankIndices;
std::vector<SelectionItem> choices;
std::string selectedCollection, selectedBank, lastStatus;
int buttonDown = 0;
bool lastBusy = false, showAssets = false;
bool rawSample(const soundbank::BankEntry& bank) {
  return bank.format == "wav" || bank.format == "aif" || bank.format == "aiff";
}
extern const AppScreen screenSoundbankBrowser;
Instrument& current() { return chipnomadState->project.instruments[cInstrument]; }
std::string collectionOf(const std::string& id) {
  auto slash = id.find('/'), archive = id.find("::");
  if (slash != std::string::npos && (archive == std::string::npos || slash < archive))
    return id.substr(0, slash);
  return id.substr(0, archive);
}
void back() { screenSetup(&screenInstrument, cInstrument); }
void show();
void select(int value) {
  if (value == -2) {
    back();
    return;
  }
  if (value == -3) {
    showAssets = true;
    show();
    return;
  }
  if (value == -1) {
    if (page == Page::collections)
      soundbankRescan(true);
    else if (page == Page::banks && showAssets)
      showAssets = false;
    else
      page = page == Page::presets ? Page::banks : Page::collections;
    show();
    return;
  }
  if (soundbankBusy() || value < 0) return;
  const auto& banks = soundbankEntries();
  if (page == Page::collections) {
    if (size_t(value) >= collectionIds.size()) return;
    selectedCollection = collectionIds[value];
    showAssets = false;
    page = Page::banks;
    show();
  } else if (page == Page::banks) {
    if (size_t(value) >= bankIndices.size()) return;
    const auto& bank = banks[bankIndices[value]];
    if (!bank.error.empty()) {
      screenMessage(MESSAGE_TIME_ERROR, "%.100s", bank.error.c_str());
      return;
    }
    // Browsing another bank does not discard the playable instrument. Selection
    // becomes persistent only after preparation succeeds.
    selectedBank = bank.id;
    page = Page::presets;
    show();
  } else {
    for (const auto& bank : banks) {
      if (bank.id == selectedBank && size_t(value) < bank.presets.size()) {
        soundbankLoad(cInstrument, bank.id, bank.presets[value].first);
        break;
      }
    }
    show();
  }
}
void preview(int value, bool held) {
  if (!held) {
    chipnomadQueuePlaybackStopPreview(chipnomadState, *pSongTrack);
    return;
  }
  if (page != Page::presets || soundbankBusy() || !current().chip.soundfont.prepared) return;
  for (const auto& bank : soundbankEntries()) {
    if (bank.id == selectedBank && value >= 0 && size_t(value) < bank.presets.size() &&
        bank.id == current().chip.soundfont.bank &&
        bank.presets[value].first == current().chip.soundfont.preset)
      chipnomadQueuePlaybackPreviewNote(chipnomadState, *pSongTrack, 48, cInstrument);
  }
}
void show() {
  labels.clear();
  choices.clear();
  collectionIds.clear();
  bankIndices.clear();
  labels.push_back("< Instrument");
  labels.push_back(page == Page::collections ? "Rescan SoundBanks"
                   : page == Page::banks     ? (showAssets ? "< Instruments" : "< Collections")
                                             : "< Bank Manager");
  int selected = -2;
  bool rawOption = false;
  const auto& banks = soundbankEntries();
  if (page == Page::collections) {
    std::map<std::string, size_t> collections, rawCounts;
    for (const auto& bank : banks) {
      auto id = collectionOf(bank.id);
      collections[id] += rawSample(bank) ? 0 : bank.presets.size();
      if (rawSample(bank)) rawCounts[id] += bank.presets.size();
    }
    for (auto& collection : collections)
      if (!collection.second) collection.second = rawCounts[collection.first];
    for (const auto& collection : collections) {
      collectionIds.push_back(collection.first);
      labels.push_back(collection.first + " (" + std::to_string(collection.second) + ")");
      if (collection.first == selectedCollection) selected = int(collectionIds.size()) - 1;
    }
  } else if (page == Page::banks) {
    bool hasMappings = std::any_of(banks.begin(), banks.end(), [](const auto& bank) {
      return collectionOf(bank.id) == selectedCollection && !rawSample(bank);
    });
    for (size_t n = 0; n < banks.size(); ++n) {
      const auto& bank = banks[n];
      if (collectionOf(bank.id) != selectedCollection) continue;
      if (hasMappings && rawSample(bank) != showAssets) {
        if (rawSample(bank)) rawOption = true;
        continue;
      }
      bankIndices.push_back(int(n));
      std::string name = bank.id.substr(selectedCollection.size());
      if (name.rfind("::", 0) == 0)
        name.erase(0, 2);
      else if (!name.empty() && name.front() == '/')
        name.erase(0, 1);
      if (name.empty()) name = bank.name;
      labels.push_back(name + " [" + bank.format + "] " + std::to_string(bank.presets.size()) +
                       (bank.error.empty() && bank.warnings.empty() ? "" : " !"));
      if (bank.id == selectedBank) selected = int(bankIndices.size()) - 1;
    }
  } else {
    for (const auto& bank : banks)
      if (bank.id == selectedBank) {
        for (const auto& preset : bank.presets) {
          labels.push_back(preset.second);
          if (bank.id == current().chip.soundfont.bank &&
              preset.first == current().chip.soundfont.preset)
            selected = int(labels.size()) - 3;
        }
        break;
      }
  }
  if (rawOption) labels.push_back("Raw WAV/AIFF samples >");
  if (selected == -2 && labels.size() > 2) selected = 0;
  choices.push_back({labels[0].c_str(), -2, nullptr, 0, nullptr});
  choices.push_back({labels[1].c_str(), -1, nullptr, 0, nullptr});
  for (size_t n = 2; n < labels.size(); ++n)
    choices.push_back({labels[n].c_str(), rawOption && n + 1 == labels.size() ? -3 : int(n - 2),
                       nullptr, 0, nullptr});
  const char* title = soundbankBusy()         ? soundbankStatus().c_str()
                      : page == Page::presets ? "PRESET BROWSER"
                      : page == Page::banks   ? selectedCollection.c_str()
                                              : "BANK MANAGER / USER";
  selectionPopupSetup(title, choices.data(), int(choices.size()), selected, select, back, true,
                      preview);
  screenSetup(&screenSoundbankBrowser, 0);
}
void setup(int n) {
  screenSelectionPopup.setup(n);
  lastStatus = soundbankStatus();
  lastBusy = soundbankBusy();
}
void draw() {
  if (lastBusy != soundbankBusy() || lastStatus != soundbankStatus()) {
    show();
    return;
  }
  screenSelectionPopup.draw();
}
int input(int down, int keys, int taps) { return screenSelectionPopup.onInput(down, keys, taps); }
ScreenPlaybackLevel level() { return ScreenPlaybackLevel::none; }
const AppScreen screenSoundbankBrowser = {nullptr, setup, screenSelectionPopup.fullRedraw,
                                          draw,    input, level};
void open(bool banks) {
  selectedBank = current().chip.soundfont.bank;
  selectedCollection = collectionOf(selectedBank);
  page = banks || selectedBank.empty() ? Page::collections : Page::presets;
  if (soundbankEntries().empty() && !soundbankBusy()) soundbankRescan();
  show();
}
int columns(int row) { return row < 3 ? instrumentCommonColumnCount(row) : 1; }
void drawStatic() {
  instrumentCommonDrawStatic();
  gfxSetFgColor(appSettings.colorScheme.textInfo);
  gfxPrint(0, 10, "Copy libraries to SoundBanks");
}
void drawCursor(int col, int row) {
  if (row < 3)
    instrumentCommonDrawCursor(col, row);
  else
    gfxCursor(0, row + 3, 23);
}
void drawField(int col, int row, CellState state) {
  if (row < 3) {
    instrumentCommonDrawField(col, row, state);
    return;
  }
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textValue
                                          : appSettings.colorScheme.textDefault);
  gfxPrint(0, row + 3, row == 3 ? "BANK MANAGER       >" : "PRESET BROWSER     >");
}
int edit(int col, int row, CellEditAction action) {
  return row < 3 ? instrumentCommonOnEdit(col, row, action) : 0;
}
int onInput(int down, int keys, int) {
  if (screenInstrumentSoundFont.cursorRow < 3) {
    buttonDown = 0;
    return 0;
  }
  auto action = popupEditInput(down, keys, &buttonDown);
  if (action == PopupEditInput::hold) return 1;
  if (action == PopupEditInput::open) {
    open(screenInstrumentSoundFont.cursorRow == 3);
    return 1;
  }
  return 0;
}
}  // namespace
ScreenData screenInstrumentSoundFont = {.rows = 5,
                                        .cursorRow = 0,
                                        .cursorCol = 0,
                                        .topRow = 0,
                                        .selectMode = -1,
                                        .selectStartRow = 0,
                                        .selectStartCol = 0,
                                        .selectAnchorRow = 0,
                                        .selectAnchorCol = 0,
                                        .playbackLevel = ScreenPlaybackLevel::none,
                                        .getColumnCount = columns,
                                        .drawStatic = drawStatic,
                                        .drawCursor = drawCursor,
                                        .drawSelection = nullptr,
                                        .drawRowHeader = nullptr,
                                        .drawColHeader = nullptr,
                                        .drawField = drawField,
                                        .onEdit = edit,
                                        .onInput = onInput,
                                        .onRawInput = nullptr,
                                        .isCellValid = nullptr,
                                        .getLoopRange = nullptr};
