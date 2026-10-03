#include "screens.h"
#include "common.h"
#include "corelib_gfx.h"
#include "corelib/corelib_file.h"
#include "chipnomad_lib.h"
#include "screen_instrument.h"
#include "waveform_display.h"
#include "synth/sample_voice.h"
#include <stdio.h>
#include <string.h>

static constexpr int valueX = 9;
static constexpr int valueWidth = 7;
static constexpr int previewRow = 4;
static constexpr int previewWidth = 32;
static constexpr int previewHeight = 7;
static constexpr int viewReadoutRow = 11;
static constexpr int fieldRow0 = 12;
static const char* sliceLabels[] = {"Off", "2", "4", "8", "16", "32"};
static const uint8_t sliceValues[] = {0, 2, 4, 8, 16, 32};
static constexpr int sliceCount = 6;

static Bitmap* samplePreviewBitmap;
static Bitmap* sampleSliceMarkerBitmap;
static Bitmap* sampleStartMarkerBitmap;
static Bitmap* sampleEndMarkerBitmap;
static Bitmap* sampleSelectionBitmap;

static int sliceToIndex(uint8_t slice) {
  for (int i = 1; i < sliceCount; ++i) {
    if (sliceValues[i] == slice) return i;
  }
  return 0;
}

static const char* sampleFilename(const char* path) {
  const char* separator = strrchr(path, PATH_SEPARATOR);
  return separator ? separator + 1 : path;
}

static const char* shortSampleFilename(const char* path, size_t maxLength) {
  const char* name = sampleFilename(path);
  size_t length = strlen(name);
  return length > maxLength ? name + length - maxLength : name;
}

static InstrumentSample* currentSample(void) {
  return &chipnomadState->project.instruments[cInstrument].chip.sample;
}

static Bitmap* ensurePreviewBitmap(Bitmap** bitmap) {
  if (*bitmap && ((*bitmap)->widthChars != previewWidth || (*bitmap)->heightChars != previewHeight)) {
    gfxBitmapFree(*bitmap);
    *bitmap = NULL;
  }
  if (!*bitmap) *bitmap = gfxBitmapCreate(previewWidth, previewHeight);
  return *bitmap;
}

static uint8_t sampleSliceDivisions(const InstrumentSample* sample) {
  return sampleNormalizeSlice(sample->slice);
}

// Which marker the view window follows: 0 none, 1 Start, 2 End,
// 3/4 the selection handles.
enum {
  kViewAnchorNone = 0,
  kViewAnchorStart = 1,
  kViewAnchorEnd = 2,
  kViewAnchorSelStart = 3,
  kViewAnchorSelEnd = 4,
};

// View window into the sample: viewStart is the first visible frame,
// viewEnd one past the last visible frame (<= frameCount).
struct SampleEditorView {
  uint32_t viewStart;
  uint32_t viewEnd;
  int anchor;
};

static SampleEditorView editorView;

// Processing selection: the region the process tools (editor phase 2) act
// on. Independent from the playback Start/End markers and stored in frames,
// not 0-255 normalized values. Session-only editor state: never saved with
// the project, reset when the screen is entered.
struct SampleEditorSelection {
  uint32_t start;  // inclusive frame
  uint32_t end;    // exclusive frame; start == end => empty
  uint8_t active;  // 0 = empty/none
};

static SampleEditorSelection editorSelection;

// Smallest zoomed window in frames
static constexpr uint32_t kMinViewSpan = 8;

// Frame position of the view anchor: a playback marker (1 = Start, 2 = End,
// before the reverse-order swap - the zoom anchor follows the marker itself,
// not the active region) or a selection handle (3/4).
static uint32_t anchorFrame(const InstrumentSample* sample,
                            const SampleEditorSelection* selection, int anchor) {
  switch (anchor) {
    case kViewAnchorStart: return sampleMarkerToStartFrame(sample->frameCount, sample->start);
    case kViewAnchorEnd: return sampleMarkerToEndFrame(sample->frameCount, sample->end);
    case kViewAnchorSelStart: return selection->start;
    case kViewAnchorSelEnd: return selection->end;
    default: return 0;
  }
}

static void zoomOutFull(const InstrumentSample* sample, SampleEditorView* view) {
  view->viewStart = 0;
  view->viewEnd = sample->frameCount;
  view->anchor = kViewAnchorNone;
}

// Maps an absolute frame to a pixel column inside the view window. Returns
// -1 when the frame is outside the window. A frame exactly on viewEnd pins
// to the last column only when pinToEnd is set (the end marker is an
// exclusive bound drawn at its limit).
static int frameToPixel(uint32_t frame, const SampleEditorView* view, int width, int pinToEnd) {
  if (width <= 0 || view->viewEnd <= view->viewStart) return -1;
  if (frame < view->viewStart || frame > view->viewEnd) return -1;
  if (frame == view->viewEnd) return pinToEnd ? width - 1 : -1;
  int x = (int)((uint64_t)(frame - view->viewStart) * width / (view->viewEnd - view->viewStart));
  return x < width ? x : width - 1;
}

// Zooms in one notch around markerFrame, keeping the marker at its relative
// position inside the window (the first zoom from the full view centers it).
// When the marker would sit at a window edge it pans just enough to keep a
// small margin, so repeated fine steps follow the marker without drift.
// The span floors at the marker quantum (one fine step moves a marker by
// roughly frameCount/255 frames), so the window always shows the frames the
// fine steps actually cross.
static void zoomToMarker(const InstrumentSample* sample, SampleEditorView* view,
                         uint32_t markerFrame) {
  const uint32_t frameCount = sample->frameCount;
  if (frameCount == 0 || sample->data == NULL) {
    zoomOutFull(sample, view);
    return;
  }

  const uint32_t span = view->viewEnd - view->viewStart;
  uint32_t minSpan = frameCount / 255 + 1;
  if (minSpan < kMinViewSpan) minSpan = kMinViewSpan;
  uint32_t newSpan = span / 2;
  if (newSpan < minSpan) newSpan = minSpan;
  if (newSpan > frameCount) newSpan = frameCount;

  uint32_t viewStart;
  const int fullView = span == 0 || (view->viewStart == 0 && view->viewEnd == frameCount);
  if (fullView || markerFrame < view->viewStart || markerFrame > view->viewEnd) {
    // First zoom-in (or the marker left the window): center on the marker
    viewStart = markerFrame >= newSpan / 2 ? markerFrame - newSpan / 2 : 0;
  } else {
    // Keep the marker at its relative position inside the window
    const uint64_t relative = (uint64_t)(markerFrame - view->viewStart) * newSpan / span;
    viewStart = markerFrame >= relative ? markerFrame - (uint32_t)relative : 0;
  }

  // Pan so the marker stays visible with a margin at the hit edge
  const uint32_t margin = newSpan / 8 > 0 ? newSpan / 8 : 1;
  if (markerFrame < viewStart + margin) {
    viewStart = markerFrame >= margin ? markerFrame - margin : 0;
  } else if ((uint64_t)markerFrame + margin > (uint64_t)viewStart + newSpan) {
    viewStart = (uint64_t)markerFrame + margin > newSpan
      ? (uint32_t)((uint64_t)markerFrame + margin - newSpan) : 0;
  }

  if (viewStart + newSpan > frameCount) viewStart = frameCount - newSpan;
  view->viewStart = viewStart;
  view->viewEnd = viewStart + newSpan;
}

static void updateSamplePreview(const InstrumentSample* sample, const SampleEditorView* view,
                                const SampleEditorSelection* selection) {
  Bitmap* waveform = ensurePreviewBitmap(&samplePreviewBitmap);
  Bitmap* markers = ensurePreviewBitmap(&sampleSliceMarkerBitmap);
  Bitmap* startMarker = ensurePreviewBitmap(&sampleStartMarkerBitmap);
  Bitmap* endMarker = ensurePreviewBitmap(&sampleEndMarkerBitmap);
  Bitmap* selectionBand = ensurePreviewBitmap(&sampleSelectionBitmap);
  if (!waveform || !markers || !startMarker || !endMarker || !selectionBand) return;

  // Calculate actual start/end positions in frames
  uint32_t frameCount = sample->frameCount;
  uint32_t startFrame = sampleMarkerToStartFrame(frameCount, sample->start);
  uint32_t endFrame = sampleMarkerToEndFrame(frameCount, sample->end);
  if (startFrame > endFrame) { uint32_t swap = startFrame; startFrame = endFrame; endFrame = swap + 1; }

  // Determine if we should use start/end or full range
  uint8_t slices = sampleSliceDivisions(sample);

  // Render only the view window; markers and greying map through it
  renderPCM16Preview(waveform, sample->data, view->viewStart, view->viewEnd, sample->channels);

  int width = waveform->widthPixels;
  int height = waveform->heightPixels;
  uint32_t viewSpan = view->viewEnd - view->viewStart;

  // Create start marker bitmap (single vertical line); skipped when the
  // marker sits outside the view window
  gfxBitmapClear(startMarker);
  {
    int startX = frameToPixel(startFrame, view, width, 0);
    if (startX >= 0) {
      for (int y = 0; y < height; y++) {
        startMarker->data[y * width + startX] = 255;
      }
    }
  }
  // Create end marker bitmap (single vertical line). It is an exclusive
  // bound, so a marker on the window's right edge pins to the last column.
  gfxBitmapClear(endMarker);
  {
    int endX = frameToPixel(endFrame, view, width, 1);
    if (endX >= 0) {
      for (int y = 0; y < height; y++) {
        endMarker->data[y * width + endX] = 255;
      }
    }
  }
  // Adjust waveform brightness
  // - Active area (between start/end): waveform at 255 (light blue)
  // - Inactive area (before start, after end): ONLY waveform pixels greyed to 48, background stays 0
  // A column is active when its frame range intersects [startFrame, endFrame);
  // an empty region (Start == End) leaves every column inactive.
  {
    for (int x = 0; x < width; x++) {
      uint32_t columnStart = view->viewStart + (uint64_t)x * viewSpan / width;
      uint32_t columnEnd = view->viewStart + (uint64_t)(x + 1) * viewSpan / width;
      if (columnEnd > view->viewEnd) columnEnd = view->viewEnd;
      if (columnEnd > startFrame && columnStart < endFrame && endFrame > startFrame) continue;
      for (int y = 0; y < height; y++) {
        if (waveform->data[y * width + x] == 255) {
          // Inactive waveform pixel: dark gray
          waveform->data[y * width + x] = 48;
        }
        // Active waveform pixels stay at 255 (light blue)
        // Background pixels (0) stay at 0 in both areas
      }
    }
  }
  // Create slice markers
  gfxBitmapClear(markers);
  if (slices) {
    // When in slice mode, slice markers divide the loop region (start to end)
    // This ensures slices follow the start/end markers
    uint32_t loopLength = endFrame > startFrame ? (endFrame - startFrame) : frameCount;
    if (loopLength == 0) loopLength = frameCount;

    // Draw slice markers within the loop region; markers outside the view
    // window are skipped cleanly
    for (int i = 1; i < slices; ++i) {
      uint32_t position = startFrame + (uint32_t)((uint64_t)loopLength * i / slices);
      int x = frameToPixel(position, view, width, 0);
      if (x < 0) continue;
      for (int y = 0; y < height; ++y) markers->data[y * width + x] = 255;
    }
  }
  // Selection band + handles: dim fill over columns overlapping
  // [selection->start, selection->end), bright lines at the handles.
  // Positions map through the view window; handles outside are skipped.
  gfxBitmapClear(selectionBand);
  if (selection->active) {
    for (int x = 0; x < width; x++) {
      uint32_t columnStart = view->viewStart + (uint64_t)x * viewSpan / width;
      uint32_t columnEnd = view->viewStart + (uint64_t)(x + 1) * viewSpan / width;
      if (columnEnd > view->viewEnd) columnEnd = view->viewEnd;
      if (columnEnd <= selection->start || columnStart >= selection->end) continue;
      for (int y = 0; y < height; y++) selectionBand->data[y * width + x] = 96;
    }
    int selStartX = frameToPixel(selection->start, view, width, 0);
    if (selStartX >= 0) {
      for (int y = 0; y < height; y++) selectionBand->data[y * width + selStartX] = 255;
    }
    int selEndX = frameToPixel(selection->end, view, width, 1);
    if (selEndX >= 0) {
      for (int y = 0; y < height; y++) selectionBand->data[y * width + selEndX] = 255;
    }
  }
}

static void drawSamplePreview(void) {
  // Clear the waveform area and space for frame
  gfxClearRect(0, previewRow, previewWidth, previewHeight);
  // Draw the waveform with light blue color for active area
  if (samplePreviewBitmap) {
    gfxSetFgColor(0xADD8E6); // Light blue
    gfxDrawBitmap(samplePreviewBitmap, 0, previewRow);
  }
  // Draw the selection band + handles (scheme info color, distinct from the
  // yellow/orange playback markers)
  if (sampleSelectionBitmap) {
    gfxSetFgColor(appSettings.colorScheme.textInfo);
    gfxDrawBitmap(sampleSelectionBitmap, 0, previewRow);
  }
  // Draw start marker (yellow) - always shown
  if (sampleStartMarkerBitmap) {
    gfxSetFgColor(0xFFFF00); // Yellow
    gfxDrawBitmap(sampleStartMarkerBitmap, 0, previewRow);
  }
  // Draw end marker (orange) - always shown
  if (sampleEndMarkerBitmap) {
    gfxSetFgColor(0xFFA500); // Orange
    gfxDrawBitmap(sampleEndMarkerBitmap, 0, previewRow);
  }
  // Draw the slice markers on top
  if (sampleSliceMarkerBitmap) {
    gfxSetFgColor(appSettings.colorScheme.textDefault);
    gfxDrawBitmap(sampleSliceMarkerBitmap, 0, previewRow);
  }
}

static int settingsColumnCount(int row) {
  (void)row;
  return 1;
}

// Zoom indicator below the waveform: 1:1 when the whole sample is visible,
// otherwise the visible span in frames
static void drawViewReadout(void) {
  gfxSetFgColor(appSettings.colorScheme.textInfo);
  gfxClearRect(0, viewReadoutRow, 40, 1);
  const InstrumentSample* sample = currentSample();
  if (editorView.viewStart == 0 && editorView.viewEnd == sample->frameCount) {
    gfxPrint(0, viewReadoutRow, "VIEW 1:1");
  } else {
    char text[32];
    snprintf(text, sizeof(text), "ZOOM %u fr", (unsigned)(editorView.viewEnd - editorView.viewStart));
    gfxPrint(0, viewReadoutRow, text);
  }
}

static void settingsDrawStatic(void) {
  const ColorScheme cs = appSettings.colorScheme;
  InstrumentSample* sample = currentSample();
  gfxSetFgColor(cs.textTitles);
  gfxPrint(0, 0, "SAMPLE EDIT");
  gfxSetFgColor(cs.textDefault);
  gfxPrint(0, 1, shortSampleFilename(sample->path, 32));
  gfxSetFgColor(cs.textInfo);
  if (sample->data && sample->frameCount) {
    char formatText[40];
    snprintf(formatText, sizeof(formatText), "%u Hz %s %u fr", (unsigned)sample->sampleRate,
             sample->channels >= 2 ? "STEREO" : "MONO", (unsigned)sample->frameCount);
    gfxPrint(0, 2, formatText);
  }
  updateSamplePreview(sample, &editorView, &editorSelection);
  drawSamplePreview();
  drawViewReadout();
  gfxSetFgColor(cs.textDefault);
  gfxPrint(0, fieldRow0, "Start");
  gfxPrint(0, fieldRow0 + 1, "End");
  gfxPrint(0, fieldRow0 + 2, "Sel.S");
  gfxPrint(0, fieldRow0 + 3, "Sel.E");
  gfxPrint(0, fieldRow0 + 4, "Slice");
}

static void settingsDrawCursor(int col, int row) {
  (void)col;
  gfxCursor(valueX, fieldRow0 + row, row == 4 ? 3 : valueWidth);
}

static void settingsDrawRowHeader(int row, CellState state) {
  (void)row;
  (void)state;
}

static void settingsDrawColHeader(int col, CellState state) {
  (void)col;
  (void)state;
}

static void settingsDrawField(int col, int row, CellState state) {
  (void)col;
  InstrumentSample* sample = currentSample();
  gfxSetFgColor(state == CellState::focus ? appSettings.colorScheme.textValue : appSettings.colorScheme.textDefault);
  gfxClearRect(valueX, fieldRow0 + row, valueWidth, 1);
  if (row == 0) gfxPrint(valueX, fieldRow0, byteToHex(sample->start));
  else if (row == 1) gfxPrint(valueX, fieldRow0 + 1, byteToHex(sample->end));
  else if (row == 2 || row == 3) {
    // Selection handle readout: absolute frame, '-' when the selection is
    // empty (both handles show '-')
    if (!editorSelection.active) {
      gfxPrint(valueX, fieldRow0 + row, "-");
    } else {
      uint32_t frame = row == 2 ? editorSelection.start : editorSelection.end;
      char text[16];
      snprintf(text, sizeof(text), "%06u", (unsigned)frame);
      gfxPrint(valueX, fieldRow0 + row, text);
    }
  } else if (row == 4) {
    // Slice is inert while Stretch drives the duration: dim it.
    if (sample->stretchMode != 0) gfxSetFgColor(appSettings.colorScheme.textEmpty);
    gfxPrint(valueX, fieldRow0 + 4, sliceLabels[sliceToIndex(sample->slice)]);
  }
}

// Clamp the selection to the sample, swap inverted handles and update the
// active flag; clamp the view window and re-anchor it if its anchor marker
// vanished. Repaints the preview. Shared with the process tools (phase 2).
static void sampleEditorNormalizeState(const InstrumentSample* sample,
                                       SampleEditorSelection* selection,
                                       SampleEditorView* view) {
  const uint32_t frameCount = sample->frameCount;
  if (selection->start > frameCount) selection->start = frameCount;
  if (selection->end > frameCount) selection->end = frameCount;
  if (selection->start > selection->end) {
    uint32_t swap = selection->start;
    selection->start = selection->end;
    selection->end = swap;
  }
  selection->active = selection->start < selection->end;

  if (frameCount == 0) {
    zoomOutFull(sample, view);
  } else {
    if (view->viewEnd > frameCount) view->viewEnd = frameCount;
    if (view->viewStart >= view->viewEnd) zoomOutFull(sample, view);
    if (view->anchor != kViewAnchorNone) {
      // Drop the anchor when its marker no longer exists (empty selection,
      // or a playback marker that fell outside the sample)
      uint32_t frame = anchorFrame(sample, selection, view->anchor);
      if (view->anchor >= kViewAnchorSelStart && !selection->active) {
        view->anchor = kViewAnchorNone;
      } else if (frame < view->viewStart || frame > view->viewEnd) {
        view->anchor = kViewAnchorNone;
      }
    }
  }
  updateSamplePreview(sample, view, selection);
}

static int settingsOnEdit(int col, int row, CellEditAction action) {
  (void)col;
  InstrumentSample* sample = currentSample();
  int handled = 0;
  int marker = 0; // 1 = Start, 2 = End
  if (row == 0) {
    handled = edit8noLast(action, &sample->start, 16, 0, 255);
    marker = kViewAnchorStart;
  } else if (row == 1) {
    handled = edit8noLast(action, &sample->end, 16, 0, 255);
    marker = kViewAnchorEnd;
  } else if (row == 2 || row == 3) {
    // Selection handles: fine steps move one frame and zoom onto the
    // handle; coarse steps jump frameCount/64 (min 16) and return to the
    // full-sample view. Tap copies the matching playback marker position;
    // clear empties the whole selection. Start/End are untouched.
    const uint32_t frameCount = sample->frameCount;
    if (frameCount == 0) return 0;
    uint32_t* handle = row == 2 ? &editorSelection.start : &editorSelection.end;
    if (action == CellEditAction::tap) {
      uint32_t markerPos = row == 2 ? sampleMarkerToStartFrame(frameCount, sample->start)
                                    : sampleMarkerToEndFrame(frameCount, sample->end);
      if (editorSelection.active && *handle == markerPos) return 0;
      *handle = markerPos;
      handled = 1;
    } else if (action == CellEditAction::clear) {
      if (!editorSelection.active && editorSelection.start == 0 && editorSelection.end == 0) return 0;
      editorSelection.start = 0;
      editorSelection.end = 0;
      handled = 1;
    } else {
      // Fine steps move one frame; coarse steps jump frameCount/64 (min 16)
      uint32_t step = 1;
      if (action == CellEditAction::increaseBig || action == CellEditAction::decreaseBig) {
        step = frameCount / 64;
        if (step < 16) step = 16;
      }
      if (action == CellEditAction::increase || action == CellEditAction::increaseBig) {
        uint64_t next = (uint64_t)*handle + step;
        if (next > frameCount) next = frameCount;
        if (next == *handle) return 0;
        *handle = (uint32_t)next;
      } else if (action == CellEditAction::decrease || action == CellEditAction::decreaseBig) {
        uint32_t next = *handle > step ? *handle - step : 0;
        if (next == *handle) return 0;
        *handle = next;
      } else {
        return 0;
      }
      handled = 1;
    }
    if (handled) {
      if (action == CellEditAction::increase || action == CellEditAction::decrease) {
        editorView.anchor = row == 2 ? kViewAnchorSelStart : kViewAnchorSelEnd;
        zoomToMarker(sample, &editorView, *handle);
      } else if (action == CellEditAction::increaseBig || action == CellEditAction::decreaseBig ||
                 action == CellEditAction::clear) {
        zoomOutFull(sample, &editorView);
      }
      sampleEditorNormalizeState(sample, &editorSelection, &editorView);
      drawSamplePreview();
      drawViewReadout();
      // Repaint both handle readouts: normalization may have swapped them
      settingsDrawField(0, 2, CellState::focus);
      settingsDrawField(0, 3, CellState::focus);
    }
    return handled;
  } else if (row == 4) {
    // Slice is inert while Stretch drives the duration.
    if (sample->stretchMode != 0) return 0;
    uint8_t index = (uint8_t)sliceToIndex(sample->slice);
    handled = edit8noLast(action, &index, 1, 0, sliceCount - 1);
    if (handled) {
      sample->slice = sliceValues[index];
      // Stretch and Slice are mutually exclusive: enabling one disables the other.
      if (sample->slice) sample->stretchMode = 0;
      projectModified = 1;
      updateSamplePreview(sample, &editorView, &editorSelection);
      drawSamplePreview();
    }
    return handled;
  }
  if (handled) {
    projectModified = 1;
    // Fine steps zoom onto the edited marker so the waveform shows exactly
    // what is being adjusted; coarse steps (and clear) return to the
    // full-sample view.
    if (action == CellEditAction::increase || action == CellEditAction::decrease) {
      editorView.anchor = marker;
      zoomToMarker(sample, &editorView, anchorFrame(sample, &editorSelection, marker));
    } else if (action == CellEditAction::increaseBig || action == CellEditAction::decreaseBig ||
               action == CellEditAction::clear) {
      zoomOutFull(sample, &editorView);
    }
    updateSamplePreview(sample, &editorView, &editorSelection);
    drawSamplePreview();
    drawViewReadout();
  }
  return handled;
}

// Slice is inert while Stretch drives the duration: skip it in navigation.
static int settingsIsCellValid(int col, int row) {
  (void)col;
  if (row == 4 && currentSample()->stretchMode != 0) return 0;
  return 1;
}

static ScreenData screenSampleSettingsData = {
  .rows = 5,
  .cursorRow = 0,
  .cursorCol = 0,
  .topRow = 0,
  .selectMode = -1,
  .selectStartRow = 0,
  .selectStartCol = 0,
  .selectAnchorRow = 0,
  .selectAnchorCol = 0,
  .playbackLevel = ScreenPlaybackLevel::none,
  .getColumnCount = settingsColumnCount,
  .drawStatic = settingsDrawStatic,
  .drawCursor = settingsDrawCursor,
  .drawSelection = NULL,
  .drawRowHeader = settingsDrawRowHeader,
  .drawColHeader = settingsDrawColHeader,
  .drawField = settingsDrawField,
  .onEdit = settingsOnEdit,
  .onInput = NULL,
  .onRawInput = NULL,
  .isCellValid = settingsIsCellValid,
  .getLoopRange = NULL,
};

static void setup(int input) {
  if (input != -1) cInstrument = input;
  // Entering the screen always starts at the full-sample view with an
  // empty selection: both are session-only editor state
  zoomOutFull(currentSample(), &editorView);
  editorSelection.start = 0;
  editorSelection.end = 0;
  editorSelection.active = 0;
}

static void fullRedraw(void) {
  // The cursor persists across screens: if it is parked on Slice while Stretch
  // is active, move it up so it never rests on a disabled cell.
  if (screenSampleSettingsData.cursorRow == 4 && currentSample()->stretchMode != 0) {
    screenSampleSettingsData.cursorRow = 1;
  }
  screenFullRedraw(&screenSampleSettingsData);
}

static void draw(void) {
}

static int inputScreenNavigation(int keys) {
  if (keys == keyOpt || keys == (keyLeft | keyShift)) {
    screenSetup(&screenInstrument, cInstrument);
    return 1;
  }
  if (keys == (keyRight | keyShift)) {
    screenSetup(&screenTable, cInstrument);
    return 1;
  }
  if (keys == (keyDown | keyShift)) {
    screenSetup(&screenInstrumentPool, cInstrument);
    return 1;
  }
  if (keys == (keyUp | keyShift)) {
    screenSetup(&screenModulation, cInstrument);
    return 1;
  }
  return 0;
}

static int onInput(int isKeyDown, int keys, int tapCount) {
  if (inputScreenNavigation(keys)) return 1;
  return screenInput(&screenSampleSettingsData, isKeyDown, keys, tapCount);
}

static ScreenPlaybackLevel getPlaybackLevel(void) {
  return ScreenPlaybackLevel::phrase;
}

const AppScreen screenSampleSettings = {
  .init = NULL,
  .setup = setup,
  .fullRedraw = fullRedraw,
  .draw = draw,
  .onInput = onInput,
  .getPlaybackLevel = getPlaybackLevel,
};
