# Native sound banks: architecture and validation plan

Branch: experimental/native-soundbanks, based on upstream/main 583702d.

## Repository findings

InstrumentType and the metadata catalogue in chipnomad_lib/project_instruments.*
select the existing chip, FM, Mutable, sample and MIDI engines. Instrument
screens use ScreenData and selectionPopup; the new screens remain children of
Instrument. SampleVoice already supplies stereo interpolation, normalized sample
regions, looping and stretch; its single-region/normalized-loop interface cannot
represent layered key/velocity zones and independent sustain loops precisely.
Use a small fixed-layer voice beside SampleVoice, reusing Envelope and
MultimodeFilter, and feed the existing tracker/chord mixer and event routing.
No change to SampleVoice's existing behavior is planned.

chipnomad_lib.cpp owns fixed track/chord voices, audio snapshots and note events.
Volume/pitch MODs and track pan/effect sends are shared. Project instrument IO is
text based; append an instrument type and explicit stable bank/preset references.
Project snapshots shallow-copy sample pointers, with lifetime reconciliation;
prepared soundbank storage needs equivalent ownership outside the callback.

PresetZip uses vendored MIT miniz but reads whole archives (32 MiB maximum).
Sound banks need a separate file-backed central-directory reader using the same
inflater, bounds checks and CRC. Do not change existing preset archives.
Existing WAV loading is PCM-focused. WAV smpl and AIFF INST/MARK metadata need
explicit parsing. Desktop/ArkOS use ordinary files; browser uses Emscripten VFS
and persistent imported files. Paths must follow existing default-data-directory
conventions, with a configurable SoundBanks root. No runtime downloads.

## Proposed files and stages

* chipnomad_lib/soundbank.*: normalized bank/preset/zone/assets and preparation.
* chipnomad_lib/soundbank_source.*: file-backed ZIP and bounded asset readers.
* chipnomad_lib/soundbank_sf.cpp, soundbank_sfz.cpp, soundbank_exs.cpp and
  soundbank_audio.cpp: format import and audio metadata.
* chipnomad_lib/synth/soundbank_voice.*: fixed layers, existing envelope/filter.
* tracker/src/soundbank_library.* and screens/instrument_soundfont.cpp: discovery,
  index, async preparation and the two Instrument rows/pages.
* Extend instrument catalogue, serialization, snapshot lifetimes and mixer.
* tools/soundbanks: reproducible complete-library acquisition and inventory;
  bulk data outside Git. docs/soundbanks: attribution, evidence and limitations.

TinySoundFont (MIT) is a useful SF2 reference, but its normal loader allocates
whole-bank sample storage. A bounded RIFF metadata importer avoids this and a
second renderer. Investigate stb_vorbis (MIT/public domain) for per-sample SF3.
Investigate renoise-exs24's actual license and offsets before implementing EXS.
Reuse vendored miniz (MIT). Sample licenses remain separate from code licenses.

## Risks and acceptance gates

SF3 sample offsets and compressed variants need explicit validation, not an SF2
extension rename. EXS is undocumented and unsupported parameters must be reported.
SFZ includes/macros, inheritance, spaces and Windows paths require real libraries.
Malformed input and cache limits must be tested before enabling UI loading.
No allocation, I/O or decompression in note-on/render; bounded region/voice counts.
Use stable relative source/member plus preset program identity, not list indices.
Publish prepared presets at a safe existing snapshot boundary; never free samples
while an audio voice references them. Browser builds need a cooperative preparation
path when pthreads are unavailable. Preserve existing controller navigation.

Run parser/playback tests after each component, then the existing Makefile.test
suite, desktop and web builds. R36H measurements require actual hardware and
must be labeled separately. Real-library reports distinguish indexed, prepared,
rendered and untested presets. No claim of complete SFZ/EXS support is intended.
