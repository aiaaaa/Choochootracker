#include "screen_project.h"
#include "common.h"
#include "corelib/corelib_file.h"
#include "corelib_gfx.h"
#include "utils.h"
#include "chipnomad_lib.h"
#include "project_utils.h"
#include "pitch_table_utils.h"
#include "version.h"
#include "audio_manager.h"
#include "file_browser.h"
#include "export_path.h"
#include "import/import_vt2.h"
#include "import/import_midi.h"
#include "import/import_m8s.h"
#include "import/import_lsdj.h"
#include "selection_popup.h"
#include <memory>
#include "string_utils.h"
#include "midi/midi_router.h"
#include <string.h>
#include <strings.h>

#ifdef WEB_BUILD
#include <emscripten/emscripten.h>
#endif

static int isCharEdit = 0;
static const AppScreen* projectReturnScreen = &screenProject;
static char* editingString = NULL;
static int editingStringLength = 0;
// Tempo is edited/displayed in whole BPM. The project's canonical tickRate
// stays in Hz (see project_io.cpp's "- Frame rate" format): BPM =
// tickRate * 60/24 = tickRate * 2.5, so tickRate = BPM * 0.4. Old projects
// with a fractional tempo (e.g. 59 Hz = 147.5 BPM) display rounded and snap
// to a whole BPM on the first tempo edit; untouched projects save unchanged.
static uint16_t bpmValue = 125;
// Mirror of appSettings.midiClockMode for the Clock source row.
static uint8_t clockSourceMirror = 0;

// Whole-BPM range: tickRate 1-200 Hz maps to 2.5-500 BPM, so whole numbers
// start at 3.
#define BPM_MIN (3)
#define BPM_MAX (500)
#define BPM_DEFAULT (125) // the old 50 Hz default

static int pendingSavSlot = -1;

int projectLoadFromPath(const char* path) {
  if (!path) {
    return 1;
  }

  // Store the directory path from the selected file
  const char* lastSeparator = strrchr(path, PATH_SEPARATOR);
  if (lastSeparator) {
    int pathLen = lastSeparator - path;
    if (pathLen > 0 && pathLen < PATH_LENGTH) {
      strncpy(appSettings.projectPath, path, pathLen);
      appSettings.projectPath[pathLen] = '\0';
    }
  } else {
    appSettings.projectPath[0] = '\0';
  }

  // Check file extension to determine loader
  const char* ext = path ? strrchr(path, '.') : nullptr;
  int loadResult = -1;
  auto replacement = std::make_unique<Project>();
  projectInitAY(replacement.get());

  if (ext != NULL) {
    if (strcasecmp(ext, ".vt2") == 0) {
      // Load VT2 file
      loadResult = projectLoadVT2(replacement.get(), path);
    } else if (strcasecmp(ext, ".mid") == 0 || strcasecmp(ext, ".midi") == 0) {
      // Import a Standard MIDI File as a new project
      loadResult = projectLoadMidi(replacement.get(), path);
    } else if (strcasecmp(ext, ".m8s") == 0) {
      // Import a Dirtywave M8 song (structure and notes only)
      loadResult = projectLoadM8SWithSamples(replacement.get(), path, appSettings.samplePath);
    } else if (strcasecmp(ext, ".lsdsng") == 0) {
      // Import a Little Sound Dj song (structure and notes only)
      loadResult = projectLoadLSDJ(replacement.get(), path);
    } else if (strcasecmp(ext, ".sav") == 0) {
      // One song of an LSDj save, picked in the list
      if (pendingSavSlot >= 0) {
        loadResult = projectLoadLSDJSav(replacement.get(), path, pendingSavSlot);
      } else {
        snprintf(projectFileError, 40, "Pick a song in the .sav list");
        loadResult = 1;
      }
    } else if (strcasecmp(ext, ".cct") == 0) {
      // Load ChooChooTracker native format
      loadResult = projectLoad(replacement.get(), path);
    } else {
      // Try native format by default
      loadResult = projectLoad(replacement.get(), path);
    }
  } else {
    // No extension, try native format
    loadResult = projectLoad(replacement.get(), path);
  }

  if (loadResult == 0) {
    char loadedTitle[PROJECT_TITLE_LENGTH + 1];
    snprintf(loadedTitle, sizeof(loadedTitle), "%s", replacement->title);
    audioManager.replaceProject(replacement.get());
    autosaveLoadFailed = 0;
    projectModified = 0; // Clear modified flag after loading

    // Store filename without extension
    if (pendingSavSlot >= 0) snprintf(appSettings.projectFilename, FILENAME_LENGTH + 1, "%s", loadedTitle);
    else extractFilenameWithoutExtension(path, appSettings.projectFilename, FILENAME_LENGTH + 1);
    // The loaded project's export folder (if any) is adopted on next export
    exportResetFolderTracking();
    settingsSave();

    // Reset all screen states (including song position)
    screensInitAll();
    // Clear FX states for all tracks
    // TODO: Make it a cleaner solution and have it somewhere in chipnomad_lib
    for (int i = 0; i < PROJECT_MAX_TRACKS; i++) {
      chipnomadQueuePlaybackClearTrackFX(chipnomadState, i);
    }
  } else {
    projectFree(replacement.get());
    screenMessage(MESSAGE_TIME_ERROR, "%s", projectFileError);
  }

  return loadResult;
}

static void onProjectCancelled(void) {
  screenSetup(projectReturnScreen, 0);
}
static void onProjectLoaded(const char* path);

// An LSDj save (.sav) holds up to 32 songs: list them and load the one picked.
static char savPath[2048];
static LsdjSavSong savSongs[32];
static char savLabels[32][40];
static SelectionItem savItems[32];

static void onSavSongSelected(int slot) {
  pendingSavSlot = slot;
  int result = projectLoadFromPath(savPath);
  pendingSavSlot = -1;
  if (result == 0 && projectReturnScreen == &screenTitle) screenSetup(&screenSong, 0);
  else screenSetup(projectReturnScreen, 0);
}

static int openLsdjSave(const char* path) {
  int count = 0;
  if (lsdjListSav(path, savSongs, 32, &count) != 0) {
    screenMessage(MESSAGE_TIME_ERROR, "%s", projectFileError);
    return 0;
  }
  snprintf(savPath, sizeof(savPath), "%s", path);
  for (int i = 0; i < count; i++) {
    snprintf(savLabels[i], sizeof(savLabels[i]), "%02d %-8s v%02X %3d blocks", savSongs[i].slot, savSongs[i].name[0] ? savSongs[i].name : "-", savSongs[i].version, savSongs[i].blocks);
    savItems[i] = {savLabels[i], savSongs[i].slot, nullptr, 0, nullptr};
  }
  selectionPopupSetup("LSDJ SONGS", savItems, count, savSongs[0].slot, onSavSongSelected, onProjectCancelled, true);
  screenSetup(&screenSelectionPopup, 0);
  return 1;
}

static void onProjectLoaded(const char* path) {
  const char* ext = path ? strrchr(path, '.') : NULL;
  if (ext && strcasecmp(ext, ".sav") == 0) {
    if (!openLsdjSave(path)) screenSetup(projectReturnScreen, 0);
    return;
  }
  if (projectLoadFromPath(path) == 0 && projectReturnScreen == &screenTitle)
    screenSetup(&screenSong, 0);
  else
    screenSetup(projectReturnScreen, 0);
}

#ifdef WEB_BUILD
extern "C" EMSCRIPTEN_KEEPALIVE int webLoadProject(const char* path) {
  const char* ext = path ? strrchr(path, '.') : nullptr;
  if (ext && strcasecmp(ext, ".sav") == 0) {
    projectReturnScreen = &screenSong;
    return openLsdjSave(path) ? 2 : 1; // The tracker picker completes the import.
  }
  return projectLoadFromPath(path);
}
#endif

static void onProjectSaved(const char* folderPath) {
  char fullPath[2048];
  snprintf(fullPath, sizeof(fullPath), "%s%s%s.cct", folderPath, PATH_SEPARATOR_STR, appSettings.projectFilename);

  if (projectSave(&chipnomadState->project, fullPath) == 0) {
    // Save the directory path
    projectModified = 0; // Clear modified flag after saving
    strncpy(appSettings.projectPath, folderPath, PATH_LENGTH);
    appSettings.projectPath[PATH_LENGTH] = 0;
    // If exports were made under a previous project name, move that folder
    // to match the name the project was just saved with
    exportSyncFolderWithProjectName();
    settingsSave();
  }
  screenSetup(&screenProject, 0);
}


static void doLoadProject(void) {
  fileBrowserSetup("LOAD PROJECT", ".cct,.vt2,.mid,.midi,.m8s,.lsdsng,.sav", appSettings.projectPath,
    onProjectLoaded, onProjectCancelled);
  screenSetup(&screenFileBrowser, 0);
}

static void doNewProject(void) {
  Project replacement;
  projectInitAY(&replacement);
  audioManager.replaceProject(&replacement);
  projectModified = 0;
  appSettings.projectFilename[0] = 0;
  exportResetFolderTracking();
  settingsSave();
  screensInitAll();
  screenSetup(projectReturnScreen, 0);
}

void projectOpenFromScreen(const AppScreen* returnScreen) {
  projectReturnScreen = returnScreen ? returnScreen : &screenProject;
  doLoadProject();
}

void projectOpenFromScreenAtPath(const AppScreen* returnScreen, const char* path) {
  projectReturnScreen = returnScreen ? returnScreen : &screenProject;
  fileBrowserSetup("LOAD PROJECT", ".cct,.vt2,.mid,.midi,.m8s,.lsdsng,.sav", path,
    onProjectLoaded, onProjectCancelled);
  screenSetup(&screenFileBrowser, 0);
}

void projectCreateNewFromScreen(const AppScreen* returnScreen) {
  projectReturnScreen = returnScreen ? returnScreen : &screenProject;
  doNewProject();
}

static void cancelConfirm(void) {
  screenSetup(&screenProject, 0);
}

static void drawRowHeader(int row, CellState state);
static void drawColHeader(int col, CellState state);

static ScreenData* projectScreen(void) {
  ScreenData* data = &screenProjectExtra;
  data->drawRowHeader = drawRowHeader;
  data->drawColHeader = drawColHeader;

  return data;
}

static void setup(int input) {
  isCharEdit = 0;
  editingString = NULL;
  editingStringLength = 0;
  // Round to whole BPM: 50 Hz -> 125, 59 Hz -> 148 (147.5 rounded).
  int bpm = (int)(chipnomadState->project.tickRate * 2.5f + 0.5f);
  if (bpm < BPM_MIN) bpm = BPM_MIN;
  if (bpm > BPM_MAX) bpm = BPM_MAX;
  bpmValue = (uint16_t)bpm;
  clockSourceMirror = appSettings.midiClockMode ? 1 : 0;
}

static void fullRedraw(void) {
  ScreenData* screen = projectScreen();
  screenFullRedraw(screen);
}

// Live followed tempo while slaved to MIDI clock, rounded to whole BPM
// (app.cpp keeps project.tickRate updated from the router's measurement).
static int liveClockBpm(void) {
  return (int)(chipnomadState->project.tickRate * 2.5f + 0.5f);
}

static void draw(void) {
  // While slaved to MIDI clock the tempo field shows the live followed
  // tempo; fullRedraw only runs on navigation/edit, so refresh the field
  // here every frame. The cursor overlay is redrawn by the framework after
  // draw() (see screenDraw), so clearing the field area here is safe.
  if (clockSourceMirror) {
    ScreenOverlayCoordinates overlay;
    const ColorScheme cs = appSettings.colorScheme;
    gfxSetFgColor(cs.textInfo);
    gfxClearRect(13, 11, 27, 1);
    gfxPrintf(13, 11, "%u BPM", liveClockBpm());
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// Common part of the form
//

static void drawRowHeader(int row, CellState state) {}
static void drawColHeader(int col, CellState state) {}

int projectCommonColumnCount(int row) {
  if (row == 0) {
    return 5; // Load, save, new, export, manage
  } else if (row >= 1 && row <= 3) {
    return 24; // File, title, author
  } else if (row == 4) {
    return 1; // Linear pitch
  } else if (row == 5) {
    return 1; // Clock source
  } else if (row == 6) {
    return 1; // Tempo (BPM)
  }
  return 1; // Default value
}

void projectCommonDrawStatic(void) {
  const ColorScheme cs = appSettings.colorScheme;

  gfxSetFgColor(cs.textTitles);
  gfxPrint(0, 0, "PROJECT");

  gfxSetFgColor(cs.textDefault);
  gfxPrint(8, 0, appTitle);
  gfxPrintf(0, 1, "v%s (%s)", appVersion, appBuild);

  gfxPrint(0, 5, "File");
  gfxPrint(0, 6, "Title");
  gfxPrint(0, 7, "Author");

  gfxPrint(0, 9, "Linear pitch");
  gfxPrint(0, 10, "Clock source");
  gfxPrint(0, 11, "Tempo");
}

void projectCommonDrawCursor(int col, int row) {
  if (row == 0) {
    if (col == 0) {
      gfxCursor(7, 3, 4); // Load
    } else if (col == 1) {
      gfxCursor(12, 3, 4); // Save
    } else if (col == 2) {
      gfxCursor(17, 3, 3); // New
    } else if (col == 3) {
      gfxCursor(21, 3, 6); // Export
    } else if (col == 4) {
      gfxCursor(28, 3, 6); // Manage
    }
  } else if (row >= 1 && row <= 3) {
    // Text fields: file name, title, author
    gfxCursor(7 + col, 4 + row, 1);
  } else if (row == 4) {
    // Linear pitch
    gfxCursor(13, 9, 3);
  } else if (row == 5) {
    // Clock source ("Internal" / "Midi Clock")
    gfxCursor(13, 10, 10);
  } else if (row == 6) {
    // Tempo ("500.0 BPM")
    gfxCursor(13, 11, 8);
  }
}

void projectCommonDrawField(int col, int row, CellState state) {
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textValue : appSettings.colorScheme.textDefault);

  if (row == 0) {
    if (col == 0) {
      gfxPrint(7, 3, "Load");
    } else if (col == 1) {
      gfxPrint(12, 3, "Save");
    } else if (col == 2) {
      gfxPrint(17, 3, "New");
    } else if (col == 3) {
      gfxPrint(21, 3, "Export");
    } else if (col == 4) {
      gfxPrint(28, 3, "Manage");
    }
  } else if (row == 1) {
    // File name
    gfxClearRect(7, 5, FILENAME_LENGTH, 1);
    gfxPrintf(7, 5, "%s", appSettings.projectFilename);
  } else if (row == 2) {
    // Title
    gfxClearRect(7, 6, PROJECT_TITLE_LENGTH, 1);
    gfxPrintf(7, 6, "%s", chipnomadState->project.title);
  } else if (row == 3) {
    // Author
    gfxClearRect(7, 7, PROJECT_TITLE_LENGTH, 1);
    gfxPrintf(7, 7, "%s", chipnomadState->project.author);
  } else if (row == 4) {
    // Linear pitch
    gfxPrint(13, 9, chipnomadState->project.linearPitch ? "ON " : "OFF");
  } else if (row == 5) {
    // Clock source
    gfxClearRect(13, 10, 10, 1);
    gfxPrint(13, 10, clockSourceMirror ? "Midi Clock" : "Internal");
  } else if (row == 6) {
    // Tempo in BPM (greyed while slaved to MIDI clock - it's read-only
    // there, driven by the incoming clock, so the live value is shown).
    gfxClearRect(13, 11, 27, 1);
    if (clockSourceMirror) {
      gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textInfo : appSettings.colorScheme.textEmpty);
      gfxPrintf(13, 11, "%u BPM", liveClockBpm());
    } else {
      gfxPrintf(13, 11, "%u BPM", bpmValue);
    }
  }
}

int projectCommonOnEdit(int col, int row, enum CellEditAction action) {
  int handled = 0;

  if (row == 0) {
    // Load/Save/New/Export
    if (col == 0) {
      // Load project
      projectReturnScreen = &screenProject;
      if (projectModified) {
        confirmSetup("Lose unsaved changes?", doLoadProject, cancelConfirm);
        screenSetup(&screenConfirm, 0);
      } else {
        doLoadProject();
      }
    } else if (col == 1) {
      // Save project - check filename first
      if (strlen(appSettings.projectFilename) == 0) {
        screenMessage(MESSAGE_TIME, "Enter filename");
        handled = 1;
      } else {
        fileBrowserSetupFolderMode("SAVE PROJECT", appSettings.projectPath, appSettings.projectFilename, ".cct", onProjectSaved, onProjectCancelled);
        screenSetup(&screenFileBrowser, 0);
      }
    } else if (col == 2) {
      // New project
      projectReturnScreen = &screenProject;
      if (projectModified) {
        confirmSetup("Lose unsaved changes?", doNewProject, cancelConfirm);
        screenSetup(&screenConfirm, 0);
      } else {
        doNewProject();
      }
      handled = 1;
    } else if (col == 3) {
      // Export - go to export screen
      screenSetup(&screenExport, 0);
      handled = 0;
    } else if (col == 4) {
      // Manage - go to manage screen
      screenSetup(&screenManage, 0);
      handled = 0;
    }
  } else if (row == 1) {
    // File name
    int res = editCharacter(action, appSettings.projectFilename, col, FILENAME_LENGTH);
    if (res == 1) {
      isCharEdit = 1;
      editingString = appSettings.projectFilename;
      editingStringLength = FILENAME_LENGTH;
    } else if (res > 1) {
      handled = 1;
    }
  } else if (row == 2) {
    // Title
    int res = editCharacter(action, chipnomadState->project.title, col, PROJECT_TITLE_LENGTH);
    if (res == 1) {
      isCharEdit = 1;
      editingString = chipnomadState->project.title;
      editingStringLength = PROJECT_TITLE_LENGTH;
    } else if (res > 1) {
      handled = 1;
    }
  } else if (row == 3) {
    // Author
    int res = editCharacter(action, chipnomadState->project.author, col, PROJECT_TITLE_LENGTH);
    if (res == 1) {
      isCharEdit = 1;
      editingString = chipnomadState->project.author;
      editingStringLength = PROJECT_TITLE_LENGTH;
    } else if (res > 1) {
      handled = 1;
    }
  } else if (row == 4) {
    // Linear pitch (ON/OFF)
    handled = edit8noLast(action, &chipnomadState->project.linearPitch, 1, 0, 1);
    if (handled) {
      projectModified = 1;
      chipnomadQueuePlaybackStop(chipnomadState);
      reinitializePitchTable(&chipnomadState->project);
    }
  } else if (row == 5) {
    // Clock source: Internal / Midi Clock. Persisted in settings.txt
    // (appSettings.midiClockMode) and applied to the router live; the
    // audio thread picks it up through the regular project refresh.
    action = convertMultiAction(action);
    switch (action) {
      case CellEditAction::tap:
      case CellEditAction::doubleTap:
        clockSourceMirror = clockSourceMirror ? 0 : 1;
        handled = 1;
        break;
      case CellEditAction::clear:
        clockSourceMirror = 0;
        handled = 1;
        break;
      case CellEditAction::increase:
      case CellEditAction::increaseBig:
        clockSourceMirror = 1;
        handled = 1;
        break;
      case CellEditAction::decrease:
      case CellEditAction::decreaseBig:
        clockSourceMirror = 0;
        handled = 1;
        break;
      default:
        break;
    }
    if (handled) {
      appSettings.midiClockMode = clockSourceMirror;
      midiRouterSetClockMode(chipnomadState->midiRouter,
        clockSourceMirror ? MidiClockMode::slave : MidiClockMode::off);
    }
  } else if (row == 6) {
    // Tempo in whole BPM. Fine steps are 1 BPM, coarse 5 BPM; clear
    // restores the old default 50 Hz = 125 BPM. While slaved to MIDI clock
    // the tempo is driven by the incoming clock, so edits are swallowed
    // (the field shows the live followed tempo).
    if (clockSourceMirror) {
      handled = 1;
    } else {
      action = convertMultiAction(action);
      switch (action) {
        case CellEditAction::tap:
        case CellEditAction::doubleTap:
          bpmValue += 1;
          if (bpmValue > BPM_MAX) bpmValue = BPM_MIN;
          handled = 1;
          break;
        case CellEditAction::clear:
          bpmValue = BPM_DEFAULT;
          handled = 1;
          break;
        case CellEditAction::increase:
          if (bpmValue < BPM_MAX) bpmValue += 1;
          handled = 1;
          break;
        case CellEditAction::decrease:
          if (bpmValue > BPM_MIN) bpmValue -= 1;
          handled = 1;
          break;
        case CellEditAction::increaseBig:
          bpmValue = bpmValue > BPM_MAX - 5 ? BPM_MAX : bpmValue + 5;
          handled = 1;
          break;
        case CellEditAction::decreaseBig:
          bpmValue = bpmValue < BPM_MIN + 5 ? BPM_MIN : bpmValue - 5;
          handled = 1;
          break;
        default:
          break;
      }
      if (handled) {
        // Backwards compatible writeback: the project file keeps storing
        // Hz ("- Frame rate"), BPM * 0.4 in Hz.
        chipnomadState->project.tickRate = (float)bpmValue * 0.4f;
        projectModified = 1;
      }
    }
  }

  return handled;
}


///////////////////////////////////////////////////////////////////////////////
//
// Input handling
//

static int inputScreenNavigation(int keys, int tapCount) {
  if (keys == keyOpt || keys == (keyDown | keyShift)) {
    screenSetup(&screenSong, 0);
    return 1;
  }
  return 0;
}

static int onInput(int isKeyDown, int keys, int tapCount) {
  if (isCharEdit) {
    ScreenData* screen = projectScreen();
    char result = charEditInput(keys, tapCount, editingString, screen->cursorCol, editingStringLength);

    if (result) {
      projectModified = 1;
      isCharEdit = 0;
      if (screen->cursorCol < editingStringLength - 1) screen->cursorCol++;
      editingString = NULL;
      editingStringLength = 0;
      fullRedraw();
    }
  } else {
    if (inputScreenNavigation(keys, tapCount)) return 1;

    ScreenData* screen = projectScreen();
    if (screenInput(screen, isKeyDown, keys, tapCount)) return 1;
  }
  return 0;
}

#ifdef DESKTOP_BUILD

// Key jazz text entry (see keyJazzTextHandleRawKey): filename, title, author.
int projectKeyJazzTextField(KeyJazzTextField* field) {
  ScreenData* screen = projectScreen();
  field->screen = screen;
  field->row = screen->cursorRow;
  field->popupOpen = isCharEdit;
  field->marksProjectModified = 1;
  field->str = NULL;
  if (screen->cursorRow == 1) { field->str = appSettings.projectFilename; field->maxLen = FILENAME_LENGTH; }
  else if (screen->cursorRow == 2) { field->str = chipnomadState->project.title; field->maxLen = PROJECT_TITLE_LENGTH; }
  else if (screen->cursorRow == 3) { field->str = chipnomadState->project.author; field->maxLen = PROJECT_TITLE_LENGTH; }
  return 1;
}

#endif // DESKTOP_BUILD

static ScreenPlaybackLevel getPlaybackLevel(void) {
  return ScreenPlaybackLevel::song;
}

const AppScreen screenProject = {
  .init = NULL,
  .setup = setup,
  .fullRedraw = fullRedraw,
  .draw = draw,
  .onInput = onInput,
  .getPlaybackLevel = getPlaybackLevel,
};
