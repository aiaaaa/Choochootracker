# UI architecture, style, and rules

Read this before adding or changing screens, popups, instrument forms,
colors, fonts, navigation, or anything drawn with `gfxPrint`.

The tracker UI is a 40x20 character spreadsheet, not a widget tree.

## Source of truth

| Piece | Path |
| --- | --- |
| Screen registry / map | `tracker/src/screens/screens.h`, `screens.cpp` |
| App dispatch | `tracker/src/app.cpp` |
| Grid renderer | `tracker/src/corelib/corelib_gfx.h`, `tracker/platforms/sdl2/corelib_gfx.cpp` |
| Colors / settings | `tracker/src/common.h` `ColorScheme` |
| Instrument shell | `tracker/src/screens/screen_instrument.cpp`, `screen_instrument.h` |
| Per-engine forms | `tracker/src/screens/instrument_*.cpp` |
| Hierarchical popup | `tracker/src/screens/selection_popup.*` |
| Spreadsheet edit helpers | `tracker/src/screens/edit_common.cpp`, `edit_fx.cpp` |
| ASCII mockups | `docs/designbook/README.md` |

User-facing control names: `docs/USER_MANUAL.md`. Never update in-app help
(`screen_quick_help`) unless the task says so. AGENTS.md: update
`docs/USER_MANUAL.md` before an alpha commit.

## Grid contract

All `gfxPrint` / `gfxCursor` / `gfxClearRect` coordinates are characters.
Assume 40 columns x 20 rows.

```text
cols  0                                33 34     39
row 0 TITLE
rows 1-18  screen body
row 19     message line                     map
```

Right strip (cols 34-39):

- rows 0-2 around the map: R/P/G/M extras
- row 1 of map: `MSCPIT`
- rows 3-10: per-track M/S, track digit, waveform, note
- row 19 col 39: motion indicator `~` live, `*` record, `x` erase, `!` overflow

`drawScreenMap()` always runs except on the title screen. Leave cols 34-39
alone unless you are the map, the track sidebar in `appDraw`, or Mixer CLIP
which is still left of that strip.

Row 19 is `screenMessage`. Timed messages use `MESSAGE_TIME` (60 frames).

Title screen is a pixel overlay (`gfxTitleBegin/End`), not the text grid.

## Screen objects

Two layers:

`AppScreen` — lifecycle:

- `init`, `setup(input)`, `fullRedraw`, `draw`, `onInput`, `getPlaybackLevel`

`ScreenData` — optional spreadsheet:

- cursor, scroll (`topRow`, 16 visible rows), selection
- `drawStatic`, `drawField`, `drawCursor`, `onEdit`
- `getColumnCount(row)`, optional `isCellValid`, `getLoopRange`

Switch screens with `screenSetup(&screenX, input)`. The switch is deferred to
the next `screenDraw` so input handlers do not redraw mid-dispatch.

`screensInitAll()` currently inits title, song, chain, phrase, table,
AY wavetable, instrument, groove, and copy buffers. If you add persistent
cursor state, init it there.

Playback level (`none` / `song` / `chain` / `phrase`) decides what Play does
when the screen does not handle it.

## Navigation map

SELECT + direction (`keyShift | keyUp/Down/Left/Right`) moves between screens.
This is the LSDJ-like `MSCPIT` layout:

```text
Rvrb    Proj           Groov    Modulations
Mixer - Song - Chain - Phrase - Instrument - Table
Dlay    Sett                    Pool         Wvtbl
```

Implement navigation in each screen's `inputScreenNavigation`, not globally.

| From | SELECT+Left | SELECT+Right | SELECT+Up | SELECT+Down |
| --- | --- | --- | --- | --- |
| Mixer | — | Song | Reverb page | Delay page |
| Song | Mixer | Chain (needs chain) | Project | Settings |
| Chain | Song | Phrase | — | — |
| Phrase | Chain | Instrument or Table if TBL/TBX | Groove | — |
| Instrument | Phrase | Table | Modulation | Pool |
| Table | Phrase | — | — | AY Wavetable |

Mixer pages 0/1/2 are the same `AppScreen` (`screenMixerGetPage`). Map letters:
M / R / D.

OPT + direction is screen-local (prev/next track, etc.). Do not steal
SELECT+direction for local chores.

## Logical controls

| Logical | Handheld | Role |
| --- | --- | --- |
| D-pad | D-pad | Move |
| EDIT | A | Enter / fine+coarse with directions |
| OPT | B | Option, copy, screen-local |
| PLAY | Start | Play/stop |
| SHIFT | Select | Screen map, selection |
| LIVE | L1 | Joystick modulation enable |
| RECORD | L2 | Motion record |
| ERASE | R2 | Motion erase |

EDIT+LEFT/RIGHT = fine. EDIT+UP/DOWN = coarse (`increaseBig` / `decreaseBig`).
OPT+EDIT = clear (`CellEditAction::clear`). Double-tap EDIT creates next free
chain/phrase/instrument where the screen supports it.

Selection (spreadsheet screens only): SELECT+OPT enters select. OPT copy,
OPT+EDIT cut, SELECT+EDIT paste. `selectMode = -1` disables selection
(Instrument, Mixer, Settings).

Do not add mouse-only or keyboard-only commands as the only path. Desktop
keys must map to the same logical buttons.

## Spreadsheet vs form

Song, Chain, Phrase, Table, Groove, Pool are spreadsheets. Use `ScreenData`
and `edit8withLimit` / `editFX` helpers.

Instrument, Mixer, Project, Settings, Modulation are forms: labelled fields,
section titles, no selection. Instrument still uses `ScreenData` with
per-row column counts.

When adding a form:

- Title in `textTitles` at (0,0).
- Labels in `textDefault`. Values in `textValue` when focused, else
  `textDefault`. Empty/off cells `textEmpty`.
- Brackets in mockups are illustration; code draws labels and values at
  fixed columns, cursor via `gfxCursor(x,y,w)`.
- Keep coordinates fixed even if a field is hidden. Do not pack remaining
  fields up. Muscle memory matters (`docs/designbook/README.md`).

## Instrument screens

`screenInstrument` picks a `ScreenData` from `InstrumentScreenKind`.

Shared header (`instrumentCommon*`):

```text
INSTRUMENT 00
Type    [name    ]       Load  Save
Name    [               ]
Transp. [On ]   Tbl.Tic [01]   Vol [FF]
```

Shared voice-post block on the right for modern engines:

- FILTER Type (Off/Clean/Classic/Aggro/Acid), Mode LP/BP/HP, Slope 12/24,
  Cutoff Hz, Reso
- ADSR A D S R Shape on row 14, envelope bitmap rows 15-17
- live waveform rows 16-18 from `voiceMonitors`

Engine-specific parameters stay on the left. Follow AY's two-column section
style. Do not invent a third instrument interaction language.

Type popup uses `selection_popup` with CHIP / DRUMS / SAMPLE / SYNTH.
Braids models and Plaits engines use the same two-panel popup. Every model
belongs in exactly one category. `MISC` is fallback only.

Quick cycle on Type (EDIT+left/right) uses `instrumentTypesQuickCycle`.
Display order is not `InstrumentType` numeric order. Serialized IDs stay put.

Load/Save sit in the header. File browser is a modal `AppScreen`.

## Color and type

`ColorScheme` slots only:

- `background`, `textEmpty`, `textInfo`, `textDefault`, `textValue`
- `textTitles`, `playMarkers`, `cursor`, `selection`, `warning`

Default theme is dark blue/sand (`initDefaultAppSettings`). Themes are `.cth`.
Do not hardcode extra RGB except through the scheme. Warnings (clip, pitch
conflict, overflow) use `warning`.

Hex for 8-bit tracker values (`byteToHex`). Notes via `noteName()`. Cutoff
prints as `N Hz`. On/off as `On `/`Off` or `*`/`-` in Mixer.

Bitmap font, not TTF, at runtime. Custom `.cnfont` allowed. Character cells
are the layout unit; pixel overlays are title/HUD only.

## Edit helpers

Reuse, do not fork:

- `edit8withLimit`, `edit8noLast`, `editNormalized8`, `editFilterCutoff`
- `editFX` / `editFXValue` (instrument-aware FX list)
- `editCharacter` for names
- `applyMultiEdit` for selections
- `popupEditInput` for hold-to-open popups

Cutoff: 20 Hz–20 kHz, exponential. Instrument volume 00-FF independent of
track fader 00-100.

FX selector shows universal FX plus the active instrument group. Scrolling
FX names uses the same filter.

## Popups and modals

Modals are full `AppScreen`s: confirm, file browser, enter name, create
folder, selection popup, export, key mapping, color theme, quick help.

Confirm via `confirmSetup(message, ok, cancel)` then `screenSetup(&screenConfirm, 0)`.
Callbacks must `screenSetup` back.

Selection popup: left category, right items, EDIT confirm, OPT cancel.
Current item marked. Used for instrument type, Braids model, Plaits engine,
modulation destination.

## Drawing rules

- `fullRedraw` paints static labels + all fields + cursor. `draw` is the
  cheap per-frame path (CPU meter, live waveform).
- Clear only dirty rectangles when possible; `gfxClearRect` before reprinting
  variable-width values.
- Never allocate in `draw` except lazy bitmap create (envelope preview).
- Do not read `PlaybackState`. Use `chipnomadGetPlaybackStatus()` and
  `voiceMonitors` (UI-facing copies).
- After editing project data, set `projectModified = 1`.

## Touch / desktop extras

`TOUCH_INPUT` draws a virtual pad outside the 40x20 canvas (Android portrait
below, landscape in side bands). Tracker coordinates stay 40x20.
`gfxGetTouchGridPosition` maps physical taps onto the grid.

Desktop: window resizable, aspect preserved, ALT+ENTER fullscreen. Do not
make layout depend on window size.

## Checklist for a new screen

1. Declare `extern const AppScreen screenFoo` in `screens.h`.
2. Implement in `screen_foo.cpp` with the same static `ScreenData` pattern.
3. Wire SELECT navigation from neighbours and the screen map letters.
4. Set `playbackLevel` if Play should start something.
5. Keep all text <= 40 chars; messages <= 41 with terminator.
6. Use ColorScheme. No new fonts.
7. If it is an instrument, use common header + voice-post helpers.
8. Update `docs/USER_MANUAL.md` for alpha, not in-app help.

## Caveats log

### 2026-08 — Mixer startup crash

- Symptom: entering Mixer crashed
- Cause: page/cursor init
- Rule: Mixer `setup` resets page 0 and row count to tracks+1 (Auto Mix row)
- Files: `tracker/src/screens/screen_mixer.cpp`
- Status: fixed

### 2026-08 — Screen map vanished after modals

- Symptom: MSCPIT overlay gone after character/FX edit
- Cause: modal redraw skipped `drawScreenMap`
- Rule: `screenDraw` always maps except title
- Files: `tracker/src/screens/screens.cpp`
- Status: fixed

### designbook — do not reflow hidden fields

- Symptom: jumping cursor when a parameter hides
- Cause: packing remaining rows
- Rule: hide in place; keep coordinates
- Files: `docs/designbook/README.md`
- Status: open as a layout rule
