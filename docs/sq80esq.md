# Experimental SQ80/ESQ instrument — Rev4

This personal experiment integrates the standalone Hybrid C++ engine. Enable with
`CHOOCHOO_EXPERIMENTAL_SQ80ESQ=1`; the personal PortMaster and web targets enable it.
Other native targets default off. Work is on `experimental/sq80esq`.
Engine source/version and the Rev2 integration extensions are documented under
`chipnomad_lib/experimental/hybrid/`. Waveform samples and private presets are
external user data, absent from public packaging.

Choose Instrument → SYNTH → SQ80/ESQ. The standard header retains name, volume,
pan, table and chain-transpose enable. Bank comes first, with Preset below it and Edit beside the preset name, directly above Velocity.
The preset row shows up to nine name characters; the full instrument name remains
in the standard header.
Wheel/Velocity, Pedal and Cutoff/Resonance follow. An asterisk identifies a matching
track's active playback override; it never writes automation into the saved patch.
Velocity is note intensity, which can affect timbre as well as volume.

On Bank, EDIT + Left/Right switches FACTORY/USER. Release EDIT alone to open
the bank chooser. On Preset, EDIT + Left/Right keeps stepping through sounds;
release EDIT alone to open the selected library browser. A chord never opens
the browser on release. The adjacent Edit control opens the patch editor. Velocity stays at its
chosen value while browsing; Wheel and Pedal reset to zero for each new preset.
Saved tracker instruments loaded through the common Load control restore their
saved performance defaults. Hold PLAY in a preset browser to audition; EDIT +
PLAY auditions the instrument/editor sound.

FACTORY contains twelve original Hybrid Starters and Stuart Keith's unchanged
MIT-licensed TRIBEL example, with source/license records. These are parameter
programs, not samples. See `docs/sq80esq-preset-sources.md`. USER remains
`instruments/USER/sq80esq` and is never copied into public releases.

## Roll / table performance FX

Values in the roll use hexadecimal, while the instrument/editor shows decimal.
These absolute controls are available only for SQ80/ESQ instruments:

| Command | Picker name | Range | Action |
|---|---|---|---|
| QCF | Cutoff | 00–7F | Live filter base cutoff; retains preset filter modulation |
| QRS | Resonance | 00–1F | Live filter resonance |
| WHL | Wheel | 00–7F | Preset WHEEL modulation source |
| PED | Pedal | 00–7F | Preset continuous PEDAL modulation source |
| SUS | Sustain | 00–01 | 01 holds released notes; 00 releases the pedal |
| AFT | Aftertouch | 00–7F | Pressure on every sounding voice in this track |
| NPR | Note pressure | 00–7F | Latest triggered note; last tone of a chord |
| LFR | LFO1 rate | 00–3F | Live native LFO1 speed |
| GLI | Glide time | 00–3F | Glide time used by subsequent note transitions |
| VEL | Velocity | 01–7F | Velocity of subsequent note triggers |
| EXT | External | 00–7F | Preset external-controller modulation source |

Controllers hold until changed or an explicit instrument selection resets them.
NPR resets for each new note: on a note row it affects that new note, and on a
blank-note row it affects the latest note. Earlier overlapping notes retain their
individual pressure, including repeated notes of the same pitch. AFT and NPR
combine using the higher value. A chord's last triggered tone is the NPR target;
independent arbitrary chord-tone addressing is not provided. KIL stops sustained
voices. Playback stop/reset clears performance state. SUS is separate from PED.
Wheel/Pedal/pressure/external commands are audible only where a preset routes
those sources. Existing pitch, volume and pan commands remain available.

Cutoff, resonance and LFO1 roll overrides affect held voices without retriggering.
Glide and velocity apply at note start. Ordinary patch edits apply to subsequent
notes; Wheel/Pedal defaults can affect current playback unless overridden by FX.
The saved patch is unchanged by all roll commands. No additional ADSR is added.

## Wave banks and editor

Place `hardware-80.hwb` and optionally `hardware-1.hwb` in
`instruments/USER/sq80esq`. SQ-80 is the calibrated default for either preset family.
The editor starts with Wheel, Pedal, Waves and Find wave banks, followed by all
three oscillators, four envelopes, three LFOs, filter, pan and mode controls.
Waves selects SQ-80 or ESQ-1 and shows ready/MISSING. Find wave banks loads missing
banks, including extracting recognized HWB files from a ZIP. Restart to replace
an already loaded bank. Missing banks produce silence. Stock wave IDs are
unchanged; SQ80 extensions remain 75–255.

Profile is hidden. New raw/SysEx imports use Calibrated; existing saved Hardware
profiles remain intact. The two decoder interpretations differ in LFO depth-source
bits and exponential velocity, so this is not a claim of hardware equivalence.
Native source numbers follow the modulation menu (15 is OFF). Coarse oscillator
pitch 36 is untransposed; fine uses 1/32-semitone steps.

The web app's **Import SQ80/ESQ** accepts HWB, program/bank SYX, 102-byte RAW,
CNI and ZIP files. Recognized HWB files in waveform ZIPs are extracted after
validation without overwriting existing files. **Import presets** also accepts
the whole `sq80esq`, `USER`, or `instruments` folder. Imports persist locally in
browser storage for this site; they are not uploaded. Keep originals for backup.

CNI and CCT store the complete 102-byte program, wave-bank choice, profile,
velocity, Wheel/Pedal defaults and browser source. They do not embed wave banks.
Rev1 files without the new optional performance record load with zero controllers.
Existing instrument and FX IDs remain unchanged; type 28 is reserved for SQ80/ESQ.
The importer handles complete and concatenated Ensoniq program/bank SysEx;
malformed data is rejected transactionally. Split/layer imports the base voice
with a warning; host split/layer and hardware sequencer behavior are not emulated.
Each tracker chord slot owns an independent engine context; global hardware
8-voice allocation across tracks is not emulated. Stereo remains intact through
tracker pan, inserts and sends.

USER preset browsing uses two panes: collections on the left and their presets
on the right. Left/Right switches panes, Up/Down moves within a pane, EDIT selects,
and EDIT+PLAY auditions. The collection path remains visible below both panes.
Folder and ZIP collections are grouped without extracting presets. FACTORY
browsing keeps its existing layout. Reopening USER recalls the selected sound.
