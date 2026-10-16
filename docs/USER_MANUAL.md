# ChooChooTracker User Manual

ChooChooTracker is an 8-track music tracker for handheld consoles.

It follows the classic LSDj workflow and navigation system, with several sound design options:

- Vintage chips and consoles emulation
- Soundfonts
- Mutable Instruments Braids, Plaits and Plaits-Alt oscillators, with a multimode filter
- PCM samples, single-cycle waveforms and wavetable oscillators, with a multimode filter
- aChChid, a versatile acid synth based on Open303
- Bogie, a native 12-model drum synthesiser with a multimode filter
- MME, a two-oscillator multi-modulation synthesiser with a multimode filter
- Sintered, a native six-model experimental digital drum synthesiser

## 1. Installation and files

### ArkOS / PortMaster

Install `choochootracker.zip` through PortMaster, or copy the extracted package to the console's `ports` directory. Start **ChooChooTracker** from the Ports menu.

### Miyoo Mini / Mini Plus (Onion OS)

Extract the MiyooPorts archive at the root of the SD card so that its `Roms/PORTS/` folders merge with Onion's existing folders. Start **ChooChooTracker** from the Ports menu after refreshing the collection. Back up the app's `projects/` folder before replacing an existing installation.

### Windows

Run `choochootracker.exe` with `SDL2.dll` and `libwinpthread-1.dll` in the same directory.

Windows input supports keyboard (see Jazz mode section for details) or Gamepad.

### Web

You can use Choochootracker online at https://choochootracker.vercel.app/ .

You can control this version with Keyboard, on-screen controls when using a mobile device, or your gamepad.

Saved settings, samples, themes, projects, etc. are stored in the browser's storage.

After screen lock or switching apps, tap a control to recover audio if needed. **Restart audio** reopens the output without discarding the project. **Reload app** saves the current work to **Continue**, saves settings, and reloads.

Use **Download .cct** to export your projects.

Note: due to iOS restrictions, using midi with the web version using an iPhone may be impossible.

### Directory configuration

| Folder | What it contains |
|---|---|
| `AY_Wavetables` | `.aywave` AY chip wavetables |
| `fonts` | `.cnfont` ChipNomad font files |
| `instruments` | `.cni` instrument presets but also FM banks and Soundfonts |
| `pitch-tables` | `.csv` alternative tunings |
| `projects` | `.cct` song and project files |
| `samples` | `.wav` files for the Sampler engine |
| `SR_wavetables` | Serum-format wavetables for the BYOWTBL engine |
| `themes` | `.cth` ChipNomad themes |
| `title` | Title screen assets |
| `waveforms` | `.wav` single-cycle waveforms for the 2xSCWF engine |

## 2. Controls

ChooChooTracker works best with a gamepad that has a D-pad, 2 analogue sticks and 7 buttons.

| Logical control | Handheld default | Windows default |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | D-pad | Arrow keys |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Space |
| **SELECT** | Select | Shift |
| Modulation axes | Left and right sticks | Game-controller sticks |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Mappings can be changed in **Settings > Key mapping**. Save a remap with **Done**, then release the button before continuing in Settings.

On Android, a USB or Bluetooth pad that the system does not recognise as a standard controller still works: its D-pad (hat or X/Y axes) drives the directions, and pads that report numbered buttons (`KEYCODE_BUTTON_1` to `KEYCODE_BUTTON_16`) show them as **Btn 1** to **Btn 16** in **Key mapping**, where you assign each one yourself (there is no default for these). Analog sticks used as a D-pad are not supported yet. The touch controls stay visible while a gamepad is connected, unless **Hide pad w/ gamepad** is ON in **Settings > Controls**.

### Navigation

- Use **[UP/DOWN/LEFT/RIGHT]** to move the cursor.
- Hold **SELECT + [DIRECTION]** to move between screens.
- Hold **OPT + [DIRECTION]** for screen-specific navigation.
- On a value, use **EDIT + [LEFT/RIGHT]** for fine changes or **EDIT + [UP/DOWN]** for coarse changes.
- Tap **EDIT** to enter a value or activate a command.

### Selection and clipboard

- **SELECT + OPT** enters selection mode and cycles through useful selection ranges.
- Use **[DIRECTION]** to extend the range.
- Tap and release **OPT** to copy the selection and leave selection mode.
- **OPT + EDIT** cuts it.
- Outside selection mode, hold **SELECT** and press **EDIT** to paste it.
- **EDIT + [DIRECTION]** edits the selected cells together.

### Playback

- **PLAY** starts from the cursor. Press it again to stop.
- You can edit notes, instruments and chains while playing; changes are heard
  at the next sequencer tick.
- **SELECT + PLAY** starts all tracks when used outside the Song screen.
- **EDIT + PLAY** previews an instrument from the Instrument Pool.


### Global Bounce

Select a range of Song, Chain or Phrase rows and columns, then double-tap **EDIT** to open the BOUNCE TO SAMPLE screen. The bounce includes every selected track: tracks whose first chain starts later in the selection wait silently and join at their first chain, so the file contains all selected tracks' chains.

The screen offers:

- **File name**: edit with the character keyboard. When the screen opens, the next free sequence number (`001`, `002`, ...) is proposed here: the first number whose `.wav` file does not exist yet in the export folder. Edit it freely — the name is used as-is. If a file with that name already exists, a `_001`, `_002`, ... suffix is added so existing files are never overwritten. The counter resets when you load or create a project.
- **Sample rate** and **Bit depth**: the same options as the export screen
- **Include in a sample name**: three checkboxes that prepend context tags to the file name when **Start** is pressed (all off by default):
  - **[BPM]** — the project's current tempo, e.g. `[120]`
  - **[Key]** — the project's root note and scale, e.g. `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** — the rendered length of the bounce: one phrase equals one bar (16 sixteenths), a beat is 4 sixteenths. Full phrases show only bars, e.g. `[2]`; partial phrases show `[0:2]` (two beats) or `[0:1:3]` (one beat plus three sixteenths). For multi-track bounces the longest track decides. A selection ending mid-beat counts one sixteenth less: the last selected row is the cut point, so its note-off lands at the end of the previous sixteenth.
  - Tags are applied in the order `[BPM][Key][Length]` before the file name.
- **Start**: render the selection to a WAV file in the export folder (see *Export location* under Project screen)
- **Cancel**: return without bouncing

While the bounce renders, **OPT** cancels it. When the bounce completes or is cancelled, the screen you bounced from returns.

## 3. Song structure and screen map

Hold **SELECT + [DIRECTION]** to move between screens.

The main navigation row is:

<pre>
        I
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Central row:

- **M**: Mixer (branches to Reverb, Delay)
- **S**: Song (branches to Project, Settings)
- **C**: Chain
- **P**: Phrase (branches to Groove)
- **I**: Instrument (branches to Modulation and Instrument Pool)
- **T**: Table (branches to the AY Wavetable editor)

Top row:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation
- **I**: Insert FX

Bottom row:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: AY Wavetable editor

A ChooChooTracker song follows this hierarchy:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

A song has 8 tracks. Each track contains a sequence of chains, and each chain contains a sequence of phrases. Phrases are 16-step sequences of notes. A note contains a pitch, an instrument, a velocity and up to 3 track FX. An instrument is the thing that makes the sound.



## 4. Song, Chain and Phrase

### Song

The Song screen contains 8 columns, 1 per audio track. Each cell refers to a chain. A song can contain up to 256 rows (`00-FF`).

Tracks run from left to right, while their chains run vertically. Each track is independent, and chains can contain from 1 to 16 phrases, so tracks can drift out of sync. Design your chains to stay together, or let them wander if that is what the track needs.

It is common practice in the tracker community to reserve 1 chain for empty phrases, usually `00` or `FE`.

You can keep multiple sub-songs in a project by creating "islands": sections separated by empty rows.

#### Live Mode

Double-tap **OPT** to enter or leave Live Mode; `LIVE` replaces the Song title. In Live Mode, **PLAY** starts the selected chain on its own track when that track is stopped. On a playing track, it queues the selected chain for the next chain boundary; press **PLAY** twice for an urgent change at the next phrase boundary. A selected empty Song cell queues a quantised stop instead. `+` marks a queued chain, `!` an urgent chain and `-` a queued stop.

Select one Song row across several columns to queue or stop those tracks together. Multi-row selections are deliberately rejected. Live queues are cleared while a Song range loop is active. Live is a performance mode and is not saved in the project.

#### Controls

- **OPT + [UP/DOWN]**: jump 16 positions up or down
- Select a range, then use **SHIFT + EDIT**: shallow-clone the chains while keeping their original phrases
- Select a range, then double-tap **SHIFT + EDIT**: deep-clone both the chains and their phrases
- Select a range, then use **EDIT + [UP/DOWN]**: move the selection up or down
- Tap **OPT** 3 times: add or remove a highlight for visual organisation
- **OPT + SHIFT**: mute the current track or selected columns (release **OPT** first to keep the mute active)
- **OPT + PLAY**: solo the current track or selected columns (release **OPT** first to keep the solo active)
- **OPT + [LEFT/RIGHT]**: solo every track to the left or right of the current track
- Select a range, then double-tap **EDIT**: open the BOUNCE TO SAMPLE screen (see Bounce below)

### Chain

A chain is an ordered list of 16-step phrases with optional transposition. It can contain up to 16 phrases, and the same phrase can appear more than once. The 2nd column sets the transposition in semitones.

An asterisk (`*`) appears next to a chain that is reused in the project. You can clone chains from the Song screen.

#### Controls

- **OPT + [LEFT/RIGHT]**: move between tracks
- **OPT + [UP/DOWN]**: move between chains in the current track
- Select a range, then use **SHIFT + EDIT**: clone phrases
- Select a range, then double-tap **EDIT**: open the BOUNCE TO SAMPLE screen for the selected chain rows of the current track (see Bounce under Song)

### Phrase

A phrase is the track pattern in a traditional step sequencer.

A phrase is 16 steps long, with 1 row per step. Each row contains a note, an instrument, a volume from `00` to `7F`, and FX columns. Notes use tracker notation such as `C-4` (note and octave).

To stop a note, insert `NOTE OFF`, use the kill-note FX, or play another note with no instrument set.

When a chain contains several phrases, the Phrase screen works like a continuous pattern editor. Moving below row `F` or above row `0` opens the next or previous phrase in the chain.

An asterisk (`*`) appears next to a phrase that is reused in the project. You can clone phrases from the Chain screen.

The FX selector shows common commands plus those supported by the instrument on that row. This avoids a long global list of engine-specific commands. ChooChooTracker supports more FX than ChipNomad.

The selector keeps its compact help panel and group spacing consistent as you browse. Commands use three-character labels; the help above shows their meaning and, where available, the saved value and range. Long groups scroll to keep the selected command visible, with arrows indicating more entries.

### Controls

- **OPT + EDIT** on an empty note: insert `NOTE OFF`
- **EDIT + [UP/DOWN]** on an FX name column: open the FX selection screen
- **OPT + [LEFT/RIGHT]**: move between tracks
- **OPT + [UP/DOWN]**: move between phrases in the current chain
- Select a range in the instrument column, then use **SHIFT + EDIT**: clone instruments
- Select a range, then use **EDIT + [UP/DOWN]**: rotate the phrase rows
- Select a range, then double-tap **EDIT**: open the BOUNCE TO SAMPLE screen for the selected phrase rows of the current track (see Bounce under Song)

On this screen, with a selection active, the note column can be filled
with rhythm patterns:

- **OPT + LEFT** fills the selection's note cells with a fixed 16-step pattern
  and advances to the next pattern on every press. After the last pattern the
  notes are erased and the cycle starts over. Patterns are aligned to absolute
  phrase rows, so a partial selection only receives the steps that fall inside
  it.
- **OPT + RIGHT** fills the selection's note cells with a randomized pattern of
  3-8 notes (7 and 8 are less likely), cycling indefinitely while **OPT** is
  held.
- **OPT + UP** mutates the notes already placed in the selection: every press
  picks 1-3 of them and shifts each by a random amount within an octave
  (small shifts are more likely than extreme ones). The 8th press restores the
  notes the selection held when the cycle started, then the cycle starts over.
- **OPT + UP** on a selection covering the volume column only (no note,
  instrument or FX cells) randomizes velocities instead of mutating: the
  first press applies the Minimal pattern (each volume shifts by up to
  +-20), the second Medium (up to +-50), the third Random (completely
  random values), and the fourth restores the volumes the selection held
  when the cycle started. Releasing **OPT** resets the cycle to Minimal.
  Rows without a volume are left untouched.
- **OPT + DOWN** spreads the placed notes of sliced Sampler instruments in
  the selection into a chromatic run: each instrument's first note (highest
  in the sequencer) keeps its value and every following note of that
  instrument rises one semitone above the previous one, turning a stack of
  identical notes into consecutive slice triggers. Several sliced
  instruments spread independently from their own first notes, notes on
  instruments without slicing are left untouched, and the result is never
  snapped to the project scale. The last note the spread writes becomes
  the last note used in note input, so continuing the run manually picks
  up the next slice. A selection holding only sliced
  instruments spreads on every press; when non-sliced notes are present
  too, the spread runs on the first press and the random arp takes over
  those notes from the second press on.
- **OPT + DOWN** on a selection without sliced-instrument notes is a random
  arpeggiator for the notes already placed there. A chord is picked from
  the CRD palette - 3-note chords for selections of 1-2 notes, 3+ note
  chords for 3, any chord for 4 or more - with its root on a random note
  of the project scale inside the selection's register (any chromatic
  note when the scale is off or the track is excluded from it), and its
  tones are quantized into the scale like playback does for CRD. Every
  press re-spreads the chord across the placed notes: randomized,
  lowest-to-highest, highest-to-lowest, then up-and-down; the 5th press
  restores the original notes and the next press picks a fresh chord.
  The chord name shows in the status bar while the arp is active, no
  notes are added or removed, and releasing **OPT** resets the whole
  process.
- Every fill press erases the selection's notes before writing, and placed
  notes carry the last used instrument; releasing **OPT** resets the cycles
  to their first step.

### Key jazz (desktop only)

Press **Esc** on the Phrase screen to toggle key jazz, a QWERTY piano-style
note entry mode (like [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` and `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` type notes
  chromatically across two-plus octaves, using each key's physical position
  so the layout is the same on AZERTY, QWERTY and QWERTZ keyboards.
- Each keypress writes the note on the current row and moves to the next row.
- `[` / `]` shift the octave.
- **Shift + arrows** select a range of rows and columns; a plain arrow
  afterwards clears the selection.
- **Ctrl+C** / **Ctrl+X** copy or cut the selected columns, or the current
  row's note/instrument/volume if nothing is selected; **Ctrl+V** pastes at
  the cursor.
- **Delete** removes the whole row(s) (every column) and shifts the rest of
  the phrase up to fill the gap; the cursor stays on the same row.
- **Backspace** removes the element under the cursor in the current column
  only (or the selection's columns, if one is active), shifts whatever is
  below it up to fill the gap, and steps the cursor back one row, like a
  text editor.
- **Insert** inserts a blank row, pushing the rest of the phrase down.
- **Ctrl+S** saves the project. Outside key jazz, Ctrl+S does nothing.
- Esc again turns key jazz off.

While key jazz is on, it takes over the note keys entirely on the Phrase
screen, including Edit/Opt/Motion shortcuts that share those keys — turn it
off with Esc to use them again.

Key jazz also works, independently toggled with Esc, on:

- **Song** and **Chain**: type a chain's or phrase's hex index directly
  (`0-9`, `A-F`) instead of incrementing with Up/Down. The first digit typed
  on a cell replaces its value; further digits typed without moving shift
  into it (typing "3" then "F" sets 3F). On Song, **Shift+arrows** select a
  range, **Delete** removes whole row(s) and shifts the rest up, **Backspace**
  removes the current/selected column(s) and shifts what's below them up,
  **Insert** inserts a blank row, and **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  copy/cut/paste the selected rows (or the current row if nothing is
  selected) - the same structure-editing shortcuts as the Phrase screen.
  This takes over Shift, so Shift+Right/Up no longer jump to Chain/Project
  while key jazz is active here - Esc to get those shortcuts back.
- **Every screen with a text field**: type directly on the keyboard
  instead of using the on-screen virtual keyboard. This covers the Project
  file name, title and author, the instrument name, the color theme name,
  the pitch table name, the bounce file name, and the name dialogs (enter
  name, create folder). Backspace works like a normal text field; Shift
  types uppercase letters. Esc toggles it on all of these screens at once,
  and the virtual keyboard still opens normally when key jazz is off.
- **Instrument**: with key jazz on, the note keys play the instrument on the
  current track to audition it, without entering any note (the same piano
  layout as the Phrase screen; the last key pressed sounds, and the sound is
  released with the keys). The octave keys shift the octave, starting from
  the octave of the instrument's first note. On the **Name** field the keys
  type text instead. Nothing is played while the song is playing that track.

## 5. Instruments

All instruments have a few common parameters:

- Instrument type
- Name, up to 15 characters
- Default table speed, in ticks per table row
- Volume
- Panning
- Transpose on or off

Each instrument has a default table with the same number in the `00-7F` range. You can use these as auxiliary tables, but keeping auxiliary tables in the `80-FE` range avoids conflicts and confusion. Tables are among the main sound design tools in ChooChooTracker.

### Controls

- **OPT + [LEFT/RIGHT]**: move between instruments
- **EDIT + PLAY**: preview the instrument
- **SHIFT + OPT**: copy the instrument
- **SHIFT + EDIT**: paste the instrument

### SOUNDFONT (experimental)

Copy original SF2/SF3 banks, SFZ/EXS instruments with their samples, or ZIP archives
into `instruments/USER/soundfont`. Select **SOUNDFONT** on Instrument. The scan
row shows **Not scanned**, **Scanning...**, or **Scanned**, with a **Rescan** action.
Scanning reads metadata; it does not load every preset's samples into memory.

The **Preset** row shows the active preset. Tap and release EDIT to open the
browser at that preset: collections appear on the left and their presets on the
right. Each collection presents a flat list, including presets inside bank files
and ZIP archives. Supporting WAV/AIFF files stay with their mapped instruments;
collections containing only individual samples list those samples as presets.
Download IDs and archive hashes are removed from display labels; musical note,
BPM and take numbers are retained. EDIT selects a preset; EDIT+PLAY previews the
loaded selection.

Hold EDIT and press Left/Right on Preset to move sequentially in the same order
as the browser, continuing into the next collection and wrapping at either end
of the library. Rapid cycling prepares the latest requested selection. The
current preset stays playable while a replacement prepares. If loading fails, the
requested preset remains selected and the instrument becomes silent. **Error:**
and the reason appear at the bottom of Instrument; cycling continues from that
preset in either direction. Rescan after importing or changing library files. Preset status appears
directly below Preset with a fixed color. Loading does not post messages on SONG
or other pages.

**Native keys** shows the selected preset's authored note-on range, using C4 for
MIDI note 60. **(gaps)** means some notes inside those bounds have no mapped zone.
It is the union across velocity layers; not every velocity or articulation must
sound at every displayed key. Release-only samples do not extend the range.
Tuning and pitch FX do not change this display or extend the authored mapping.

**Edit**, to the right of Preset, opens the experimental SoundFont editor:
tune (−2400 to +2400 cents), cutoff (20–20000 Hz), resonance (0–40 dB), envelope
delay/attack/hold/decay/release (0–20000 ms), sustain (0–100%), volume, pan, and
**Bend step** (1–100 cents per PBN unit). New instruments use a 1-cent bend step.
Older files show **Stock**, retaining their previous PBN scale (25 cents in linear
pitch mode, 1 in period mode); set Bend step to 1 for finer bends. Clear restores
a 1-cent bend step. This affects SoundFont PBN only; FPT remains fixed tuning.
Volume and pan are the same instrument values shown on the home page.
Tone and envelope fields default to **Bank**, retaining each layer's authored
value. An explicit value overrides that parameter across the preset's layers.
Clear restores **Bank**, zero cents for Tune, full volume, or centered pan.
EDIT+Left/Right makes fine changes; EDIT+Up/Down makes larger changes. OPT returns
to Instrument; EDIT+PLAY auditions the instrument at its song note or the default.
This edits instrument playback controls, not the bank's per-sample mappings,
key/velocity zones or loop points.

Tuning, cutoff and resonance apply live. Attack, delay and hold are set at
note-on. Decay, sustain and release can update a playing voice; release begins
with a note OFF, not just on an empty row. Decay is inaudible at 100% sustain.
A release time cannot extend a sample past its available audio. An explicit
release override also makes a one-shot sample respond to note OFF. With Bank
settings, its authored one-shot behavior remains. The preset's key and velocity
ranges remain authoritative; notes outside them are silent. FPT can pitch an
already selected sample beyond that range without changing its mapping.
Edits save in instrument and project files without modifying the source bank;
older songs without overrides retain their original bank settings.

A WAV with an unknown sampler-specific loop type can play its audio without that
loop. The Instrument warning names the ignored loop type; the source file is
unchanged. A forward WAV loop ending exactly one sample beyond the audio is
accepted as an exclusive end at the sample boundary, with a warning; the PCM and
explicit SFZ/EXS loop mappings are unchanged. Other invalid loop bounds and root
notes report specific errors. A single zero RIFF alignment byte mistakenly
counted in an odd-sized PCM frame layout is ignored with a warning; other partial
PCM frames remain errors. Standard alternating/backward WAV loops are still
unsupported.

**SoundFont FX** exposes eight controls in the contextual Lane FX picker. All
values below use tracker hexadecimal notation. The FX affect their track only
and leave the instrument's saved controls unchanged. Each new note starts from
the instrument settings, then applies commands on its row and subsequent rows.

| FX | Control | Values |
| --- | --- | --- |
| FPT | Pitch | `00` = −24 semitones, `18` = zero, `30` = +24; exact semitone steps |
| FCF | Cutoff | `00`–`FE`: 20 Hz–20 kHz, logarithmic |
| FRS | Resonance | `00`–`FE`: 0–40 dB |
| FAT | Attack | `00`–`FE`: 0–20 seconds, finer spacing at short times |
| FHD | Hold | `00`–`FE`: 0–20 seconds before decay, set on the note row |
| FSU | Sustain | `00`–`FE`: 0–100% held level |
| FDC | Decay | `00`–`FE`: 0–20 seconds, finer spacing at short times |
| FRL | Release | `00`–`FE`: 0–20 seconds, finer spacing at short times |

For FCF/FRS/FAT/FHD/FDC/FSU/FRL, `FF` returns to the Edit setting (including **Bank**).
Put Attack/Hold on the note row. Decay/Sustain/Release can change the active
note; use OFF to hear its release. Volume and pan use the common controls;
legacy FVL/FPN commands still load and play but are no longer offered. Pitch FX
change tuning without changing the bank's selected key zone. Existing sequencer
pitch/fine-pitch commands, MODs and track effects remain available. Projects save
stable bank/preset references and FX commands; sample libraries remain external.


All banks are USER banks. Large instruments may exceed the default 64 MiB PCM
cache, and unsupported source parameters produce warnings. See the
[SoundBanks guide](soundbanks/README.md) for platform paths, browser import,
cache configuration, format coverage and current validation limits.

#### SoundFont compatibility: banks, presets and modulation

One SF2 or SF3 **bank file** can contain many **presets**, together with their
samples. A library **collection** is the folder used to group those files in the
browser. These are different levels: a preset can have unsupported behavior even
when other presets in the same file translate well.

**Preset modulation** means programmed links from inputs such as velocity, key,
mod wheel or pressure to a sound parameter. Those links can belong to a preset
or to an underlying instrument layer shared by several presets. They are not
necessarily a setting that affects every sound in the bank.

| Preset feature | Current translation |
| --- | --- |
| Sample/key/velocity mapping, root pitch and tuning | Implemented; authored ranges are retained. Velocity zones and ordinary tracker velocity still work. |
| Sample offsets, supported loops, gain/pan and exclusive groups | Implemented within the parser and voice limits. |
| Static low-pass cutoff/resonance and volume envelope | Implemented through the tracker renderer; not a guarantee of identical response to another SoundFont player. |
| Custom preset/instrument modulation routes (`pmod` / `imod`) | Not implemented. Authored controller-to-parameter mappings, amounts and response curves are ignored. |
| SoundFont modulation/vibrato LFOs and modulation envelope routing | Not implemented; programmed movement of pitch, filter or volume may be missing. |
| Key-scaled volume-envelope hold/decay, fixed-key/fixed-velocity generators | Not implemented. |
| SoundFont chorus/reverb sends | Not implemented. Tracker effects can be set manually but do not reproduce the preset's settings automatically. |

The older message **“Custom bank modulation ignored”** describes a file-level
check: the parser found custom modulation records somewhere in the bank. It does
**not** establish that the selected preset uses them, or that the whole bank is
unusable. This general limitation is documented here instead of repeating that
message under every preset. Detailed parser diagnostics retain the records;
other load errors and compatibility warnings still appear when applicable.
The engine does not claim complete SoundFont default-modulator behavior either.
Tracker PBN, vibrato, Edit and Lane FX are independent controls, not automatic
translations of omitted preset programming.

For choosing a library, start with presets based mainly on sample mappings,
loops and static envelopes. Sounds whose character depends on controller
articulations, animated filters or programmed LFOs need closer comparison in a
full SoundFont player. **Loads and plays** does not mean **fully reproduces the
preset**. Compare useful notes and velocities as well as held notes and note OFF.
Changing SF2 to SF3 (or back) does not by itself remove these modulation needs.
A conversion must translate the mappings and behavior into features this engine
supports; rendering an effect into a sample preserves only that rendered result,
not its interactive controller response. See the [format coverage and limits](soundbanks/README.md#format-coverage)
and [validation notes](soundbanks/validation.md) before choosing or
converting a bank. These limits describe the current experimental implementation,
not restrictions inherent to all SoundFont players.


### AY Classic, AY Plus and AY Sample

These are the original ChipNomad AY/YM engines. AY Classic exposes hardware-style tone, noise and envelope controls. AY Plus adds software oscillators and richer modulation. AY Sample reproduces a sample through AY-style volume levels and is distinct from the Sampler engine.

#### AY Classic

This is a legacy instrument type from ChipNomad `v1.0.0`. The newer AY Plus instrument offers more features.

AY Classic instruments have these parameters:

- Mixer: tone on or off, noise on or off, envelope shape (`0-F`)
- Volume: software-generated ADSR envelope
- Automatic envelope period: on or off, with a rate from `1:1` to `F:F`

**AY Quality** in Settings affects only AY/YM rendering. It does not change Braids, Plaits or Sampler quality. **Sample dithering** applies to AY Sample quantisation.

See the [ChipNomad AY-3-8910 documentation](https://chipnomad.org/chips/ay-3-8910/) for chip details.

#### AY Plus

AY Plus applies classic synthesiser concepts to AY-3-8910/YM2149F sound design. You can think of AY as a synthesiser with 5 oscillators:

- 3 square wave tone oscillators
- 1 noise oscillator
- 1 amplitude envelope generator which can be used as an oscillator

Under this model, each instrument has 3 hardware oscillators: tone, noise and envelope. AY Plus adds a 4th, software oscillator. Updating AY registers faster than the project tick rate opens up more sound design options. Because of the way AY works, the oscillators are mixed using ring modulation. Try different pitch offsets, detuning and oscillator combinations to see what falls out.

Each tonal oscillator can be tuned independently with pitch and fine tune offsets.

Software oscillator types and their parameters:

- Pulse - square wave with controllable pulse width
  - FM Depth - simple FM depth
  - Pulse width - the full range is `00-FF`, while the number of steps is set on the Project screen
  - Pulse low - controls the low value in a repeating pair whose high value is always `15`
- Sync Tone - hard-sync effect for the tone oscillator
  - FM Depth - simple FM depth
- Sync Env - hard-sync effect for the envelope oscillator
  - FM Depth - simple FM depth
  - Pulse width - duration ratio between the 2 shapes in the envelope pair
  - Envelope Pair - 2 envelope shapes to switch between
- Wavetable - 32-step wavetables, edited on the Wavetable screen
  - FM Depth - simple FM depth
  - Wavetable index
- Tone FM - simple FM-like effect for the tone oscillator
  - FM Depth - tone FM depth
- Env FM - simple FM-like effect for the envelope oscillator
  - FM Depth - envelope FM depth

Pulse and Wavetable software oscillators cannot be used with the Envelope oscillator. The Envelope oscillator switches off automatically.

Simple FM switches quickly between 2 period values, 1 lower and 1 higher. This is similar to using a square wave as an FM modulator. Pulse, Sync Tone, Sync Env and Wavetable use a `1:1` ratio between the FM carrier and modulator. Tone FM and Env FM can imitate other ratios through software oscillator pitch offsets.

#### AY Sample

You can load mono or stereo WAV files at `8-bit`, `16-bit`, `24-bit` or `32-bit`. They are converted to `8-bit` and truncated to `16 KB`. This limit matches the RAM page size of the ZX Spectrum.

AY plays samples through fast writes to the volume register. Its output is unipolar, so samples need to be "lifted to zero" for cleaner playback. This removes end-of-sample clicks and improves quiet tails.

AY has `4-bit` DAC resolution with a non-linear volume scale, so `8-bit` samples are converted to `4-bit` during playback. The Sample dithering option in Settings can improve the perceived bit depth. Dithering is not practical on retro platforms such as the ZX Spectrum and Atari ST, so leave it off when writing for real hardware or chasing a crunchy lo-fi sound.

Samples behave like another AY Plus software oscillator, but their extra parameters make them easier to handle as a separate instrument type.

Sample parameters:

- Sample rate - playing a sample at its native rate produces a `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

The Tone and Noise oscillator sections match those of AY Plus. Because of the way AY works, all oscillators are mixed using ring modulation.

### Braids

Braids provides 47 synthesis models. Its main controls are:

- **Model**: oscillator or synthesis algorithm
- **Timbre** and **Color**: model-dependent macro parameters
- **Filter**: additional LP/BP/HP filter, `12` or `24 dB` slope, cutoff and resonance
- **ADSR**: attack, decay, sustain and release

Tap **Model** to choose from categorised model lists.

| Category | Models |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parameters

Timbre and Color depend on the selected model. Refer to the original Braids manual from Mutable Instruments for the full model list and details, or just play by ear.

#### VCF and VCA

Braids feeds the standard ChooChooTracker multimode filter and ADSR envelope.

#### Settings

Global Braids settings are available in the app settings:

- **BITS**: reduces output resolution
- **DRFT**: adds oscillator pitch instability
- **SIGN**: reproduces the original Braids waveform-imperfection algorithm

### aChChid

**aChChid** is a monophonic acid bass engine based on Open303. `Square` and `Saw` use its native TB-303 oscillator, filter, envelope and accent behaviour. `Braids` replaces only the oscillator, then continues through the same 303 filter and amplifier path. It exposes Model, Timbre, Color and an extra waveshaper. Theshaper progresses from soft saturation into wavefolding.

Cutoff and Decay use logarithmic editing: **EDIT + LEFT/RIGHT** moves by one
musical control step, while **EDIT + UP/DOWN** moves by sixteen steps. Cutoff
covers 200 Hz to 20 kHz; Decay covers 200 ms to 2 s.

An `F` in the note volume column triggers an accent. `ASL` slides to that note from the previous pitch without retriggering the 303 envelope; it automatically holds the preceding gate, and `ASL 00` gives a `60 ms` glide. `ATY` keeps its note open for the whole current step. Notes without `ASL` always retrigger.

Modulation destinations include Decay and Accent, plus Timbre and Color in Braids wave mode.

### Bogie

**Bogie** is a native one-shot drum synthesiser. Tracker notes set pitch and the tracker/instrument volume sets level; each trigger has its own internal decay, so it has no ADSR page. It has 12 models: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` and `Clave`.

Its six macros are **Decay**, **Tone**, **Sweep**, **Noise**, **FM** and **Drive**. Their labels adapt where useful (for example, Kick's Noise is `Click`, and Cowbell's FM is metallic cross-modulation). Every macro is active on every model, but its musical role changes with the model: Sweep can alter pitch, burst spacing or metallic spread; FM can add cross-modulation, a metallic layer or a ring-like overtone. The lower half of each range is intended for conventional drum sounds; the upper half deliberately opens wider timbral morphs and more synthetic results.

Every model can use the shared LP/BP/HP multimode filter, with Clean, Classic, Aggro or Acid character, `12`/`24 dB` slope, cutoff and resonance. Bogie supports all of its active macros plus Volume, Pitch, Cutoff and Resonance as modulation and motion-recording destinations. Use short Decay with Cowbell for sharp phonk attacks; increase FM for a harder, more metallic bell.

On the Modulation screen, press **EDIT** on a source to choose a family (Envelopes, LFO or Sticks), then its source. **Left** and **Right** still cycle sources directly.

### Sintered

**Sintered** is a native one-shot digital percussion synth. Notes set its fundamental pitch; its six models are `Knot`, `Shard`, `Burst`, `Comb`, `Logic` and `Melt`. Every model has **Decay**, **Mod** and **Motion**, plus three model-specific controls: Knot uses Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush and Melt Ratio/Chaos/Drive. **Mod** is always the internal coupling depth.

Motion is an internal macro envelope: values below `80` use an AD shape, `80` disables it, and values above `80` use a decay-only shape. Moving farther from the centre makes the envelope faster. Its destinations are fixed per model, so Sintered stays predictable in tracker patterns: Knot animates Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. All models reset their state on a note trigger and bound internal feedback for repeatable hits.

### MME

**MME** (Multi Modulation Engine) is a two-oscillator synth dedicated to interactions between oscillators. Its models are `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` and `Vocode`. Both oscillators are internal and follow tracker notes; `Interval` sets their relationship.

The six macros are **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** and **Shaper**. Waves interpolate between musical A/B waveform pairs. Amount controls the model's core interaction, Flow changes its direction or algorithm variant, and Feedback is bounded but becomes deliberately wild at the end of its range. Shaper is a shared final stage which moves from clean through saturation to wavefolding.

MME uses the shared LP/BP/HP multimode filter and ADSR. All six macros, Cutoff and Resonance are available to modulation and motion recording. Hold **EDIT** on Model to open the model popup.

### MIDI Out

**MIDI Out** (desktop, PortMaster and Web MIDI browsers) drives an external MIDI device instead of synthesizing audio: triggering a note sends a real MIDI Note On on the instrument's **Channel** (`1-16`) and, when the note ends, the matching Note Off. Volume becomes velocity, and a chord track's voices each get their own Note On/Off. There is no audio to hear from ChooChooTracker itself - select the MIDI output device under [Settings](#13-settings) first. See [MIDI](#15-midi) for the full picture, including sound preview from a MIDI keyboard.

**Program** and **Bank high/low** (CC0/CC32) are optional - shown as `--` when off, `Clear` toggles a field off and remembers its value. When set, they are sent once, right before the next note, whenever they differ from what that channel was last told (not before every note, which would retrigger the receiving device's own envelopes/patch).

**MC1 number**-**MC4 number** pick which CC number (`00-7F`) each of the four MC1-MC4 row FX sends to - see [MIDI Out FX](#midi-out-fx). Also `--`/optional; a slot left unset makes its FX inert.

### Classic architecture engines

The engines in this category share a VCO to VCF to VCA architecture.

Several VCO types are available:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Each instrument feeds its VCO into a multimode filter.

The filter has a switchable `12 dB` or `24 dB` slope and can work as a low-pass (LP), band-pass (BP) or high-pass (HP) filter.

Several filter characters are available:

- **Clean**: sterile and digital
- **Classic**: inspired by classic American analogue synths
- **Aggro**: inspired by roaring Japanese analogue synths
- **Acid**: double the aggro of aggro

Classic, Aggro and Acid can get seriously resonant. Start low and turn them up gently.

The filter feeds a VCA controlled by an ADSR envelope. The envelope shape can morph between exponential, linear and logarithmic curves.

#### Plaits and Plaits-Alt

**Plaits** provides 24 engines, grouped in the selection popup as follows:

| Category | Engines |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** is a collection of alternative models for the original Plaits module. Most are stranger or more experimental than the average VCO.

| Category | Engines |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

The parameters follow Mutable's design:

- **Harmonic** controls harmonic relationships, balance or model choice inside an engine.
- **Timbre** generally moves from dark/sparse to bright/dense spectra.
- **Morph** explores another timbral dimension.
- **Main/Aux** blends the main output with the engine's alternate output.

Their precise meaning depends on the engine. For example, chord engines use them for chord type, inversion and waveform. Physical models use them for material, excitation and decay, while drum engines use them for tone, character and decay. Refer to the Plaits user manual for details.

For `CHORD`, `DIATONIC CHORD` and `WT DIATONIC CHORD`, Harmonic displays the selected chord name (`MIN`, `MAJ`, `MIN7`, `MAJ7`, and so on) rather than its raw control value. Braids `WAVE PARA` similarly names its Color voicings.

Tap **Engine** to choose from categorised engine lists.

**Env Mode** has 2 routings:

- `TRIG` reproduces the module with TRIG connected and LEVEL unpatched
- `VCA` holds LEVEL open and applies the tracker ADSR after the voice

#### Sampler

This clean Sample engine plays mono or stereo PCM samples loaded into RAM.

- Tap **Sample** to load an uncompressed `8-bit`, `16-bit` or `24-bit` PCM WAV. Press **PLAY** in the browser to audition the highlighted file.
- When a sample is loaded, **EDIT** appears next to its name. Tap it to open the Sample Edit screen. **OPT** or **SELECT + [LEFT]** returns to the instrument.
- The Sample Edit screen shows the filename (with a `*` marker when the sample in RAM differs from the file on disk), a format readout (sample rate, channels), a tall waveform, and the **Region**, **Select**, **Slice**, **Process** and **File** fields.
- Edits live in RAM only until a save flow writes them: the project stores the sample's file path and reloads the WAV from disk on the next project load, so leaving the tracker without **Save**/**Save As** discards process operations (the `*` marker is the warning). Loading a different sample into the instrument also discards unsaved edits and resets the Region to the full sample (Start `00`, End `FF`).
- **Region** sets the playback Start/End markers, also available on the Sampler instrument screen. They set normalised playback boundaries (`00-FF`); if Start is after End, the sample plays in reverse. **EDIT + [LEFT/RIGHT]** fine-adjusts the marker in steps of one (`01`) and zooms the waveform around it while **EDIT** is held; **EDIT + [UP/DOWN]** coarse-adjusts (step 16) and returns the view to the full sample. **EDIT + OPT** resets the marker to its default (Start `00`, End `FF`).
- The waveform view zooms to a fixed window of one second of audio around the edited marker and pans to keep it visible; samples that fit inside the window stay at the full 1:1 view. The zoom is transient: it lasts while **EDIT** is held, and releasing **EDIT** returns to the full 1:1 view, as do coarse steps. Entering the screen always resets the view to the full sample.
- **Select** sets a processing selection in absolute frames, independent of the playback Start/End markers. **EDIT + [LEFT/RIGHT]** moves a handle twenty frames and zooms onto it while **EDIT** is held; **EDIT + [UP/DOWN]** jumps by `frameCount/64` (minimum 16) and returns to the full-sample view. **EDIT (tap)** on a handle copies the current Start or End marker position to it. **EDIT + OPT** on either handle empties the whole selection. When the selection is empty both handles show `-`; when the handles are inverted they swap automatically. The selection is session-only editor state: it is not saved with the project, and entering the screen seeds it with the playback Region span (the whole sample with the default markers).
- **Slice** is a single row with four value cells - **Mode**, **Count**,
  **Slice** and **Frame** - and is saved with the instrument. Off plays the
  Start/End window. Phrase notes select slices chromatically from **C-0**, and
  notes past the last slice wrap around to the beginning (note N plays slice
  `N % count`, so a run past the last slice cycles back to the first one).
  Sliced notes do not transpose pitch or use Scale quantization, but **CRD**
  keeps the selected slice and transposes its voices as a chord. Slice starts
  are marked on the waveform with short two-pixel-wide lines (dark orange; the
  current slice's line is brighter), and the slice under the cursor gets a
  black background band on the waveform (the selection tint is not drawn over
  the band, so it stays fully black and clearly distinct from the rest of the
  preview).
  - **Mode** cycles `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + left/right or
    tap; EDIT + OPT turns slicing off). Switching modes initializes the slice
    points: **EQUAL** divides the Start/End window evenly into **Count**
    parts; **AUTO** runs a transient detection over the window and places a
    slice on every onset it finds (up to **Count**); **LAZY** starts with one
    slice covering the whole window. Switching to LAZY stashes the current
    EQUAL/AUTO setting (count and bounds), and switching back to EQUAL or
    AUTO restores it verbatim - no confirmation is asked and nothing is
    lost. Slice and Stretch are mutually exclusive: enabling one disables
    the other, and the Slice cells are dimmed while Stretch drives the
    duration.
  - **Count** shows the total number of slices (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** steps it by one (minimum 1, maximum 64), **EDIT +
    [UP/DOWN]** cycles through the power-of-two counts `2 / 4 / 8 / 16 / 32 /
    64` with wrap-around. Every change recalculates the division: **EQUAL**
    re-divides the window evenly, **AUTO** re-runs detection with the new
    target (the detection sensitivity is derived from the count - more slices
    mean a more sensitive threshold). The box shows `-` when the mode is
    OFF. In **LAZY** it stays bright (it counts the hand-placed slices) but
    is display-only: the cursor skips it — moving right from **Mode** or
    left from **Slice** lands on **Slice**, coming down from the **Select**
    row's **END** marker lands there too, and touching the box does nothing
    (hand-placed slices have no division to recalculate). **EDIT + OPT** and
    a plain **EDIT** tap do nothing here: turning slicing off lives on
    **Mode**, deleting a slice lives on **Slice**.
  - **Slice** is the browser: it shows the 1-indexed current slice
    (`01`..count). **EDIT + [LEFT/RIGHT]** moves between slices one at a
    time, **EDIT + [UP/DOWN]** jumps four slices (both clamped, no
    wrap-around); the browsed slice is indicated on the waveform by its
    brighter marker and the black band, and the zoomed view recenters on the
    slice start. **EDIT + OPT** deletes the current slice (it merges into
    the previous one; the first slice cannot be deleted, and deleting the
    last remaining slice turns slicing off - bounds survive an OFF
    round-trip and come back when a mode is picked again). **EDIT (tap)**
    previews the currently selected slice as a one-shot playback (works in
    every slice mode; the preview stops when the key is released or when you
    leave the screen). The box shows `-` when the mode is OFF.
  - **Frame** shows the current slice's start frame in hex. **EDIT +
    [LEFT/RIGHT]** nudges it by ten frames (zooming onto the marker while
    **EDIT** is held), **EDIT + [UP/DOWN]** nudges by one hundred and
    returns to the full view. A marker stays inside its own slice: it
    cannot cross the previous or next slice start. **EDIT + OPT** deletes
    the slice.
  - **B** exits to the instrument screen from the Slice row like everywhere
    else on this screen; **SHIFT + LEFT** exits from anywhere.
  - **LAZY workflow**: with LAZY selected, tap **PLAY** to start a one-shot
    full-sample playback - it plays once from the start and stops by
    itself at the sample's original pitch and speed (a green two-pixel
    marker follows the position on the waveform); tapping **PLAY** again
    stops it early. While it plays,
    every **EDIT** click drops a slice at the playback position, no matter
    where the cursor is (slices closer than 50 ms to an existing one are
    rejected with `Too close to slice`). Playing the sample again keeps
    the existing chops and adds new ones at every tapped position, in
    order - the **Count** box refreshes with every dropped slice and the
    **Slice** browser follows the new one. The same browsing and editing
    rules as the other modes apply to hand-placed slices: **EDIT +
    direction** on **Slice** browses them, **Frame** adjusts the selected
    start, **EDIT + OPT** deletes the current slice. The Frame cell dims
    while the playback-drop is armed. **SHIFT + PLAY** is not intercepted
    in LAZY: it starts phrase playback like on every other screen. LAZY
    slices play back in the sequencer just like EQUAL and AUTO: phrase
    notes map chromatically from **C-0** onto the hand-placed slices,
    wrapping around past the last one.
  - **Combo hints**: while the cursor rests on a Slice row cell and no other
    message is showing, the status bar shows that cell's hint and keeps it
    up while the cursor rests there. On **Mode** the hint is bound to the
    active mode and shows only the description (the mode name is already in
    the cell): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - switching modes
    swaps the text immediately, and OFF clears the bar. On **Count** it
    shows `Adjust the number of slices`, on **Slice**
    `Browse slices`, on **Frame** `Adjust slice start`. Any action message
    replaces the hint until it expires.
- **Process** selects a destructive editing operation: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** or **Reverse** (EDIT + left/right cycles, tap cycles forward, EDIT + OPT sets none). **GO** (same row) runs the selected operation on the current selection, or on the whole sample when the selection is empty. **Crop** keeps only the selection; **Normalize** scales the selection so its peak reaches full scale (both channels share one gain so the stereo image is preserved); **Delete** removes the selection and joins the tails (deleting the whole sample is rejected); **Silence** zeroes it; **Fade In**/**Fade Out** ramp the selection linearly from/to silence; **Reverse** plays the selection backwards (frames are swapped in place, both channels of a frame move together, length and markers are unchanged). Crop and Delete require a selection — with an empty selection they show `Select region first`. Every operation pauses audio briefly, keeps a one-level undo, and marks the sample as changed in RAM: the file on disk is not touched until the Save flows (see below), and leaving the screen discards the undo slot.
- **UNDO** (next to GO) swaps the sample back with the state before the last operation. It is dimmed until an operation runs, toggles between the pre-op and post-op states on repeated presses, and is cleared when the screen is re-entered.
- **File** holds the save flows. The instrument stores the full path of the WAV it was loaded from; these flows write that file or point the instrument at a new one. They never touch the instrument name.
  - The File field cycles between **Save** and **Save As** (EDIT + left/right or tap); **GO** (same row) runs the shown action.
  - **Save** overwrites the WAV the sample was loaded from. For a sample
    without slices it asks `Overwrite <name>?` first. A sliced sample first
    opens the **SAVE SLICES** dialog asking where the slice points belong:
    **SAVE TO SAMPLE** writes them into the WAV as cue chunks (portable -
    DAWs show them as markers) and keeps them in the project too,
    **SAVE TO PROJECT** writes a plain WAV and keeps the points in the
    project only, **CANCEL** aborts. A `Don't ask again in this project`
    checkbox stores the picked destination with the project and skips the
    dialog from then on (the choice is saved as `- Sample save choice:` in
    the .cct and can only be changed by editing the project file). The
    dialog is skipped for samples without slices. **Save** is dimmed (and
    skipped in navigation) until a sample with a file path is loaded. A
    failed write keeps the dirty marker and shows the error.
  - **Save As** writes the current sample to a new file: enter a file name (pre-filled with the current name without extension), then pick a folder in the browser. The sample is written as `<folder>/<name>.wav`, the instrument points at the new file, and the folder is remembered as the default sample folder. It is dimmed (and skipped in navigation) until sample data is loaded, so a freshly loaded sample can be given a file path. Paths longer than 255 characters are rejected.
  - The `*` marker before the filename means the sample in RAM differs from the file on disk (any process operation sets it, Save and Save As clear it). The marker is session-only: it is not saved with the project and resets when the screen is re-entered. If several instruments reference the same WAV file, saving one overwrites the file for all of them.
- On the Sample instrument screen, use **EDIT + [LEFT/RIGHT]** to load the previous or next WAV in the same folder.
- **Pitch** transposes by semitones (`-48` to `+48`).
- **Loop** selects Off, Loop or Ping-Pong.
- **Speed** controls granular time-stretching from `0%` to `500%` (`100%` is normal).

Unsupported WAV formats display an error. Convert unusual files to `PCM8`, `PCM16` or `PCM24` WAV before importing them.

#### 2xSCWF

2xSCWF is a dual single-cycle waveform oscillator. Load 1 mono WAV containing exactly 1 period into each oscillator. Both oscillators read forwards and wrap at the end of their cycle.

Mix crossfades A and B.

Detune is fine and exponential from unison through `+200 ct`, then moves in semitone steps from `+3 st` to `+24 st`.

2xSCWF treats each file as a 1-cycle table and reads it exactly once per oscillator cycle. This lets you blend and detune 2 single-cycle waves.

The factory `waveforms/AKWF/` folder contains waveforms from the Adventure Kid library. Plenty more are available online if you want to expand the collection.

#### BYOWTBL

BYOWTBL is a dual wavetable oscillator compatible with Serum tables. Each oscillator loads a mono WAV containing consecutive single-cycle frames. `Pos A` and `Pos B` scan their tables independently from `00` to `FF`. The engine interpolates linearly within each frame and between adjacent frames. `Mix` crossfades the oscillators, and `Detune` offsets oscillator B.

### FM instruments family

#### Native OPLL and VRC7 instruments

The Instrument Type selector has an FM group with **OPLL / MSX** (YM2413)
and **VRC7** (DS1001). Each now offers **73 programs** in scrolling Bank / Preset
lists: the original 15 tones, 40 additional distinct tones from emu2413's
YM2413/VRC7/YMF281B tables, and 18 ChooChoo-authored two-operator programs.
Exact duplicate tone bytes are omitted within each engine. The additional
palettes use the chip's programmable tone slot; they do not expand the physical
ROM. Program zero identifies a custom tone. EDIT + left/right selects the
previous or next preset within the bank filter. Fine ct adjusts tuning from
-100 to +100 cents.

In the program list, EDIT + PLAY auditions the highlighted sound; release the
buttons to stop. EDIT commits and OPT cancels. Browsing/audition does not change
the instrument, table or song. The normal Instrument-page audition gesture
continues to work after selection. Native chip envelopes supply attack/release;
the optional Amp env adds a software ADSR (see Native chip controls below).
There is no full operator editor.

#### AdLib / OPL2 and OPL3

FM also includes AdLib / OPL2 (YM3812) and OPL3 (YMF262). Bank filters the
factory list; Preset opens category groups and an All view. EDIT + PLAY auditions
before selection, EDIT selects, OPT cancels, and EDIT + left/right on Preset
loads the previous/next matching entry. Fine ct adjusts local tuning. Mode shows
2 operator, 4 operator, or Dual voice. OPL2 hides incompatible OPL3 patches;
OPL3 can play the shared two-operator collection.

Bank and preset popup titles identify the current engine. Switching engine or
instrument slot resets the bank filter to **All banks**, including switching
between the compatible AdLib and OPL3 engines. Returning from a popup within the
same slot keeps the chosen filter. Cancelling a DX7 bank import restores the
previous filter.

Preset confirmation also publishes edits made on button release to the next
UI/audio tick. Playing songs no longer need another button press or a transport
restart to receive that patch. Output still follows the configured audio buffer
and the engine's envelope/retrigger behavior.

The factory collection contains 697 source entries from The Fat Man 2-op,
The Fat Man 4-op and DMXOPL3, with 589 normalized unique patch identities.
Aliases retain source names and attribution. Source-native operator levels,
LFO-depth flags, note offsets, fixed percussion pitches and dual-voice tuning
are retained. Tracker volume uses software amplitude; it does not reproduce
the original MIDI players' volume curves or velocity-offset policies. Source
release-duration estimates do not cut off sustained notes. Some effects have
slow attacks: hold Seashore rather than expecting a short click to reveal it.

Selecting a preset leaves the slot's table and track-owned inserts unchanged.

#### Sega PSG and Game Boy native instruments

CHIP includes Sega PSG, GB Pulse and GB Noise. On Sega PSG, **Bass range:
Extended** keeps low notes distinct. Choose **Chip** for the original limited
range, where the lowest notes converge. Fixed-rate noise keeps its original
pitch; tone-derived noise follows the selected bass range.
The GB instruments use DMG pulse/noise registers; no Game Boy wave channel is
exposed. Their authored preset lists now contain **24, 24 and 28** presets
respectively. The additions cover linked/periodic Sega noise, GB pitch sweeps,
short percussion, metallic noise, drones and rises. These are original programs,
not extracted game sounds.
The page offers native mode/duty/width, envelope/sweep/noise controls as
applicable, plus a software amplitude ADSR.

#### DX7 FM

Select **Type → FM → DX7 FM**; DX7 is the seventh entry, after Arcade / YM2151.
DX7 FM uses six operators. Its page offers Bank, Preset and Fine ct alongside
the common instrument settings. Preset browsing
uses bank/category lists. Hold EDIT+PLAY to hear the highlighted sound; releasing
stops audition. EDIT alone commits on release; OPT cancels. Loading a sound
copies its complete patch without changing tracker tables or track insert FX.

For a persistent personal library, put `.syx` files in
**`instruments/banks/dx7/`** beside the existing instrument library. Subfolders
are supported. Open **DX7 → Bank**: each file appears as a named bank, ready for
Preset browsing. Reopen Bank after adding or removing files; no conversion or
Load Instrument step is needed. A standard original DX7/TX7 bank contains
**32 voices**. A file with four bank messages appears as four numbered banks
(128 voices total). Single-voice files are accepted too. The selected patch is
saved with the instrument, so removing its source bank does not change the song.

**Load Instrument** also accepts original DX7/TX7 `.syx` single voices and
32-voice banks. Select a patch to use it, or cancel to keep the current
instrument. Use the Bank folder above for a persistent library; save the song
or a `.cni` to keep an edited patch. Unsupported Yamaha formats are rejected.

DX7 has a limit of **16 active notes across the song**, including chord voices
and release tails. When the limit is reached, release tails give way first, then
quiet held notes, so new attacks remain playable. Preset audition is unavailable
while the song is playing.

#### FM operator editor

Genesis FM, Arcade FM, OPL2, OPL3, OPLL and VRC7 instruments have an **EDIT**
button to the right of Fine ct. It opens a table of the patch parameters:

- Genesis FM / Arcade FM: algorithm, feedback, pan, AMS, PMS, operator mask and
  LFO settings, then MUL, DT, TL, KS, AR, D1R, D2R, RR and D1L for the four
  operators, plus SSG (Genesis) or DT2 and AM (Arcade).
- OPL2 / OPL3: feedback, connection and pan per voice, deep vibrato and tremolo,
  then MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS and KSR per
  operator. On OPL3 the mode (2op, 4op, dual) can be changed here.
- OPLL / VRC7: the custom tone registers (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Editing a ROM preset turns it into
  "Custom".

Left/Right and the usual edit keys change values; the instrument keeps playing
as usual so you can hear changes. The status bar describes the parameter under
the cursor. OPT or Left+Shift returns to the instrument screen. DX7 and SID
have no operator editor.

Genesis FM (YM2612) and Arcade FM (YM2151) use the same Bank/Preset browser,
audition and fine-tune controls as the other FM engines. Their native envelopes
control the release; changing tracker volume or pitch does not retrigger them.

#### FM factory and USER presets

Native chip instruments use **Bank:** to choose ALL, an included collection or
USER. **Preset:** chooses a sound within that selection. Hold EDIT and press
Left or Right on either row to cycle; a short EDIT tap opens its chooser.

Included collections are ZIP packs under `instruments/FACTORY/`. USER files go
under `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse`, or `gb-noise`.
Browse folders directly or put a ZIP of compatible presets there. ZIPs remain
intact on disk. All these folders accept compatible `.cni` files; CNI is
ChooChoo/ChipNomad's portable instrument format, not a chip manufacturer's standard.
The USER browser also reads the following source formats, including inside ZIPs:

| USER folder | Additional formats |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 `.syx` single voices and 32-voice banks |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | VOPM `.opm` banks, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | Furnace `.fui` custom patches and fixed ROM selections |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

A bank opens into compatible voices. Unsupported files or features report a
reason; incompatible voices in a mixed bank are excluded while their source
numbers remain stable. Loading is transactional: a rejected preset leaves the
current instrument unchanged. Source presets replace the instrument's tracker
table with an empty one, except DX7 SysEx, which preserves its existing table.
CNI restores its saved table. Loaded patches are owned by the song and continue
to work after moving or deleting the source pack.

WOPL banks can contain unused slots without an explicit blank flag. The browser
omits unnamed slots whose active operators are fully attenuated and have no
attack. Unnamed playable voices remain available as `Program N [Bbank]` or
`Drum N [Bbank]`, using the source's zero-based program and bank numbers.
An unnamed single OPLI instrument uses its filename. Named silent patches are
retained.

Furnace, DefleMask and GoatTracker files are imported as instruments, not whole
songs. Animated source macros, samples and song-specific playback settings may
not transfer. Put Game Boy files in the intended Pulse or Noise folder.

USER preset cycling continues across files, folders, banks and ZIP packs in
both directions, wrapping at the ends. Reopening the USER browser follows the
current selection. ALL includes USER sounds under Unsorted for categorized
engines; USER remains a separate top-level choice. Sega PSG and Game Boy use
flat ALL/Factory lists and a hierarchical USER browser, without an Unsorted
panel. Older DX7 files in `instruments/banks/dx7/` remain accessible in USER.

An engine shows only its compatible included collections. Factory Presets
combines the included OPLL/VRC7 tone sets for the selected engine; DX7 combines
ChooChoo and YSE originals. Downloaded named collections remain separate.
Fat Man 2-op is listed under OPL2 and Fat Man 4-op under OPL3. Compatible OPL2
CNI files may still be used in OPL3 USER folders.

#### Native chip controls

Native instrument waveform previews refresh when their controls are redrawn.
FM instruments can overlay their optional Amp env curve; SID keeps its waveform
preview without that FM-only overlay, including when browsing banks and presets.

All seven FM engines offer **Bright**, **Feedback**, and **Amp env**. Bright
ranges from -63 to +63, with zero preserving the patch; it changes modulation
operator levels while retaining carrier levels. Its audible effect depends on
the algorithm. Feedback defaults to **Preset**, or overrides the native feedback
with 0–7. These controls do not rewrite the saved native operator bytes.

Amp env defaults to **Bypass**. Select **ADSR** to add attack, decay, sustain,
release and shape around the native sound. A/D/R use the shared 0–5 second
quadratic range; sustain runs from silence to full level. Native envelopes still
run, so this envelope cannot extend a sound beyond its native release. Preset
browsing preserves the slot's FM amp and tone controls.

The following phrase FX also appear as supported Modulation and motion-recording
destinations. Values in the FX column are hexadecimal. Settings apply to playback
without changing the saved instrument. Native GB sweep and envelope controls
latch at the next note trigger; duty/width and noise frequency can change live.

| Engines | FX | Control |
|---|---|---|
| Native FM | `OL1`–`OL6` | Absolute operator output levels: OPLL/VRC7 modulator and OPL `00–3F`; OPLL/VRC7 carrier `00–0F`; Genesis/Arcade `00–7F`; DX7 `00–63` (0–99). Higher means greater output. Only supported operators appear. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Operator 1 attack, decay, release and sustain attenuation, `00–0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00–0F` | Operator 1 frequency multiplier. |
| Genesis/Arcade | `LFR` | Native LFO rate: Genesis `00–07`, Arcade `00–FF`. |
| Arcade | `LAD`, `LPD 00–7F` | Separate LFO amplitude and pitch depths. |
| Genesis/Arcade | `LAS`, `LEN` | Native amplitude sensitivity `00–03` and LFO enable `00–01`. |
| Genesis/Arcade | `LPS 00–07` | Native pitch sensitivity. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00–0F` | Native attack, decay, sustain and release. Time values increase toward `0F`; sustain increases toward full level. |
| SID | `SPR 01–10` | Silent partner frequency from 1× to 16×; affects ring modulation and hard sync. |
| All native FM | `FBK 00–07` | Absolute native feedback, initialized from the instrument. |
| All native FM with Amp env enabled; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00–FF` | Attack, decay, sustain, release, shape. These do not enable a bypassed FM amp. |
| Sega PSG | `CMD 00–02`, `CNR 00–03` | Tone / white noise / periodic noise; three fixed noise rates or tone-derived rate. |
| GB Pulse | `CMD 00–03` | Native pulse duty. |
| GB Noise | `CMD 00–01`, `CND 00–07`, `CNS 00–0D` | Noise width, clock divisor and shift. |
| GB Pulse | `CSP 00–07`, `CSS 00–07`, `CSD 00–01` | Sweep period, shift, downward direction. |
| GB Pulse / Noise | `CEI 00–0F`, `CEP 00–07`, `CED 00–01` | Native envelope initial level, period, rising direction. |

Native FX selection uses the instrument in the phrase row's `I` column, or the
active instrument found by looking backward when `I` is blank. The FX popup
shows a short control description, its current preset value and valid command
range in a compact block. Selecting a different
native effect starts at that value; reopening the same effect preserves its
edited value. A multi-row selection resolves each row's instrument separately.
Tables use their instrument context. Native value edits stop at their legal
endpoints, including duty, noise, sweep, ADSR, feedback and operator levels.
The values are hexadecimal: for example, DX7's maximum `63` means decimal 99.

#### SID instruments and Phrase FX

Choose **CHIP → SID**. Bank offers **ChooChoo SID Originals** and **SIDkit
Effects**. Browse, audition and save them like the other native-chip presets.

The instrument page exposes waveform/pulse width, filter mode/cutoff/resonance
and native ADSR controls. **EDIT**, beside the preset name, opens **SID EDIT**.
Wave and PW have separate labels; Filter has a mode row, then Cutoff and Reso;
ADSR labels sit beside their values. Touching each field selects that exact
control. Existing parameter ranges and fine/coarse adjustment behavior are retained.

SID EDIT uses the FM editor's compact rows, theme colors, hexadecimal values and
EDIT+direction controls. OPT or Left+Shift returns; EDIT+PLAY auditions the
instrument. The page edits the existing stored patch directly, so changes are
saved with the instrument and project.

Imported SID programs can own a waveform, pulse, filter or pitch control. A
`*` marks such a control; select it to see which imported macro or table owns
it. Change the source program to alter it. Controls without `*`, and Phrase or
Table FX, remain editable.

SID USER shows one collection per ZIP and keeps model variants such as `6581`
and `8580` in the sound name. A single imported SID file uses its filename,
which is often clearer than its embedded name.

SID EDIT groups waveform, ring/sync, ADSR, pulse width, filter, motion,
arpeggio, pitch sweep and vibrato. The status bar identifies each control and
its range. Ring and sync each use two of the four available SID voices, so a
SID chord may need fewer notes when either is active.

| FX | Range | Action |
| --- | --- | --- |
| `SCP` | `00–FF` | Pulse-width base, scaled to native 12-bit width; recipe pulse motion remains active. |
| `SCT` | `00–FF` | Cutoff base, scaled to native 11-bit cutoff; recipe filter motion remains active. |
| `SRN` | `00–0F` | Native resonance. |
| `SWV` | `00–07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00–07` | Filter mode bits: 1 low-pass, 2 band-pass, 4 high-pass; 0 bypass. |
| `SMR` | `00–FF` | Recipe macro clock, scaled from 1 to 200 Hz. |
| `SRG` | `00–01` | Triangle ring modulation with silent partner. |
| `SSY` | `00–01` | Oscillator sync with silent partner. |

Pulse width and cutoff support `SLE`. Discrete waveforms, switches, resonance
and FM feedback retain their useful native steps; wider byte values would not
create more hardware states. `FBK` directly selects feedback `00–07`.
Operator levels use the engine-specific ranges listed above. Brightness and
master adjustments retain the full byte range.

#### SID and native-chip factory and USER presets

Native chip instruments use **Bank:** to choose ALL, an included collection or
USER. **Preset:** chooses a sound within that selection. Hold EDIT and press
Left or Right on either row to cycle; a short EDIT tap opens its chooser.

Included collections are ZIP packs under `instruments/FACTORY/`. USER files go
under `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse`, or `gb-noise`.
Browse your folders directly, or put a ZIP of folders and compatible `.cni`
presets there. DX7 also accepts supported `.syx` single voices and 32-voice
banks, including inside ZIPs. ZIPs remain intact on disk. Direct WOPL/WOPLX loading is not supported;
use compatible CNI presets or convert supported source formats offline first.

USER preset cycling continues across files, folders, banks and ZIP packs in
both directions, wrapping at the ends. Reopening the USER browser follows the
current selection. ALL includes USER sounds under Unsorted for categorized
engines; USER remains a separate top-level choice. Sega PSG and Game Boy use
flat ALL/Factory lists and a hierarchical USER browser, without an Unsorted
panel. Older DX7 files in `instruments/banks/dx7/` remain accessible in USER.

An engine shows only its compatible included collections. Factory Presets
combines the included OPLL/VRC7 tone sets for the selected engine; DX7 combines
ChooChoo and YSE originals. Downloaded named collections remain separate.
Fat Man 2-op is listed under OPL2 and Fat Man 4-op under OPL3. Compatible OPL2
CNI files may still be used in OPL3 USER folders.

## 6. Modulation and motion recording

Each instrument can have up to 4 modulation slots. Audio instruments can target **Instrument Pan** or **Track Pan**; both use `00` for left, `80` for centre and `FF` for right.

The available modulation types are:

- ADSR - classic attack-decay-sustain-release envelope
- AHD - attack-hold-decay envelope
- LFO - low-frequency oscillator with several shapes
- SLFO - LFO with a period multiplier for slow cycles
- FLFO - audio-rate LFO that can reach the kHz range; FLFO can be heavy on the CPU
- SLIN - linear joystick control
- SRAT - rate-based joystick control

The list of modulation destinations depends on the instrument type.

When an ADSR or AHD envelope targets Volume, it becomes the instrument's volume envelope. An LFO targeting Volume offsets the current output volume instead.

The modulation amount can be positive or negative. Its range depends on the destination. If the destination's full range is `127` or less, as it is for most parameters, the amount defines an absolute range. Wider destinations such as pitch use a scaled amount.

All duration parameters, including attack, hold, decay and period, are measured in ticks.

LFO shapes:

- Tri - bipolar triangle wave
- Sin - bipolar sine wave
- UniTri - unipolar triangle wave
- UniSin - unipolar sine wave
- RampDn - linear ramp down (saw wave)
- RampUp - linear ramp up (saw wave)
- ExpDn - exponential ramp down (exponential saw wave)
- ExpUp - exponential ramp up (exponential saw wave)
- Square - square wave
- Random - sample-and-hold random wave
- Wavetable - reads one of the project's 32-step AY wavetables. Values `00` to `0F` map from `-100%` to `+100%` without interpolation.

LFO trigger types:

- Free - restarts the LFO only when the instrument changes
- Retrig - restarts the LFO on every note
- Phrase - restarts LFO and SLFO when playback enters a new phrase
- Chain - restarts LFO and SLFO when playback enters a new chain
- Hold - stops after 1 cycle and holds the last value
- Once - stops after 1 cycle and returns to zero

### Motion recording

- **STICK LIVE** (`L1` by default) applies stick modulation without changing the phrase. In **Settings > Stick live mode**, `HOLD` (the default) enables it while held; `TOGGLE` enables it on one press and disables it on the next; `FREE` leaves it permanently enabled. Releasing the button in `TOGGLE` leaves it enabled.
- During playback, hold **MOTION RECORD** (`L2`) to apply stick modulation and record changed destinations as absolute FX values.
- Hold **MOTION ERASE** (`R2`) to remove matching destination FX from the current row.

The bottom-right `~` indicates active Stick live. Record (`*`, or `!` for overflow) and Erase (`x`) take indicator priority and remain momentary in both modes. Both enable live stick processing while held; releasing them leaves a separately enabled live toggle intact.

The mode is saved with application settings, but the live toggle always starts off on launch. Changing modes clears the toggle: `HOLD` follows any currently held Stick live button, `TOGGLE` waits for a fresh press, and `FREE` is active immediately.

Motion recording writes track FX into the phrase currently playing. It updates matching FX first, then uses empty slots from right to left. It never overwrites a different FX. If all 3 slots are full, that motion is not recorded on the step. A `!` in the bottom-right corner means that more destinations changed than the 3 FX columns could hold.

Motion recording supports Braids, Plaits, Sampler, 2xSCWF, BYOWTBL and Bogie destinations, including active Bogie macros and filter controls where applicable.


## 7. Track insert effects

Each of the eight tracks has two serial slots, **TF1 → TF2**. From Instrument,
use the usual Shift+Up gesture to open MOD, then Shift+Up again for **F: Insert
FX**. Shift+Down returns to MOD. OPT+Left/Right selects the track (1–8), without
changing the instrument selector. The module header opens the existing chooser;
bypass retains the module, configured values, and automation. Parameters are
byte values: left column 1–4, right column 5–8. Context text shows full names and
decoded values. Old projects load with both slots OFF.

The navigation display keeps the full F–M–I–P column visible throughout the
instrument screens, including Insert FX and the instrument pool, and highlights
the current screen.

| Effect | Use it for | Parameters |
| --- | --- | --- |
| Compressor | Even out a track or make it punchier. | **Threshold** starts compression; **Attack** lets transients through; **Release** sets recovery; **Makeup** restores level; **Ratio** sets strength; **Detect** listens to stereo, left or right; **DetFilt** filters that detector; **Mix** blends dry and compressed sound. |
| Distortion | Add aggressive digital drive. | **Input** drives the effect; **Mode** selects Density, Drive, Spiral, Mojo or Dyno; **Output** compensates level; **Mix** blends it in. |
| Doubler | Widen a mono sound. | **Detune** spreads the doubled voices; **Mix** sets the width. |
| TAPESCAM | Add tape colour and instability. | **Input** feeds the tape stage; **Drive** adds saturation; **Color** changes its character; **Wobble** adds pitch drift; **Tone** darkens or brightens it; **Output** sets level. |
| OTT | Apply the bright, dense multiband compression often heard in bass music. | **Depth** is the overall OTT amount; **Time** is how fast it reacts; **Upward** raises quiet detail; **Downward** controls peaks; **Input** and **Output** trim level. |
| Chorus | Thicken and widen a sound. | **Rate** is the movement speed; **Depth** is pitch movement; **Tone** darkens the delayed voice; **Mix** sets wet level; **Feedback** makes the effect more resonant. |
| Flanger | Add a moving comb-filter sweep. | **Rate** is sweep speed; **Depth** is sweep range; **Feedback** adds the metallic resonance; **Mix** sets wet level. |
| Phaser | Sweep notches through a sound. | **Rate** is sweep speed; **Depth** is sweep range; **Feedback** sharpens the sweep; **Mix** sets wet level; **Stages** chooses 4 to 8 notches. |
| Rotary | Make a rotating-speaker stereo movement. | **Speed** is rotation rate; **Depth** is stereo movement; **Drive** adds speaker grit; **Mix** sets wet level. |
| Saturation | Warm, round or thicken a sound. | **Drive** adds saturation; **Tone** darkens or brightens it; **Level** matches output; **Mix** blends it in. |
| Bitcrusher | Make lo-fi, sampled-digital texture. | **Bit depth** lowers resolution; **Rate reduction** holds samples longer; **Tone** filters the result; **Mix** blends it in. |
| Destruction | Break a sound more radically. | **Mode** selects Fold, Clip or Crush; **Amount** sets severity; **Tone** filters it; **Mix** blends it in. |
| Parametric EQ | Carve room or rebalance a track. | **Low/Mid/High gain** cut or boost each band; the matching **frequency** chooses where that band works. Gain `80` is neutral. |
| Trance Gate | Turn sustained sound into a rhythmic pattern. | **Pattern** chooses the rhythm; **Rate** sets its note division; **Length** shortens it; **Gate** sets open-step length; **Phase** rotates it; **Mode** changes direction; **Smooth** softens edges; **Mix** blends it in. |
| Beads | Granulate, freeze or delay captured audio. | **Time** selects the captured audio; **Size** changes grain length and direction; **Density** sets grain rate/randomness; **Pitch** tunes grains; **Scatter** randomises them; **Feedback** recaptures output; **Shape/Freeze** changes the grain window or freezes audio; **Mix** sets wet level. |

**Quick starting points.** `Mix` is dry at `00` and fully processed at `FF`.
For bipolar controls, `80` is neutral. Start modulation effects with modest
Depth and Mix; raise Feedback for a stronger, more resonant sound. Use Output,
Level or Makeup to match the bypassed level before deciding whether an effect
really helps the track.

### Lane FX for Inserts

Use **F11–F18** for TF1 and **F21–F28** for TF2. The Phrase and Table FX
picker only shows parameters used by the inserts on that track; choose the
effect first, then automate its numbered controls. MOD can move those same
controls while the instrument plays.

Start with one effect, set its Mix, then level-match it against bypass. Drive,
makeup and EQ boosts can clip the mix, so watch the Mixer clip indicator.


## 8. Tables

Tables are a core sound design tool in trackers. In Vortex Tracker terms, they combine instruments and ornaments, but they can do much more. If you know LSDj or NerdSEQ, the idea should already feel familiar.

The Pitch column accepts relative (`~`) or absolute (`=`) pitch values in semitones. The Volume column is `00`-`0F`, applies to every engine on top of its envelope, and can be used for a gate: alternate `0F` and `00` then loop with `HOP 00`. The 4 FX lanes mostly match the lanes in a phrase, although a few FX behave differently in tables.

Putting a `TIC` FX on the last table row sets the speed of that column and overrides the instrument's default table speed. Each FX column can run at a different tick speed. The Pitch and Volume columns follow the speed of the 1st FX column.

A 16-row table may look short next to a long Vortex Tracker instrument, but the `HOP` FX can create conditional loops such as "repeat these rows 5 times", as well as nested loops. With loops and independent FX column speeds, those 16 rows can go a surprisingly long way.

It's possible to copy/paste lane FX between phrases and tables ;)

### Retrigger mode

The `Retrig` field at the top of the Table screen selects when an already active table restarts. `Inst` is the default and compatible mode: a phrase row containing an instrument selects that instrument's default table and starts it at row `00`. `Phrase` restarts an active table when playback enters a phrase, `Chain` when it enters a chain, and `Free` leaves it running until the track stops or an explicit command replaces it.

A structural mode never reaches backward in time. If a track has no active instrument table, the first phrase row that introduces an instrument activates its table at that exact row, even if it is row `4`, `8` or `F` of a phrase. Later instrument numbers do not replace an active `Phrase`, `Chain` or `Free` table. This makes tables useful as LFO-like automation without silently simulating earlier phrase steps.

`TBL`, `TBX` and `RET` are always explicit actions: they select or restart immediately, whatever the mode. On a Phrase or Chain boundary, its restart happens before the new phrase row is read, so a `TBL` or `TBX` on that row has the final say. Table row `00` FX therefore run again normally; cumulative FX retain their usual accumulation.

Tables `00-7F` are reserved for default instrument tables. Tables `80-FE` are intended for auxiliary tables started by the `TBX` effect. The lower range also works for auxiliary tables, but using it that way can cause unexpected conflicts and confusion.

### Controls

- **EDIT + [UP/DOWN]** on an FX name column: open the FX selection screen
- **OPT + [DIRECTION]**: move between tables

The FX picker leaves a blank row between its four-row help area and the group list.
Command groups use up to eight columns; Modulation keeps five commands per row,
one row for each modulation slot.

## 9. Instrument Pool and AY Wavetable

### Instrument Pool

The Instrument Pool shows every instrument in the project and lets you reorder them. During playback, it also shows which instruments are currently active.

#### Controls

- **EDIT**: edit the instrument and jump to the Instrument screen
- **SHIFT + OPT**: copy the instrument
- **SHIFT + EDIT**: paste the instrument
- **EDIT + [UP/DOWN]**: reorder instruments
- **EDIT + PLAY**: preview the instrument

### AY Wavetable

Wavetable is an AY Plus software oscillator type. A project can contain up to 256 wavetables, each with 32 steps. All wavetable instruments share the same set of waves. This screen is where you edit them.

AY volume levels are non-linear, so the Standard view waveform is drawn to match the actual output levels. Double-tap **OPT** to switch between the Standard view and the persisted **LFO view**, using the same shortcut as Song/Live Mode: the LFO view draws the raw `0`–`15` steps linearly, with the centre line between `7` and `8` as the modulation zero point.

The screen has 2 logical rows. The top row contains the **Load** and **Save** buttons, while the 2nd contains the wavetable editor.

#### Controls

- **OPT + [LEFT/RIGHT]**: move through the wavetable list
- **OPT + [UP/DOWN]**: move through the list by 16 waves
- **SHIFT + OPT**: copy the wavetable
- **SHIFT + EDIT**: paste the wavetable
- **OPT + PLAY**: switch between Standard and LFO view

## 10. Tracker FX

Track FX force sequencer or sound-engine values on individual steps. Other grooveboxes often call these "parameter locks". Each Phrase row has 3 FX slots.

Each FX has a 3-letter command and a hexadecimal value. The in-app help panel gives a short description of the selected command.

### Sequencer FX

| FX | Value | Detailed behaviour |
|---|---|---|
| `ARP` | `XY` | Arpeggiates the base note, `+X` steps and `+Y` steps. `37` produces a minor-chord pattern. |
| `ARC` | `XY` | `X` selects the arpeggio direction or range mode; `Y` is its speed in ticks. |
| `PVB` | `XY` | Pitch vibrato: `X` is speed and `Y` is depth. In Linear mode, depth uses `10-cent` steps. |
| `PBN` | signed `XX` | Adds `XX` pitch units every phrase/table row; `FF` means `-1`. Use `00` to stop. |
| `PSL` | `XX` ticks | Slides from the preceding pitch to the new note over `XX` ticks. |
| `PIT` | signed `XX` | Accumulated relative offset in pitch-table steps. |
| `FIN` | signed `XX` | Accumulated fine offset in cents with Linear pitch, period units otherwise. |
| `PRD` | signed `XX` | Accumulated relative oscillator-period offset. |
| `VOL` | signed `XX` | Accumulated relative volume offset (`FF` is -1, not full volume). |
| `VSL` | signed `XX` | Adds `XX` to volume on every phrase/table row. Use `00` to stop. |
| `PAN` | `00-FF` | Absolute instrument panorama: left, centre (`80`), right. |
| `TPN` | `00-FF` | Absolute mixer-track panorama: left, centre (`80`), right. |
| `RET` | `XY` | Retriggers every `Y` ticks; `X` applies a volume change. `Y=0` stops retriggering. |
| `DEL` | `XX` ticks | Delays note-on. A delay longer than the current groove step skips the note. |
| `OFF` | `XX` ticks | Sends note-off after `XX` ticks and enters an ADSR release stage. |
| `KIL` | `XX` ticks | Hard-kills the voice after `XX` ticks without running ADSR release. |
| `TIC` | `XX` ticks | Sets table ticks per row. In a table it changes that FX column's speed. |
| `TBL` | `00-FE`, `FF` off | Replaces and restarts the instrument table immediately; `FF` stops it. |
| `TBX` | `00-FE`, `FF` off | Starts or replaces an auxiliary table immediately; `FF` stops it. |
| `THO` | row `XX` | Jumps all instrument-table columns to row `XX`. |
| `TXH` | row `XX` | Jumps all auxiliary-table columns to row `XX`; it is not used from inside a table. |
| `GRV` | groove `XX` | Selects a groove for the current track. |
| `GGR` | groove `XX` | Selects a groove for every track. |
| `HOP` | `XY` | Jumps to row `Y`, `X` times; `X=0` loops forever. Table HOP affects its own column. |
| `SNG` | signed `XX` | Moves playback by `XX` song rows. |
| `PRO` | `00-64` | Evaluates an absolute trigger probability from `0%` to `100%`. |
| `MOD` | `AB` | Triggers on pass `A` of a `B`-pass cycle. |
| `SPD` | signed `XX` | Selects a persistent per-track clock ratio; see the speed table below. |
| `SLE` | `00-FF` ticks | Sets persistent per-track glide time for continuous engine FX. `00` is immediate. The setting resets to `00` when playback stops. |

### Audio FX sends

| FX | Value | Detailed behaviour |
|---|---|---|
| `RSN` | `00-FF` | Sets this track's reverb send until the next note trigger. |
| `DSN` | `00-FF` | Sets this track's delay send until the next note trigger. |

### ADSR / Trigger FX

| FX | Value | Detailed behaviour |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Override attack, decay, sustain, release or envelope shape. |
| `TDC` / `TCL` | `00-FF` | Override Trigger-mode Decay or Color on Plaits and Plaits-Alt. |

#### Conditions

Inspired by Swedish "trig conditions", the tracker supports trigger probabilities and modulo conditions.

| FX | Value | Meaning |
|---|---|---|
| `PRO` | `00-64` | Absolute trigger probability from `0%` to `100%` |
| `MOD` | `AB` | Trigger on iteration `A` of `B`; for example `12`, `22`, `14`, `34` |

`MOD` counters are local to the track and phrase. Invalid combinations, such as `A` greater than `B`, do not trigger.

`MOD34` triggers on the 3rd pass of every 4-pass cycle. `MOD1F` triggers on the 1st pass of every 16-pass cycle.

#### Playback speed

Like NerdSEQ, ChooChooTracker supports an independent playback speed for each track.

`SPD` reads its value as a signed byte. `00` is normal speed, positive values make the track faster, and negative values make it slower. The setting remains active until another `SPD` command changes it.

| Value | Speed |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Older projects without signed `SPD` support keep their original `00-10` mapping, so they still play as saved.

Very high multipliers can exceed the resolution of the tracker tick scheduler and have not been hardware-tested.

### Scale & chords FX

#### Scale automation

`SCL XY` changes the global playback scale and root without changing the notes stored in phrases. `X` selects a scale (`0` Chromatic, `1` Major, `2` Minor, through `C` Custom); `Y` selects the root (`0` C through `B` B). The command applies to phrase FX only, not tables. If several tracks issue `SCL` on the same tick, the lowest-numbered track wins.

`SCL` belongs to Quantizer mode. With **Note Lock** active the command is not offered in the FX list, and any `SCL` already written into phrases is ignored while the mode is on.

#### Chords

`CRD XY` turns the note on its own Phrase row into a chord. `Y` selects: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9, or `F` Min9. `X` selects one of 16 voicings: `0-3` are a rising closed inversion cycle; `4-7` add drop-2; `8-B` add drop-3; and `C-F` combine both drops. The cycle continues through an extra octave for triads and power chords, so every value is distinct; larger values become progressively wider and less conventional. For example, `CRD 4Y` is drop-2 root position and `CRD DY` is first inversion with both drops. At the bottom or top of the pitch table, voices are clamped to its range. The command is not persistent: write it on every chorded row. Each generated note is quantized independently when Scale is active, in both Quantizer and Note Lock modes. It is available to software engines only; AY instruments ignore it and remain monophonic.

### Modulation FX

Phrase and table FX can change a modulation slot without editing the instrument:

| FX pattern | Meaning |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Relative Amount offset for modulation slots 1-4 |
| `M11`, `M12`, `M13`, `M14` | Relative P1-P4 offsets for modulation slot 1 |
| `M21`, `M22`, `M23`, `M24` | Relative P1-P4 offsets for modulation slot 2 |
| `M31`, `M32`, `M33`, `M34` | Relative P1-P4 offsets for modulation slot 3 |
| `M41`, `M42`, `M43`, `M44` | Relative P1-P4 offsets for modulation slot 4 |

The value is interpreted as a signed `8-bit` relative change (`01` adds `1`, `FF` subtracts `1`). Repeated commands accumulate. Effective values are clamped to their valid range.

`P5` is not exposed as a phrase or table FX. It is available as the `M1–M4 Wavetable` modulation destination, so one modulator can scan the AYWavetable index of another.

| Modulation type | P1 | P2 | P3 | P4 | P5 when Shape is Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Unused |
| AHD | Attack | Hold | Decay | Unused | Unused |
| LFO | Shape | Trigger mode | Period | Unused | AY wavetable index |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | AY wavetable index |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` to `20 kHz`) | Unused | AY wavetable index |

### Braids FX

| FX | Value | Meaning |
|---|---|---|
| `BMD` | `00-2E` | Braids model |
| `BTM` | `00-FF` | Absolute normalised Timbre |
| `BCL` | `00-FF` | Absolute normalised Color |
| `BCF` | `00-FF` | Exponential cutoff, `20 Hz` to `20 kHz` |
| `BRS` | `00-FF` | Exponential filter resonance |

### aChChid FX

| FX | Value | Meaning |
|---|---|---|
| `ASL` | `00-FF` | Slide to the note without retriggering. `00` is `60 ms`; higher values extend the glide. |
| `ADC` | `00-FF` | Decay override: `200 ms` to `2 s`. |
| `AAC` | `00-FF` | Accent amount: none to maximum. |
| `ATM` | `00-FF` | Braids Timbre override; active only in Braids wave mode. |
| `ACL` | `00-FF` | Braids Color override; active only in Braids wave mode. |
| `ACF` | `00-FF` | Exponential 303 filter cutoff, `20 Hz` to `20 kHz`. |
| `ARS` | `00-FF` | 303 filter resonance, none to maximum. |
| `AEM` | `00-FF` | 303 filter envelope modulation, none to maximum. |

### Bogie FX

| FX | Value | Meaning |
|---|---|---|
| `DMD` | `00-0B` | Selects the Bogie model until the next trigger. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Override Decay, Tone, Sweep, Noise, FM or Drive until the next trigger. All six macros are available on every model. |
| `DCF` | `00-FF` | Filter cutoff, mapped logarithmically. |
| `DRS` | `00-FF` | Filter resonance. |

### Sintered FX

| FX | Value | Meaning |
|---|---|---|
| `SMD` | `00-05` | Selects the Sintered model until the next trigger. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Override Decay, Mod, the first two model macros, Motion or the final model macro until the next trigger. |
| `SCF`, `SRS` | `00-FF` | Filter cutoff and resonance. |

### MIDI Out FX

| FX | Value | Meaning |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Sends a MIDI CC, rescaled to `0-127`. Which CC number each slot sends to is set per-instrument (`MC1 number`-`MC4 number` on the instrument screen), not by the FX itself - a slot with no number set is inert. |

### Plaits FX

| FX | Value | Meaning |
|---|---|---|
| `PMD` | `00-17` | Plaits engine |
| `PHA` | `00-FF` | Absolute normalised Harmonics |
| `PTM` | `00-FF` | Absolute normalised Timbre |
| `PMO` | `00-FF` | Absolute normalised Morph |
| `PAX` | `00-FF` | Main/Aux blend: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Exponential cutoff, `20 Hz` to `20 kHz` |
| `PRS` | `00-FF` | Exponential filter resonance |

### Sample FX

| FX | Value | Meaning |
|---|---|---|
| `SPL` | `00-03` | Playback mode: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Play the numbered slice (`01` = first slice) regardless of the note; `00` keeps normal note mapping. Requires slice mode enabled on the instrument |
| `SPT` | signed `XX` | Sample transposition in semitones |
| `SST` | `00-FF` | Normalised playback start |
| `STA` | `00-FF` | Alias of `SST` (normalised playback start) |
| `SEN` | `00-FF` | Normalised playback end |
| `SVL` | `00-FF` | Absolute sample volume |
| `SCF` | `00-FF` | Exponential cutoff, `20 Hz` to `20 kHz` |
| `SRS` | `00-FF` | Exponential filter resonance |
| `SSP` | `00-FF` | Sample speed, mapped from `0%` to `500%` |

### AY FX shared by AY instruments

| FX | Value | Detailed behaviour |
|---|---|---|
| `AYM` | `XY` | `X` is envelope shape; `Y` selects off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | signed `XX` | Accumulated relative noise-period offset. |
| `NOA` | `00-1F`, `FF` | Absolute noise period; `FF` yields noise-period priority to earlier tracks. |
| `ERT` | any | Retriggers the current hardware envelope shape. |
| `EAU` | `XY` | Automatic envelope ratio `X:Y`; `X=0` disables it. |

### AY Classic-only FX

| FX | Value | Detailed behaviour |
|---|---|---|
| `EVB` | `XY` | Envelope-period vibrato: `X` speed, `Y` depth. |
| `EBN` | signed `XX` | Adds `XX` to envelope period every phrase/table row. |
| `ESL` | `XX` ticks | Slides from the preceding envelope period to the new value. |
| `ENT` | note `XX` | Sets envelope period from the pitch-table note shown by the UI. |
| `EPT` | signed `XX` | Accumulated relative envelope-period offset. |
| `EPL` | byte `XX` | Sets the low byte of the envelope period. |
| `EPH` | byte `XX` | Sets the high byte of the envelope period. |

### AY Plus FX

| FX | Value | Detailed behaviour |
|---|---|---|
| `TNN` | note `XX` | Sets the tone oscillator to a pitch-table note, ignoring the row note. |
| `TNP` / `TNF` | signed `XX` | Accumulated tone pitch-step / fine offset. |
| `TRT` | any | Retriggers tone oscillator phase where supported. |
| `ENN` | note `XX` | Sets the envelope oscillator to a pitch-table note. |
| `ENP` / `ENF` | signed `XX` | Accumulated envelope pitch-step / fine offset. |
| `SFT` | `00-07` | Software type: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | note `XX` | Sets the software oscillator to a pitch-table note. |
| `SFP` / `SFF` | signed `XX` | Accumulated software oscillator pitch-step / fine offset. |
| `SRT` | any | Resets software oscillator phase. |
| `SFM` | signed `XX` | Accumulated FM-depth offset. |
| `PWM` | signed `XX` | Accumulated pulse-width offset. |
| `SPL` | signed `XX` | Accumulated pulse low-level offset. |
| `SWT` | signed `XX` | Accumulated wavetable-index offset. |

### AY Sample FX

AY Sample accepts the shared AY commands plus `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` and `SFF` as described above.

| FX | Value | Detailed behaviour |
|---|---|---|
| `SMS` | `00-FF` | Sets legacy AY Sample playback start to `XX x 64` source samples. |

Engine parameter FX apply to their matching instrument type and are reset at the next note trigger unless stated otherwise. `SPD` is intentionally persistent.

The FX selector filters this reference to common commands plus the group supported by the active instrument. The contextual help panel provides value details while editing.

## 11. Mixer, Reverb and Delay

The Mixer is the leftmost main screen. It displays CPU load and clipping warnings. A red `!` in the **CLIP** column marks the track whose dry signal pushed the mix beyond the safe range.

Use it to balance the 8 tracks, shape each track with Tilt EQ, and send audio to the reverb or delay:

| Field | Meaning |
|---|---|
| LVL | Post-engine track level, `000-100` |
| REV | Send to the shared reverb, `000-100` |
| DLY | Send to the shared delay, `000-100` |
| PAN | Folds the audio left or right |
| TLT | Per-track Tilt EQ, `00-FF`; `80` is neutral, low values favour bass and high values favour treble. Low values will introduce a soft overdrive. |
| M | `*` mutes this track |
| S | `*` solos this track |

The mixer is track-based, not instrument-based. If a track changes instruments, its level, PAN, Tilt and sends remain attached to the track. Instruments also have their own `00-FF` **Vol** and **Pan** in the top row of the Instrument screen. Instrument PAN is applied first, then track PAN. Both preserve an existing stereo image at centre; they progressively attenuate the opposite side when moved left or right.

### Master FX

The **MASTER FX** entry opens a two-slot Insert FX sub-menu for the complete
mix. Its effects run after the reverb and delay returns, before the master
volume. Select a module, edit its parameters or bypass it exactly as for a
track insert; press **OPT** to return to Mixer. Master FX settings are saved
with the project and are not automated by phrase, table or modulation.

Effects in this category are the same as on the Insert FX screen.

### Reverb

Use **SELECT + [UP]** from Mixer to open the reverb settings screen.

- **Algorithm**: selects Clouds or Beads. Each algorithm keeps its own controls.
- **Return**: wet reverb level in the master mix.
- **Clouds**: **Time**, **Damping**, plus pre-reverb **LP** and **HP** filters.
- **Beads**: **Time**, native feedback-filter **Tone**, and a pre-reverb **HP** filter.

This is the send reverb section only of Clouds or Beads, not the granular processor.

### Delay

Use **SELECT + [DOWN]** from Mixer to open Delay, then **SELECT + [UP]** to return. Reverb works in the opposite direction: **SELECT + [UP]** opens it and **SELECT + [DOWN]** returns to Mixer.

- **Algorithm**: selects **Ping-Pong** or **Tape Echo**. Changing it clears the current delay tail.
- **Return**: wet delay level.
- **To Reverb**: amount of the wet tape or ping-pong repeats sent into the shared reverb, `0-100%`.
- **Ping-Pong**: **Ticks** sets the delay time; `24 ticks` equal one beat. **Feedback** is cross-feedback, limited to `95%`, and **Filter** is a low-pass filter in the delayed path. The first repeat follows the stereo input; later feedback crosses between left and right.
- **Tape Echo**: **Time** selects a tempo division from `1/16` to `1/2`; **Heads** selects one or more of three virtual tape heads; **Feedback** controls repeats from `0%` to `95%`. When several heads are selected, the loop is normalized so their sum cannot raise the feedback above the selected amount. **Tone** darkens the tape; **Flutter** starts gentle and reaches an exaggerated `20 ms` tape-speed wobble at maximum; **Spring** starts subtle and becomes a louder, longer splash at maximum. The spring is local to Tape Echo and is not fed back into its tape loop or sent to the shared reverb.

## 12. Project screen

The Project screen provides **Load**, **Save**, **New**, **Export**, **Manage** and **Scale** commands, along with filename, title and author metadata.

### Export location

Exports, stems and bounces are written to a per-project folder inside the app's preexisting samples directory:

- **Default**: `<app folder>/samples/Exports/<project name>/`. While the project is unnamed, the folder is `current-project`.
- On the web build the default is `/user/samples/Exports/<project name>/`.
- On Android the samples folder is inside the app's workspace: private to the app in the Play Store build, `samples` in the folder you picked in the direct APK build.

The **Folder** row on the EXPORT screen shows the active destination:

- Tap **EDIT** to pick a custom folder with the folder browser. The chosen folder is used exactly as selected (no project subfolder is added) and is remembered in settings.
- **EDIT + OPT** (clear) resets to the default location.

When a named project is saved under a new name, the default export folder is renamed to match, keeping previously exported files with the project. Renaming does not apply to a custom folder, and the folder is not renamed by autosave. If a folder with the target name already exists, both folders are kept. If the tracker lost track of the folder (for example after a restart), saving a named project renames the existing `current-project` folder instead. Instruments keep working after a rename: loaded sample paths that point into the renamed folder are updated automatically.

**Load** accepts `.mid`/`.midi` files, imported as a new project: notes are grouped by MIDI channel (one channel per track, up to the track count), quantized to 4 rows per beat, and placed on a single default AY instrument - MIDI program numbers have no chiptune equivalent, so pick real instruments afterward. Only the file's first tempo is used (one global tick rate, no per-section tempo changes). The Export screen's **MIDI** row does the reverse: writes the current arrangement's notes, volume and tempo/groove as a Standard MIDI File (one MIDI track per tracker track); [MIDI Out](#midi-out) instruments and other per-row FX beyond volume and the global groove have no MIDI equivalent and are not translated.

**Load** accepts Dirtywave M8 songs (`.m8s`, firmware 2.x-4.x), imported as a new project. Only the structure and the notes come across (song rows, chains with their transpose, phrases with note, velocity and instrument number, note-offs, tempo and song name); notes keep their real pitch (the M8 names octaves two higher: its `C-6` is `C-4` here). Each instrument used by a phrase is imported under the M8 instrument's name. M8 **MACROSYN** instruments become Braids instruments (the 47 shapes are the Braids models, same numbers; timbre, color, filter type, cutoff, resonance and pan are carried over, the cutoff curve being approximate). **SAMPLER** instruments become Sample instruments: the `.m8s` only stores the path of the WAV on the M8 card, so the file is searched under the folder of the `.m8s` and its parents (a copy of the card layout works) and in your current sample folder (`Samples` or `samples` both work); if it is not found the instrument keeps the M8 path and stays silent. Imported samples get +24 semitones in their Pitch setting, as the M8 plays a sample at its original pitch two octaves lower than this app does. **FMSYNTH** becomes a Genesis FM instrument (rough approximation), **WAVSYNTH** and **HYPERSYNTH** become aChChid. **MIDI OUT** becomes a MIDI Out instrument (channel, bank, program and the first four CC numbers). Other types (external) become a default AY instrument - pick real sounds afterward. FX and tables are ignored.

**Load** accepts Little Sound Dj songs (`.lsdsng`, one song per file), imported as a new project. Picking a whole LSDj save (`.sav`, up to 32 songs) opens a list of its songs (slot, name, version, size); pick one to import it (the song being edited when the cartridge was saved is not listed). Only the structure and the notes come across (song rows, chains with their transpose, phrases with note and instrument number, tempo and song name); every note gets full volume, as LSDj has no note volume, and the four LSDj channels PU1, PU2, WAV and NOI become tracks 1-4. Pulse instruments become GB Pulse (duty, envelope and panning are carried over; the pulse sweep is not), noise instruments become GB Noise (envelope, stability and panning), wave instruments become the GB Pulse Hollow Lead preset and kits a default GB Noise. The grooves are imported, and the phrase commands `C` (chord), `D` (delay), `E` (pulse and noise envelope, not on the wave channel), `G` (groove), `K` (kill) and `V` (vibrato) become `ARP`, `DEL`, `CEI`/`CEP`/`CED`, `GGR`, `KIL` and `PVB`. The other commands (`A`, `F`, `H`, `L`, `M`, `O`, `P`, `R`, `S`, `T`, `W`, `Z`), tables, wave frames, kit samples and synth settings are ignored, so a song relying on them will sound different. The tempo is converted for a 6-tick groove.

On desktop and Android, the Export screen's **LSDJ** row does the reverse, in a limited way: tap it and pick an existing `.lsdsng` as a **template** (remembered until you quit, **EDIT + OPT** forgets it). Its song rows, chains, phrases, tempo and title are replaced and everything else (tables, grooves, waves, kits, synth settings) is kept as is. Tracks 1-4 become PU1, PU2, WAV and NOI; GB Pulse and GB Noise instruments are written back as LSDj pulse and noise instruments, any other instrument type as a default pulse instrument. FX, note volumes and note-offs are not exported, and a song using chain `80` or above or phrase `FF` or above cannot be exported (LSDj has 128 chains and 255 phrases). The exported file has not been tested in LSDj itself yet.

On desktop, the Export screen's **M8S** row does the reverse, in a limited way: tap it and pick an existing `.m8s` as a **template** (remembered until you quit, **EDIT + OPT** forgets it). Its song, chains, phrases, tempo and title are replaced and everything else is kept as is, except that Braids instruments are written back as MacroSynth and Sample instruments as Sampler (path `/Samples/<file name>`: copy the WAV there on the M8 yourself), provided the template has an instrument of that type to copy the other parameters from. FX are not exported, and a song using phrase `FF` or above cannot be exported (the M8 has 255 phrases). The exported file has not been tested on a real M8 yet. See [M8 songs](m8s-format.md) for the details.

On desktop, key jazz lets you type the filename, title and author directly on the keyboard instead of using the on-screen virtual keyboard — see [Key jazz](#key-jazz-desktop-only).

### Scale / Quantize

The Scale screen controls the global 12-TET scale engine. The **Mode** row selects between two modes:

- **Quantizer** (default): phrase entry remains chromatic; when enabled, notes are rounded downward immediately before reaching the sound engine, so changing root or scale reharmonizes a song without editing its phrases.
- **Note Lock**: notes typed into phrases are snapped to the nearest note of the scale as they are entered, so only scale notes can be written. Fresh entries and decreases snap down; `EDIT + RIGHT` / `EDIT + UP` raise the note to the next scale note above. Entry always follows the project scale on this screen: changing the root or scale immediately affects newly entered notes on the enabled tracks, while notes already stored stay untouched. `SCL` FX is unavailable in this mode and any `SCL` already written is ignored. Playback quantization of plain notes is bypassed (entry is already locked); chord quantization via `CRD` still applies.

#TODO: explain tempo , midi sync and link

ChooChooTracker saves projects as `.cct`. This is a proprietary format that is easy to read even by a human.

Use **Save** before changing instrument types or loading another project.

## 13. Settings

Use Settings for preferences that stay with the app, rather than with the current project. Project-wide controls such as tempo, groove and mixer levels remain on their own screens.

### Controls

- **Repeat delay** and **Repeat speed** set how soon, and how fast, a held navigation or edit button repeats. Raise the delay if a held press runs away from you; lower it for fast tracker entry.
- **Stick live mode** chooses how the mapped **STICK LIVE** button works: `HOLD` is active while held, `TOGGLE` stays active after a press, and `FREE` is always active. See [Motion recording](#motion-recording).
- **Hide pad w/ gamepad** (touch builds only) hides the on-screen touch controls while a gamepad is connected. OFF by default on Android, ON by default on other touch builds such as PortMaster.
- **Key mapping** assigns keyboard or controller controls. Choose a command, press its new control, then select **Done** to save the mapping.

### Audio

**Buffer size** trades responsiveness for safety. Use a smaller value for tighter live playing; raise it if the audio crackles or the CPU meter is consistently high. The available values are `32`, `64`, `128`, `256`, `512` and `1024` samples. The CPU load display helps when choosing a stable setting.

### MIDI

Choose the input and output devices, assign incoming MIDI channels to instruments, and map MIDI CC controls here. See [MIDI](#15-midi) for connecting a keyboard, controlling the mixer, or sending notes to external hardware.

### Synths

- **AY Quality** chooses the quality level used by AY/YM instruments. **AY Sample dithering** can smooth AY sample playback; leave it off for a more direct, lo-fi result or when targeting original hardware.
- **Braids BITS**, **DRFT** and **SIGN** are global Braids character settings, shared by every Braids instrument. Use them to set the overall resolution, drift and signature character before fine-tuning individual sounds. See [Braids](#braids).

### Mixer

- **Mix volume** is the application's master output level. Set it lower when feeding another mixer or recorder; it does not change the song's track mix.
- **Tilt pivot** sets the centre frequency for every Mixer **Tilt** control in this project, from `250 Hz` to `4000 Hz` (default `1 kHz`).

### Graphics

- **Edit color theme** changes the interface colours. **Load font** loads a `.cnfont` display font.
- **Song waveform** shows or hides the mix waveform on the Song page; see [Song waveform](#song-waveform).
- **Track visuals** chooses, for each track, either **Detailed** (the instrument waveform and overlays) or **Audio waveform** (the track's resulting audio). Use **All detailed** or **All audio** to change every track together.
- **Waveform FPS** sets how often the instrument scope and mixer visualizers redraw, from `1` to `60` frames per second. Lower it if you prefer a quieter display or need more headroom.
- **Renderer** reports whether the application is using `GPU` or `Software` rendering. **Boot screen** selects the `Original` or `Night` title screen.

### Support report and quit

**Support report** creates a small diagnostic file for a bug report. Attach it with the `.cct` project when a problem depends on a particular song. **Quit ChooChooTracker** closes the application cleanly.

### Song waveform

Graphics → Song waveform is OFF by default. Tap EDIT to toggle it, or use EDIT + direction; the choice is saved as an app preference. When enabled, a two-row waveform above the SONG page shows the final mix summed to mono. All other pages retain their full-height layouts, normal spacing and instrument previews regardless of this option. The saved preference remains compatible with earlier versions. SONG touch targets follow the displayed rows in both modes.

## 14. Troubleshooting

### CPU

CPU cost depends on the active engines and effects. Plaits physical models and Clouds Reverb are heavier than basic AY voices. Check performance on the target console, especially with 8 Plaits voices and both sends active. A CPU reading near `100%` can cause crackles or missed audio deadlines.

If you run into pops, crashes or slowdowns, use **Settings > Support report** and attach the generated `support-report.txt` to the bug report. If a specific song triggers the problem, attach that `.cct` too.

### No sound

Check the instrument number, track mute or solo state, track LVL, application Mix volume and instrument envelope. For samples, check that the original WAV still exists at its saved path.

On ArkOS, press **MENU + L3** to toggle the operating system mute. If that does not help, restart the hardware and reconnect your audio interface. Try the "repair audio" option in ArkOS, it can help.

### Input feels wrong

Adjust Repeat delay and Repeat speed in Settings. If a single press moves twice, check that only 1 physical control is mapped to that direction and report the exact screen and shortcut.

## 15. MIDI

Settings > **MIDI** opens this submenu: **MIDI In** / **MIDI Out** pick a connected device, cycling through detected ports with `OFF` at either end; both directions need a device selected.

- **Sound preview from a MIDI keyboard**: with a MIDI In device selected, playing notes on the connected keyboard auditions an instrument on any screen, the same as the on-screen **EDIT + PLAY** shortcut on the [Instrument](#5-instruments) screen. This is preview only - it does not enter notes into the song.
- **Channel mapping**: opens a list of the 16 MIDI channels; assign each one an instrument (`OFF` by default) so notes received on that channel preview that instrument regardless of which one is selected on the Instrument screen - e.g. channel `01` -> instrument `05`. A channel left `OFF` falls back to the currently selected instrument, the original behavior. This mapping is saved to settings.txt.
- **MIDI CC mapping**: maps an incoming controller to an instrument parameter, including its **Volume**, or to global controls. Global rows show `TRK 1`-`TRK 8` instead of an instrument and provide track **Mute**, **Solo**, **Volume**, **Reverb send**, and **Delay send**; **Song play/stop** has no target. A new mapping ignores the CC that learned/configured it and takes effect on the next physical movement, so setting up mappings during playback cannot reset a patch to zero. Mute and solo use `0-63` = off, `64-127` = on.
- **Driving an external MIDI device**: give a track the [MIDI Out](#midi-out) instrument type and set its Channel; triggering notes on that track sends real MIDI to the selected MIDI Out device instead of making sound in ChooChooTracker.

## 16. Credits and licensing

ChooChooTracker is a fork of ChipNomad and retains its MIT licensing approach. Braids, Plaits, Plaits-Alt, Clouds DSP, Warps-derived MME DSP and stmlib code are derived from Mutable Instruments' open-source releases under their applicable MIT notices. Plaits-Alt is sourced from the lylepmills/eurorack Plaits Lab fork; its retained source notices apply. The aChChid engine uses Open303 by Robin Schmidt, copyright 2009, under the MIT License. Bogie and Sintered are original native implementations. MIDI I/O uses RtMidi by Gary P. Scavone, under its MIT-style license. See the packaged license files for exact attribution. Ableton Link is provided with a special licencing agreement from Ableton, is not under MIT licence, and our licence does not carry to your forks or to other projects than ChooChooTracker.
