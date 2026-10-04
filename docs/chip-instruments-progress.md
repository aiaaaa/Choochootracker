# Native chip instruments progress

Baseline/rollback: c0a8c7e9ee7e177e94294ede7e6cad09c59f06c8, clean personal/r36h.
Task worktree: ../native-chip-instruments, feature/native-chip-instruments.
Latest user authorization (October 4): publish a feature branch on the fork; build, validate and install on the handheld preserving the device personal build. No PR. SSH 192.168.36.2 currently times out; no device changes yet.
Baseline: make -C tracker -f Makefile.test -j4: 328 tests / 8,101,169 assertions pass.

Extension map (MME followed end to end):
- project_instruments.h/.cpp: value union, metadata table, init/free; add IDs after 16, preserve retired 14/15.
- project_instruments_io.cpp: instrumentLoadData/instrumentSaveData dispatch; project_io.cpp CNI/CCT headers + table handling. CNI currently destructive: new factory loads must use owned temporary state.
- chipnomad_lib.h/.cpp: per-track/chord allocated voices, updateMMEVoices before applyVoiceEvents; renderMonoVoiceTracks feeds track inserts, sends, meters.
- playback.cpp supplies chordPitchFinal, fineOffset, note events and generic modulation. No new FX IDs needed for first slice.
- screen_instrument.cpp: grouped menu, quick cycle, screen-kind array; instrument_mme.cpp demonstrates preset popup and page delegation.
- copy_paste.cpp cloneInstrument is value assignment; fixed payload avoids new heap ownership.
- Makefile.common and Makefile.test own source discovery. Test suite excludes project_io and instrument UI: add executed dedicated coverage.

Plan: Stage B OPLL + VRC7 fully first, then OPL banks/browser, PSG/GB, OPN/OPM, packaging/performance.
Audits: ymfm 81aec25 BSD-3 with OPL translation-unit closure (dormant PCM/ADPCM required to link, never exposed); emu76489 c0fa097 MIT, Sega 16-bit bit0/3 taps/reset 0x8000, noise divider still under audit; gb_apu 3d73d0d MIT including upstream MIT blip relicensing statement; selected WOPLX sources 7ae1469 each contains explicit MIT.
The dated stage notes below are historical. Current status is the final checkpoint at the end.

Stage B checkpoint: OPLL/VRC7 metadata/UI, 15+15 pinned melodic programs,
per-track/chord native voice, 24-tap/64-phase stateful resampling, pitch/volume
modulation, complete tone persistence, transactional v6 CNI loading and
non-destructive queued preset audition implemented. 335 tests / 12,335,816
assertions pass, including all programs at 44.1/48/96 kHz, block independence,
pitch at 22.05/32/44.1/48/96 kHz, CNI/CCT reload and owned preview.
Desktop production build with reused SDL2.framework passes. No GUI launched.
Instrument=680, Project=313256, union=624 (OPLL payload fits existing union),
OPLLVoice=11152 bytes; FX count stays178, max chord slots4.
Remaining Stage B validation: actual GUI capture/input, native release/reference
measurements, factory CNI/demo artifacts, optimized/ARM benchmarks. Stage C not
started. No stage marked finished merely because it compiles.

Stage C checkpoint in progress: OPL2/OPL3 typed patches, two/four/dual topology,
per-track/chord contexts, stereo register routing, bank/category preset UI,
owned queued preview, canonical C++ IR->CNI writer and WOPLX allowlist conversion.
697 CNI records generated/reloaded; 589 normalized unique IDs; 185 four-op,
252 dual-voice; 289 percussion source entries. Native levels/software gain;
MIDI player volume models/velocity offsets retained but deliberately not applied.
339 tests /30,596,794 assertions pass, including every generated CNI render.
Seashore's native AT=1 initially silent in 250ms test; held test confirms output.
Python parser tests 4 pass. UI syntax checked; production GUI capture remains.
C audit/assets/pipeline: tools/chip_banks and packaging/common/instruments/chips.
D dependencies fetched/audited but not integrated: C++ gb_apu requires explicit
calloc cast under project's compile-C-as-C++ build; emu76489 tone-derived noise
clocks independently at double desired shift rate, needs focused edge patch.

Scope extension (user 2026-10-03): DX7 FM now explicitly included; original
No DX7 exclusion superseded. Audit and vendor Apache-2.0 MSFA core only;
Dexed application and JUCE excluded. Preserve this worktree and all chip work.
Prove native DX7 isolated voices, audition, playback and full save/reopen first;
then acquire/review/convert/deduplicate/index/categorize/package preset banks
and implement validated local DX7 SysEx import. Benchmark before handheld
voice-limit choice. No push/PR/handheld install. Full named addendum not yet
visible in Downloads or conversation attachment; requested its local path.
Stage D now implemented (not merely dependency audit): native Sega PSG and
DMG Pulse/Noise, authored 14/10/10 presets, post ADSR, queued audition, UI and
v6 persistence. Four focused tests passed /19,230,771 assertions (all presets
44.1/48/96 kHz, block independence, pulse duties/noise widths, exact Sega
fixed/derived noise clocks). Full suite/production rebuild still to run.

Full DX7 addendum now read/copied to dx7-addendum.md. Acceptance additions:
- six-op full VCED semantics, one maintained Apache MSFA scalar core, per-file
  provenance/modification notices, no Dexed app/JUCE/MTS/new GPL dependency;
- per-part LFO shared appropriately across chord notes, isolated tracks;
- 64-sample buffering retains leftovers; awkward event timing tested/documented;
- owned native patch + wrapper, transactional shared FM browser and local .syx;
- strict 155/4096 payload formats, bounded multi-message/checksum/range validation;
- >=64 curated starter target; investigate >=1000 cleared unique parameters,
  source-by-source audit; don't substitute counts or call fallback huge library;
- shared index >=10000 metadata entries; no synth allocation per catalogue row;
- optimized 1/4/8/16 note and representative-song benchmarks, reference harness,
  release-aware deterministic limits only from actual headroom. No device run,
  install, push or PR automatically; unavailable hardware measurement pending.
DX7 in-progress: pinned maintained Source/msfa at 2e182b3db85c09083ab13c8b9b00565ce7d9ff85,
selected scalar DSP modules, namespace isolation, removed unused app includes;
original note adapter under development. Native ID24 appended, owned VCED155,
strict parser/serialization files started. Not yet integrated/tested/selectable.
MSFA globals fixed at44100 for now to avoid cross-renderer rate races; review
per-instance native host-rate alternative before accepting this policy.
Stage D full regression:343 tests,49,827,589 assertions all pass.

DX7 functional checkpoint: ID24 native DX7 FM integrated in existing hierarchy,
owned VCED155 + fine/velocity/source/name payload (Instrument remains680bytes),
per-track DX7Part contains4independent MSFA notes + one shared LFO/quantum clock.
App has one dedicated preview part; queued patch copy never mutates Project.
Events deferred next64native frame boundary, no leftover discard; fixed44100
immutable global lookup tables -> existing FIR. Same shared OPL/FM browser now
handles DX7 plus validated .syx local imports transactionally. Bank/preset/fine;
no new operator editor. EDIT selection deferred to release in audition popups
so EDIT-first thenPLAY cannot accidentally commit. Version6CNI/CCT completepatch.
350 tests/61,684,307 assertions passed before latest index/pitch/UI-gesture tests;
production desktop build passed. Selected MSFA scalar modules ASan/UBSan all32
algorithms/extreme parameters passed. Additional sharedcatalog10k test, sine/fixed
frequency/velocity/reference tests currently building/running.
DX7 sources: OpenDX7 pinned26c9b3 has31musical+INIT(excluded); YSE pinned911ea2d
CC0 bank32slots butonly4unique; authored32ChooChoo CC0recipes. Converter nowshared
pipeline emits764totalCNI (697OPL+67DX7),656parameterIDs (589OPL+67DX7). All67DX7
reload/render audible finite testpassed. 95parsedDX7entries ->67parameters; source
aliases/fullhashes in dx7-manifest.json. Large1000goal unmet; broader review ongoing.
Pafreak directsubmission archive fetched/reviewed, contains DX7II additional
messages(5239bytebanks,730bytesingles); excluded rather than silentlydrop extra
parameters. Benson228SYXarchive no included docs; mixedfactory/unknown collection
not yetapproved. BlackWinny/mirror blanketCC0 notaccepted as authorclearance.
Pending: UI screenshots+gesture tests, optimizedbenchmarks, audit patchmanifest
refresh/notices/docs, canonicalcommit. Otheroriginalscope Genesis/Arcade still
pending, asareotherfactory/demo/packaging/web/handheld validation items.

DX7 validation update:353executabletests nowexist (343previous+8DX7+2catalog).
Most recent focused9tests passed11,865,482assertions; actual764entrycatalog+10k
synthetic index tests pass. Python10contenttests pass (allsourcehashes,31+4+32
uniqueness, originalrecipe determinism, code-rejectingliteralparser, malformed
SysEx and boundednonextractingZIP reader). SelectedMSFA sanitizerharness passes.
OptimizedMacx86_64/O3 offline benchmark30saudio/configuration, rates44.1/48/96K,
blocks128/512,notes1/4/8/16, no measured deadline misses. 48K512/16notes mean132.300us,
p95168.574us,p99265.618us,worst381.907us; 10.667ms deadline. DX7Part13408bytes.
Not paced realtime; CPUmodel/governor/thermals/hardwareunderruns unavailable.
CSV/JSON .tmp/chip-audit/dx7-benchmark.*. No handheld limit finalized.
ProductionUIharness first run reachedpages; caughtmissingproject_utils type-name
mapping (fixed all8newtypes). Browser failure was harness screenSetup deferred
untilappDraw (fixedharness). Rebuild inexecsession20714; rerun dummySDL next.
MSFA filemanifest refreshed, ApacheNOTICE/patchlog/prominentfilemodification
notices complete; nativefactorysources runtimeMIT/CC0notices added. Manual,
buildnotes andpersonalfeaturemanifest updated actualstatus; no in-apphelp changed.

Stage E functional checkpoint: added GenesisFM25 and ArcadeFM26 (DX7 remains24),
FourOp patch/voice adapter reuses pinned ymfm OPN/OPM/SSG (unchanged files added
and hashes recorded), shared FM browser/queue/native6 persistence/chord lifecycle.
24 individually authored MIT recipes per family, no ROM or uncertain downloads.
TFI42byte and VOPMtext offline converters added; bounded user-import CLI also
handles WOPLX and DX7. VOPM PAN0/64/127 mapped nativeL/both/R; partial pan/noise
explicitly rejected. Format inspected original VOPM Save/Load/SendPan in ignored
vopm-OPMdrv.cpp; no code copied. Genesis DAC idle504 accounted and20Hz DC blocker.
Native frequency tests verified44x pitch rates22.05/44.1/48/96K;4op release/kill,
independent state, unevenbuffers, pan, nativefiles, transaction/preview pass.
Full357 tests72,489,645 assertions pass; Python13tests pass.
FMcatalog812 (697OPL+67DX7+48four-op),704parameter IDs. Builtins64CNI generation
nowadded (30OPLL/VRC7+34simple), not inserted in sharedFMcatalog (own UI browsers).
DX7 UI dummy harness passed including actual sequencer/UI; popupoverlaysfixed,
visuallychecked .tmp/chip-audit/ui-captures/dx7-presets.png (clear names/footer).
Latest UI timing48K1024/Osoffscreen p951400.175us,p991655.166,worst1833.765;
not paced/hardware. Full desktop/UI/web rebuilds and audition reports pending.
No handheld deployment/installation/run or newtoolchain performed.
Next: audit content regeneration, implement auditiontool+smallproject/WAVreport,
optimized all-family/mixed benchmarks, personalflag productionbuild+package,
webdist regeneration with existing /private/tmp/choochoo-pr-emsdk, scopedcommit.


## Final host checkpoint — October 4

All ten native types implemented; DX7 end-to-end proof and content work complete.
876 CNI files: 812 shared FM entries and 64 simple/ROM-tone entries. DX7 has
67 distinct cleared presets; 1,000 goal unmet. Native generation reproduced
byte-identically. 13 bank WAVs / 52 machine auditions and portable demo created.
359 standard tests / 72,681,719 assertions pass; personal suite 371 pass,
2 explicit opt-in skips / 72,691,756 assertions. Python content tests 13 pass.
Desktop personal build and web deploy succeed. Final SDL dummy UI smoke passes.
Yamaha key-off/on retrigger now clocks the off state before asserting on;
direct register/reference tests pass. Host tool header dependencies repaired.

Final 62-case host benchmark records three misses only at 32 OPL2 voices.
DX7 48k/512/32 mean292.164µs, p99550.174µs, worst689.200µs, zero misses.
Paced ten-minute FM-heavy song/effects plus 9,566 scans of synthetic10k catalog:
p952903.271µs, p992938.673µs, worst4409.223µs, zero render misses; peakRSS112623616.
All measured rows are tracked in docs/chip-{all-chip,soak}-benchmark.csv.
Handheld voice limit remains provisional pending device measurements.

Latest user authorizes installation and a fork branch, no PR. Local and remote
personal/r36h both remain c0a8c7e, the ancestor of this feature worktree.
SSH at saved192.168.36.2 timed out both sandboxed and unrestricted. Asked user
whether USB SSH address changed. Existing on-device GCC9/header/dependency build
procedure is in ../mod-lucky-validation/stage_device.py; do not reinstall SDKs.
Next: verify desktop package, commit/publish feature branch; once SSH restored,
inspect installed source/assets/processes and existing ARM toolchain, build and
benchmark in an isolated /roms task directory. Merge tested feature into
personal/r36h, preserve every user asset/settings/autosave with verified rollback,
then install one regular launcher and validate. Do not open a PR.
Human listening, ARM64 build/runtime/thermal soak and target voice limit remain
unresolved. Full details and provenance exclusions: chip-instruments-report.md.


SSH restored after user retry. Verified paired machine ID and existing GCC9,
ARM libxmp-lite, curl headers and base/MIDI header caches. Isolated build staged
at /roms/choochootracker-native-chips-20261004; installed app still unchanged.
Runner builds personal PortMaster package, enabled tests, production UI smoke,
all-family benchmark and ten-minute catalog/playback soak. Local orchestrator:
.tmp/chip-audit/stage_device.py; record device-build.json. First GCC build exposed
five sparse C99 tables unsupported in C++ mode. Expanded exactly to positional
entries with zero fill; equivalence verified for all 260 entries, provenance
patched hash updated. Build resumed. UI capture directory created explicitly.
Device clock is November2025 (hostOctober2026); do not adjust unrelated clock.
Use tar -m and device-generated mtimes. Governor interactive, build temp56–80°C.
Desktop package first attempt caught notice filename LICENCE.txt (not LICENSE);
packager corrected to require that plus Blip_Buffer.txt. Archive not published.

Desktop ZIP completed: 876 CNI, required notices and CRCs pass. SHA256 bf642716879d7e18a8a14de7b94566562fe448ebf9ac6484ca5e8e6c1888bfac. Final web WASM compiles with Node; host standard suite still359pass after GCC table fix. Added developer-only native_chip_audio.cpp for 70-second real SDL/ALSA callback validation with portable demo; not part of production executable.


Publication/device checkpoint: f768258 committed and pushed to
origin/feature/native-chip-instruments. No PR. personal/r36h remains c0a8c7e until
combined device validation completes. ARM64 PortMaster personal executable and
ZIP built successfully; ldd resolves all libraries. Full enabled test suite is
compiling on the device, then runner proceeds into UI and benchmarks/600s soak.
Task scripts prepared (not yet executed): .tmp/chip-audit/hardware_validation.py
(70s real ALSA demo plus isolated production startup waveformOFF/ON; releases only
idle ES audio and restores supervisor), install_device.py prepare/install (copies
all installed assets, validates full snapshot/rollback, preserves regularlauncher).
Run hardware_validation only once build.exit=0. Upload it and run background,
collect hardware-audio.exit/log before installation. Device source hash comparison
matches production code; differences are docs, newly added validation driver/
Makefile, vendor PATCHES note and desktop packager. Stage record hashes predate
those updates: refresh inventory and source-commit.txt before final install.
Local docs/build-notes.md and personal-features.json have uncommitted follow-up
instructions/channel naming/included_commit; finish them with actual device data.


ARM suite now PASSED: 371 tests, 72,691,756 assertions, two expected opt-in skips.
Production ARM binary SHA256 ddd1f640c632c93d3c50095eeb2f096cf47c087c3b36727543ae11fa8a1d9976.
Runner target typo `chip-benchmark` corrected to `benchmark-native-chips` in
local stage script; remote continue-validation.sh resumes after passing suite.
Old exit2 moved to build-target-typo.exit. Current build.exit belongs to ongoing
UI +62case benchmark+600s soak. Telemetry resumed in append mode. No app installed.


Device measurements require final tuning before install. DX7 16 notes at48k512:
mean2061.297us, p952174.958us,p992190.708us,worst4134.083us,zero misses.
32-note stress has misses at44.1k512 and96k128. Implemented shared16activeDX7budget
in existing owned slots, releases/quiet held notes before fresh attacks;
ties preserve roots across tracks before chord extensions. No other instrument
polyphony changed; preview already disabled during playback. New functional
stealing tests pass in full host suite360tests/72,681,766assertions.
Baseline ARM -Os8OPL3four/dual exceeds512deadline (~10.9/11.4ms). Applying focused
-O3 only to new native chip vendor cores and adapters (no fast-math); all other
code keeps prior flags. New Makefile.native-chip-flags included byPortmaster and
tests; test CLI NATIVE_CHIP_OPT_FLAGS=-O3 matches production profile.
Benchmark --bounded retains all30DX7rawloadcases plus1/8otherfamilies andactual
mixed/FMheavy songs (52cases), omittingoptional32-voice non-DX7overloadstress.
Need upload/rebuild/retest and measure final policy/profile before installation.
UI ARM successful with SDL_VIDEODRIVER=offscreen SDL_AUDIODRIVER=alsa; bothdummy
drivers absent. UI+render48k1024 p9528289.041,p9928406.292,worst28590.042us;
separate from audio callbacktiming. Device DX7 browser screenshot visuallyclear.


Final tuning now staged and rebuilding via remote validate-final.sh, logs
production-build-final.log then tests.log, benchmark-build-final.log,
ui-build-final.log, ui-smoke.log, device-benchmark.csv, device-soak.csv.
Previous Os sweep intentionally stopped during optional32voice overload stage;
kept as baseline-os-device-benchmark.csv (partial). Old tests/UI logs also kept.
Final production/codeprofile verifies -Os followedby -O3 only on selectednative
sources. New test checks actual sequencer8four-noteDX7chords remains4logicalnotes
pertrack but16active andunchangedproject. Host full361tests72,681,789assertionsPASS.
Desktoppersonal andweb rebuilt afterbudgetpolicy PASS. Latest hostartifacts
containlimit; prior desktopZIP stillprelimit and MUST regenerate beforehandoff.
Device final benchmark now53cases:30DX7rawpoints,20otherfamilies1/8,3songs incl
FM-heavy chord song+FX (11notes max). Soak useslatter+10kmetadata,600seconds.
The final device tests should be373passing (prior371+2); confirmactualresult.
Current sourcechangesuncommitted: policy,tests,focusedflags,benchmark,docs,web.
Afterfinalchecks commit+pushfeature,fast-forwardcleanpersonal/r36h andpublish;
then sourcehashsync,hardwareaudio70s+startup,verifyrollback/install. NoPR.


cd25ce3 policy/optimization checkpoint pushed to feature branch. Final ARM
production profile built successfully; now compiling full373-case enabledsuite.
Telemetry sampler had stopped during baseline transition; restarted append at
monotonic4585, before final benchmark/soak. No systemclock/governorchanges.
Added previewfooter "STOP SONG TO HEAR  EDIT SELECT  OPT EXIT" when published
PlaybackStatus.isPlaying; usesexistingthread-safeUIstatus API, no directaudio
state read. ExtendedUIharness checks previewcannotstartduringplayback/captures
fm-presets-during-playback. These twofiles uploaded andUNCOMMITTEDlocally.
Production was repackaged onceforfooter, then snapshot-readfix landed: FINAL
PortMaster-deploy must runafterUItestrebuildto relinklatestselection_popup object
beforehardwareaudio/install. Hostdesktop/web/UI must rebuildlatestfooter too.


October 4 reconnection checkpoint: handheld reboot interrupted final test build.
Resumed validate-final.sh from existing objects; no installed app changes.
Final ARM64 suite PASSED: 373 tests, 72,691,826 assertions, two opt-in skips.
Final host suite361/72,681,789 PASS; desktop/web rebuild PASS; WASM compile PASS.
Host UI including playback-preview guard exits0, p951455.944us,p992040.833us,
worst2667.054us; screenshot stop-song footer visually verified.
Device clock moved backward at reboot, so existing future file mtimes trigger
make clock-skew warnings. Force the final production link after validation;
verify source hashes before install. Do not change device clock.
Remote runner now building developer UI/audio, then53-case benchmark and600ssoak.
Telemetry restarted after reboot (monotonic values reset; segment accordingly).


Performance checkpoint: final53-case native-only-O3 sweep completed. Individual
chip/DX7 cases pass deadlines, but actual songs miss all512-frame deadlines:
mixed mean16085.941us; FM-heavy17027.497us; chord17268.775us (deadline10666.667).
Did NOT start the misleading ten-minute overloaded soak or install. Paused then
terminated only task-owned validate-final.sh after its benchmark finished;
baseline-native-only-o3-benchmark.csv preserves all53rows. Running isolated
diagnostic.sh compares3-second songs with/without inserts and O3 on four
existing audio units: chipnomad_lib.cpp, insert_fx.cpp, synth/master_effects.cpp,
audio_monitor.cpp. Alternateobjects/binaries leave production objects untouched.
Localhelper .tmp/chip-audit/profile_device.py; remote diagnostic-*.csv/log/exit.
Need resolve measured overload, update targeted flags only if supported by
comparison, rerun affected tests and full benchmark/soak before installation.


Diagnostic results confirm existing insert-load limit: baseline3-second mixed
with16inserts16.44ms vs7.58ms without; FM-heavy-chord17.62ms vs8.77ms.
Isolated O3 onfourexistingaudiofiles reduced all16inserts chord to15.53ms,
stilloverdeadline. No diagnostic object has replaced a production object.
Benchmark now distinguishes4representativeinserts from --all-inserts16stress,
plus --songs-only. Existing50enginepoints remain valid (productionunchanged);
new3songpoints plusold3stress will form56-rowdeviceCSV. resume_performance.py
awaiteddiagnostics, rebuiltbenchmarkonly, nowrunssongs/600ssoak.
Sourcefuturemtimes afterreboot caused unnecessary recompilation. Normalized
2730isolatedsourcefilemtimes todevice-nowminus1day afterhashaudit; contents
unchanged. DoNOT change systemclock. Forcedfinalproductionlink stillrequired.


User clarified modest track/insert tradeoffs are acceptable and proposed8tracks:
4WAV +2simplechips +2FM, sprinkle effects. Explained2insertSLOTS PER TRACK,
16availabletotal, CPUbudgetseparate; do not reduce availabletracks/slots.
Eighttrack2insert30sruns have14/36/92deadline misses (mixed/FM/chord), so
do not labelthoseglitch-free. Baselines4insert and16insert kept, nofx3sdiagnostic
passes. Four-fileO3diagnosticdidnotfix16slotload; productionprofileunchanged.
Benchmark nowoffers --four-tracks, --sample-mix (fourloopedexistingWAVs, SegaPSG,
GBPulse, DX7,OPL3; chordvariant11notes), --no-inserts/--four-inserts/--all-inserts.
Runtimeapplicationcodeunchanged. Finalrunner finish_performance.py buildshelper,
measures4tracks4inserts anduser8trackmix4inserts, then600suser-mix/chord/4FXsoak.
64rowaggregatepreserves50enginecases+9eighttrackstress/loadrows+3fourtrackrows
+2balancedrows. validation-stage.json tracksphase. Native WAV helpermissingheader
fixed; hostbenchmarkbuildPASS; device resumed afterisolatedhelperfailure.
soak-memory.py samplesactualsoakVmRSS/VmHWM/threads10s; telemetryresumed.
Local .tmp/chip-audit/finish_performance.py stagesremote version; hardware script
nowrequires70sbankdemo AND70sbalanced-benchmark.cct (generated bybenchmark),
thenpreservedproductionstartupOFF/ON. Do not runinstallerbeforeallchecks.


Final balanced run underway: validation-stage.json recordsstartmonotonic2082.385.
64-caseCSV now tracked docs/chip-r36h-benchmark.csv (50enginecases0miss;
allstress/loadrows13009missinclknownoverloads). User8track4WAV+2chip+2FM+4FX
30s mean7445.996us,p957988.167,p998059.917,worst8646.750,0miss; DX7chordvariant
mean7689.152,p958253.583,p998332.625,worst12142.667,1miss.
FourtrackAY/Plaits/FM+4FX alsohadbursts; trackcountaloneisnotCPUbudget.
Ten-minute balancedchord4FXsoak now~8minutes, RSS50444..50708KiB observed;
final stats pending. Remote balanced-benchmark.cct exportsactualsamplemix for
second70srealALSAcheck; runaudiohelperwithcwdtrackerforrelativeWAVpaths.
No4-filediagnosticO3changeadopted. Productiontestresults remain373pass; source
changeaftertestsonlyUIfooter(alreadyUIchecked), benchmarkhelper/docs/webfooter.
Next: finishsoak; finalize_device_build.py forcesproductionlink; hardware_validation
(two70saudiochecks +6sOFF/ONstartup, exactidleESrelease/restore); collectmetrics,
updatefinaldocs/manual/features; commit/pushfeature, ffpersonal/r36h andpush.
Then sync_device.py syncsourcehashes+commit; refresh_device_package.py rebuildZIP
withfinaldocs/notices/inventory; install_device.py prepare/installverifiedrollback.
RegeneratedesktopZIP(usingnewstage) andretrieveARMZIP. NoPR.


FINAL VALIDATION COMPLETE October4: productionARMbinarySHA256
1966139c400e5fc25ca93107b5d442816f5d60c84d026814bef750a6945bc247.
ARM373tests/72,691,826assertionspass2optinskips; host361/72,681,789pass.
64measuredcases tracked chip-r36h-benchmark.csv; overloads preserved.
600sbalancedchord4FXsoak: p958647.042us,p999041.375,worst20440.875,28misses;
3948synthetic10kindexscans. WarmRSS50444..50708KiB, HWM52508KiB,62.083..74.583C.
InitialdefaultPCMhardwareprobesunderran: actualregularlauncher exports
AUDIODEV=plughw:0,0, whiledefaultPCMwas44100Hzdmix4096buffer. CorrectedTEST
environmentonlytomatchlauncher, noapp/settings/systemaudiochanges.
FinalS16/48000/4906 direct-card70sbank AND70sbalanced4WAV+2chip+2FMchord4FX
PASS:685callbackseach,0deadline misses,0loggedALSAunderruns,0nonfinite.
Bankp9522131.959us,p9924590.416,worst27391.875; balancedp9580990.292us,
p9981576.542,worst81818.625 (deadline102208.333us). Test-onlymastergain0.4
leavesheadroom; no patch/usergainchanges. Direct-cardproductionstartup
waveformOFF/ONpass; installedsettings/autosavehashespreserved; ESrestored.
NewdeveloperfixtureS16optionalbufferandMakefileindependentaudiotargetvalidated
onARM; neitherisproductionbinary. Reports/manual/featuresupdated.
Next remainingdelivery: commit/publishfeature+ffpersonal; sourcehashsync; refresh
ARMZIP; install_device.pyprepare/install(requireconfigured-audio.exit0 too);
verifyrollback/settings/assets; desktoppackage+ARMdownload; commitinstalled
checkpointdocs. Humanlistening/interactiveplaytest and1000DX7goalremainopen.


DELIVERY COMPLETE — October 4, 2026 (host date).
Installed source/application commit: 4b7bad7e2a3ed04e62cd22173b97774874c9435d.
Both origin/feature/native-chip-instruments and origin/personal/r36h published;
personal worktree fast-forwarded, all existing personal features retained. No PR.
Regular install /roms/ports/choochootracker replaced only after staged full-file
verification. Full745-file prior installation verified in rollback directory:
/roms/choochootracker-backups/pre-native-chips-4b7bad7e2a3e/previous-install
Original executable SHA256:
dcbef85cccf7b30d102973fa94111ef2cfd3b969209a60f3219c022e644c1f55
Installed executable readback SHA256:
1966139c400e5fc25ca93107b5d442816f5d60c84d026814bef750a6945bc247
Installed1656files;876nativeCNI verified. Kept all user assets/settings/autosave
and regularlauncher; preserved22existingpackageconflicts includingcustomgptk,
fonts, drum-instrumentfiles andgameinfo. audio48000/4906, persistentWaveform1,
DepartureMono02 remainexactlyasbefore. Installed personal-build.json records
channeldevice-personal-build, sourcehash, rollback, testreport andhumanplaytestpending.
No automatic interactive launch; idlefrontendrestored; no testappsleft running.

Release archives generated from application source4b7bad7, eachCRC/876CNI/notice
checked andSHA256verified; neithercontainsuserdataorconnectionkeys:
- releases/ChooChooTracker-native-chips-r36h.zip
  12778957 bytes; SHA256 261cae73ef1833d5d37c6f9339ca46766a08a7aba455e6578fae5d9448b3a4d7
- releases/ChooChooTracker-native-chips-macos-x86_64.zip
  14429447 bytes; SHA256 bb652d177f03f376947522f4eb829b8c905d66fadd421c9f9b47208e4b8d18ef

This final checkpoint is documentation-only after the installed4b7bad7 build;
package/application identity deliberately remains4b7bad7. Localreceipt and
installlogs: .tmp/chip-audit/package-receipt.json, install-prepare.log,
install-install.log. Device installation-result.json andinstallation-plan.json
retain detailed verification; do not publish their user-file inventories.
No remaining automated delivery step. User listening/interactiveplaytest remains
pending,1000clearedDX7goalunmet67distinctfallbackdelivered. 10,000wasONLYsynthetic
browserstresstest, notbundledsounds. Total876presetfiles/812FMentries/704distinct
FMparametersets, includingOPLsourcealiases. HeavyFX/small-bufferlimitationsare
recordedinchip-instruments-report.md and64-case/600sCSV; configureddirect-card
physicalaudio passed both70sfixtureswith0miss/0ALSAwarnings.
# FM type-menu correction — 2026-10-04

DELIVERED: installed application source `0146fcf523de8d919d90938bc775d4bd7b29bde9`.
All seven FM entries are visible in the captured R36H menu; the real popup
selection/reopen regression and the existing UI suite pass. The rebuilt personal
binary passed a six-second offscreen startup using the regular direct ALSA route.
Installation preserved 1,652 other files and the regular launcher unchanged.
Verified full rollback:
`/roms/choochootracker-backups/pre-fm-menu-fix-0146fcf523de/previous-install`.
Installed binary SHA-256:
`1dccb2e18eae85d817d714fb63cbea57b7565fd1d8a816135d95141cdbc74d8e`.
Updated local release ZIPs each contain 876 presets and source `0146fcf`:
R36H SHA-256 `da714a4563698dd2286c62e68afae564f0c724abc088455ad06b826436c5b7de`;
Mac SHA-256 `17b21d613d195fb6a92e8b3f37a1dd1dc0852dcd4e0cb6265f1acd786bda9931`.
Regenerated tracked WebAssembly also passes Node compilation. This final
checkpoint is documentation-only; installed/package source remains `0146fcf`.
Both authorized fork branches include this fix; no PR is requested.

User found DX7 missing from Type → FM. The category's hard-coded child count
was five although its array held seven entries, hiding Arcade and DX7.
Derive the count from the array. The developer UI harness now navigates the
actual Type popup, selects each of the seven FM instruments, and reopens each
selection. This catches the gap left by earlier direct page initialization.

On the paired R36H, the new regression failed against the old menu count and
passed with the fix. The remaining UI audition/import/copy/clone/playback checks
also passed (song+UI p95 27,746.834 µs, p99 27,937.292 µs, worst 28,057.458 µs;
these include drawing and are not callback timing). Mac personal and Web builds
completed. This menu-only change does not alter DSP, voice limits, or presets.

User saved and closed the handheld app for installation. Task-local helpers/logs:
`.tmp/fm-menu-fix/`; remote
`/roms/choochootracker-native-chips-20261004/fm-menu-fix/`. Earlier reports below
remain historical evidence.
