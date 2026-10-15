# Validation and performance

Experimental branch, 2026-10-14. Architecture base: upstream/main `583702d`.
These results do not certify a release or replace interactive testing of the
combined daily build.

## SoundFont editor, Lane FX and browser follow-up (2026-10-14)

On `experimental/soundfont-edit`, the eight Lane FX are checked against actual
tracker playback: each produces the same audio as the corresponding saved
instrument control. Tests also cover command persistence, unchanged old IDs,
track isolation, Bank/default restoration, nested folders, ZIP directories,
raw-sample separation and cycling in both directions with wraparound.

The combined native suite ran **724 cases: 723 passed, one failed, two skipped**.
The sole failure was an old Monk test freezing the total FX count at 254. Its
check now protects the existing ID boundary while allowing appended commands;
the subsequent SoundFont/Monk run passed **14 cases / 1,032,439 assertions**.
The engine code did not change after the full-suite run. Stretch and Beads
lifetime checks also passed.

Production UI checks ran through SDL's dummy driver, with screenshots inspected:
scan status/Rescan, preset browser and cycling, failed-load preservation, latest
queued selection, all 11 Edit controls and all eight contextual Lane FX.
A local Emscripten build and headless Chrome 155 run passed USER ZIP import,
existing-file preservation, traversal rejection, persistence, preset browsing,
Edit changes, finite audible output and song save/reload, with no page errors.
Only synthetic public fixtures were used. These updates have not been deployed
or interactively checked on the handheld.

The earlier results below describe the original SoundBanks implementation.

## Automated checks

- Focused core tests: **16 cases / 127 assertions pass** with AddressSanitizer and
  UndefinedBehaviorSanitizer (`make -f Makefile.soundbanks soundbanks-test`).
- Full tracker suite: **665 cases, 664 pass, 1 fails**; **113,129,356 assertions,
  113,129,355 pass**. The failure is the M8 importer's sample-path case expectation
  at `tests/test_import_m8s.cpp:214` on the Mac's case-insensitive filesystem.
- Reproduction against an untouched archive of upstream `583702d`: the same M8
  case fails, with **7/8 M8 cases passing and 134/135 assertions passing**. This is
  a verified baseline failure, not a newly introduced SoundFont regression.
- Stretch lifetime regression: **32 create/reinitialize/destroy cycles**, zero
  retained allocation bytes.
- New integration checks cover project and instrument text round-trips, stable
  identities without runtime handles, queued preset lifetime protection, tracker
  preview rendering/stopping, and the compact five-row Instrument screen.
- Core fixtures cover SF2, genuine mono Vorbis SF3 (inclusive/exclusive endpoints),
  SFZ inheritance/includes/path resolution, legacy EXS, WAV/AIFF metadata, stored
  and deflated nested ZIP paths, traversal rejection, malformed/missing sources,
  bounded cache reuse, layer bounds, root/velocity selection, round robin, pan,
  sustain loops, note-off/release regions, omitted WAV alignment padding, Mac
  archive metadata, and duplicate EXS sample basenames with Windows/HFS path hints.
- Acquisition audit verifies complete original archives and per-member hashes.
  Original archive members have SHA-256 inventories; **84 preserved open-library source
  documentation files** match their recorded checksums. All **46 Gugak publisher
  manuals** also match their checksums and remain in the private original archives.
  Python acquisition/inventory/report tools compile successfully.

Full-suite logs are retained locally under `tracker/build/soundbanks/`, alongside
isolated baseline sources/build and sanitizer logs. The library probe evidence
is retained in the external data directory's `results/`. The checked-in
[compatibility JSON](compatibility.json) preserves per-preset results.

## Platform builds

| Target | Evidence | Remaining validation |
| --- | --- | --- |
| macOS desktop, x86_64 | Complete SDL desktop build succeeds with the existing local SDL dependency | Interactive browser/navigation/listening playtest |
| WebAssembly | `Makefile.web web-deploy` succeeds using the existing Emscripten SDK; checked-in `web/dist` regenerated | Browser import/persistence and long-preparation interaction runtime test |
| R36H / ArkOS aarch64 | Core probe and benchmark compile/run on actual R36H with GCC 9.2.1; 197/197 real SF3 presets prepare at 64 MiB | Complete application built natively on the actual R36H; interactive audio underruns and daily-build integration remain unmeasured |
| Windows / Android | Uses C++17 filesystem and existing platform storage helpers; no new platform-only runtime dependency | Toolchains not exercised in this session; no build-pass claim |

An attempted local browser smoke test could not run: the computer-use tool
reported no available browsers or apps (including no in-app browser). The
localhost-only test server was closed. No browser runtime pass is claimed.

The desktop command overrides the existing Makefile's output name to
`build/desktop/choochootracker` to match its chmod target. It uses the local SDL
headers/library already present in the personal build workspace. The normal
web command is documented in `docs/build-notes.md`; no cloud service is needed.

## Actual R36H measurements

Tests ran in `/roms/choochootracker-soundbanks-test-20261014`, with no installation
or launcher changes. Hardware reported aarch64 and approximately 897 MiB RAM.
The device clock was left unchanged; acquisition/test dates use the host clock.

FluidR3Mono's original SF3: **197/197 presets prepare and render** with a fresh
64 MiB PCM cache per preset. Largest observed prepared preset: **11,554,364 PCM
bytes**. Synthetic SF3 plus all five instruments inside the synthetic deflated
ZIP (SF2, SFZ, EXS, WAV, AIFF) also prepare and render on the device.

The following is an **offline throughput benchmark**, not live CPU utilization
or an audio-underrun test. Every voice has one looping synthetic SF2 zone and
renders 240,128 frames at 48 kHz (about five seconds). This excludes the tracker
UI, other engines, effects, disk activity and complex multilayer presets.

| Voices | R36H wall seconds | R36H CPU seconds | Process peak RSS bytes | Mac wall seconds |
| ---: | ---: | ---: | ---: | ---: |
| 1 | 0.070257 | 0.070135 | 1,380,352 | 0.00710358 |
| 8 | 0.560311 | 0.560217 | 1,380,352 | 0.0620475 |
| 16 | 1.12079 | 1.12057 | 1,380,352 | 0.119697 |
| 32 | 2.24697 | 2.24651 | 3,821,568 | 0.235716 |

The fixture used 512 PCM bytes. R36H metadata parse: **0.555042 ms**; preparation:
**22.2265 ms**. Mac parse: **0.103543 ms**; preparation: **3.70174 ms**. RSS is the
OS-reported `getrusage` process high-water mark for this tiny isolated benchmark,
not the full application or a real library's memory requirement.

There are eight tracker tracks, four chord slots per track and up to eight sample
layers per voice. The 32-voice test therefore covers the number of tracker/chord
slots with one layer each, not the much more expensive maximum layer count.
No performance claim is made for 256 simultaneous layers on handheld hardware.

Desktop indexing and real ZIP preset-preparation timings are in the
[compatibility report](compatibility.md). No live underrun counts have been
measured. Listening tests, repeated preset switches during playback, controller
navigation and browser recovery remain acceptance gates for the later daily
integration.

## Known incomplete components

- Broad EXS group/envelope fidelity remains incomplete. Gugak v1.3.1 validates
  41/41 presets after reading the publisher's Mac disk images; older ZIPs validate
  89/89. Original source directory suffixes now distinguish duplicate sample
  filenames for the two Ajaeng bow types, without modifying library content.
  Ten current presets report unsupported EXS group/synthesis parameters. Separate
  Logic effects and preset files are retained but not interpreted. DMG and nested
  ZIP containers are not runtime formats; original source files are copied out
  during the optional macOS acquisition workflow, without format conversion.
- Large-instrument disk streaming: not implemented. Salamander exceeds the
  maximum current PCM budget; other large pianos only load with desktop budgets.
- Full SFZ modulation/articulation/CC/pedal semantics and SoundFont custom
  modulators/LFOs: warnings identify unsupported parameters; successful loading
  does not imply identical playback.
- A dedicated editable filter/envelope MOD interface is not added. Existing
  volume/pitch MOD and pan routes work; source envelope/filter values are imported.
- Browser filesystem memory is not native disk memory. Large imported ZIPs and
  temporary member files can consume browser RAM beyond the PCM cache budget.
- Factory selection, packaging and downloads are deliberately not implemented.

## Accepted upstream contribution record

# SoundFont contribution validation

Prepared against upstream `583702d68ee6badf7ad37f7264a1ba9f2b87ab46` on October 14, 2026.
This branch contains the SoundFont feature only; personal ESQ, Beads, MonkSynth,
launcher artwork and deployment configuration are excluded. SoundFont takes the
next upstream instrument ID, 28. Existing upstream instrument/FX IDs are unchanged.
Personal-fork song numbering is different and is not an upstream interchange format.

## Standalone contribution

- Native suite on macOS: 686 cases, 685 passed, one pre-existing M8 import path-case
  assertion failed (113,826,438 assertions, one failure). The same failure was
  reproduced previously against untouched upstream `583702d`: the test expects
  lowercase `samples`, while a case-insensitive filesystem can resolve `Samples`.
  The M8 importer and its test are unchanged here. This is not a clean full-suite pass.
- Focused SoundFont suite: 40 cases and 707,081 assertions passed.
- Stretch allocation lifetime check: 32 cycles, zero retained allocation bytes.
- Parser/voice address and undefined-behavior sanitizer suite: 23 cases and
  3,773 assertions passed without sanitizer findings.
- Fresh Emscripten WebAssembly build passed using the stock Web Makefile.
- Headless Chrome 155: USER ZIP import, keeping existing files, traversal rejection,
  persistence after reload, nonzero finite audio, collection browser, direct editor
  and instrument/song roundtrip passed with no JavaScript runtime errors.
- Browser storage queue tests: four passed. Source and checked-in wrapper HTML match.

The browser test uses generated fixtures and an isolated profile, not the user's
live browser storage. Automated audio sampling is not a speaker listening test.

## Integration evidence from the personal daily

The same dedicated SoundFont parser, source, voice, browser and library code is
used by personal daily `7720a36`; shared tracker integration is adapted here to
stock upstream types and layout. The personal build contains other experiments,
so its results are supporting evidence rather than tests of this exact branch.

- R36H: startup and SoundFont UI/load/failure/editor/save checks passed.
- 54 note-transition/KILL playback cases passed across nine installed samples.
- All 411 installed loose WAV/AIFF files loaded; seven compatibility warnings,
  zero failures. The larger local reference scan loaded 1,670 files with zero
  failures. These counts do not establish every preset's sonic fidelity.
- Owner reported an hour of R36H playtesting before requesting review.
- Personal web `7720a36`: production build and the same browser regression passed;
  all 11 published runtime file hashes matched its build manifest.

## Remaining limits

This is a supported subset of SF2/SF3, SFZ and legacy EXS24, not a complete
replacement for their original players. See [format coverage](README.md#format-coverage)
and the user manual. Large instruments can exceed the 64 MiB decoded PCM budget;
there is no streaming. Custom modulation and some articulation/envelope/routing
features remain unsupported. Physical Safari/iOS, Windows and Android behavior
have not been established by this review. Private libraries are not included.
