# Experimental SQ80/ESQ instrument

This personal experiment integrates the standalone Hybrid C++ engine. Enable with
`CHOOCHOO_EXPERIMENTAL_SQ80ESQ=1`; the personal PortMaster and web targets enable it.
Other native targets default off. Source integration is on `experimental/sq80esq`.
Engine source/version and development provenance are under
`chipnomad_lib/experimental/hybrid/`. No waveform samples or personal presets are
compiled in or placed in public packaging.

Choose Instrument → SYNTH → SQ80/ESQ. The standard instrument header retains name,
volume, pan, table and modulation. Bank: USER opens the existing folder/ZIP browser;
EDIT + Left/Right on Preset steps across compatible USER sounds. Hold PLAY in the
browser to audition; EDIT + PLAY auditions the selected instrument/editor sound.

Place `hardware-80.hwb` and optionally `hardware-1.hwb` in
`instruments/USER/sq80esq`. SQ-80 is the calibrated default for either preset family.
The Waves row selects SQ-80 or ESQ-1 and shows ready/MISSING. Load reloads missing
banks; restart the application to replace a bank already in use. Startup loads
available banks automatically. Missing banks produce silence, without substituting
unrelated waves. Stock wave IDs are unchanged; additional IDs remain 75–255.

The browser's **Import SQ80/ESQ** accepts HWB, program/bank SYX, 102-byte RAW,
CNI and ZIP files. A waveform ZIP's recognized HWB files are extracted after
validation, without overwriting existing files. **Import presets** also accepts
the whole `sq80esq`, `USER`, or `instruments` folder. Imports persist in browser
storage for this site; they are not uploaded. Keep originals for backup. Browser
storage can be cleared by the browser. Importing device files does not create a
synchronization service.

EDIT PRESET opens the scrollable native parameter editor: all three oscillators,
four envelopes, three LFOs, filter, pan and mode controls. Decimal values use the
hardware parameter ranges. Coarse oscillator pitch 36 is untransposed; fine is in
1/32 semitone steps. Source numbers match the hardware menu (15 means OFF).
Calibrated/Hardware import profiles select the standalone engine's two existing
SysEx interpretation modes. Patch edits apply to the next note; sounding notes
retain their trigger-time patch. An asterisk marks relevant track pitch/volume
FX or modulation, with a contextual explanation, without writing those playback
values into the preset.

CNI and CCT store the complete 102-byte program, bank choice, profile and velocity.
They do not embed the waveform bank. Existing instrument type IDs are unchanged;
type 28 is reserved for this experiment. Builds without it reject its instruments.
The importer accepts complete Ensoniq program/bank SysEx messages, including
concatenated banks; malformed input is rejected transactionally. Split/layer
programs import their base voice with an explicit warning. Hardware sequencer,
multitimbral routing and split/layer host behavior are outside this synth adapter.

The adapter retains stereo engine output through track pan, inserts and sends.
Each tracker chord slot owns an independent engine context; original global
hardware eight-voice allocation across multiple tracker tracks is not emulated.
Engine controls other than tracker volume and pitch have no new phrase-FX codes.
