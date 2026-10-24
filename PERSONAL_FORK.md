# My R36H build

## October 23 upstream update

The shared personal source incorporates upstream main through `4b58e480`.
Accepted FM optimizations, SoundFont browsing, and stop/mute smoothing use the
upstream implementations. This update also adopts on-demand synth allocation,
live engine switching, 32/48 kHz audio, compressor sidechain routing, random
slice/note FX, Sequence Trance Gate modulation, copy/paste and Song controls
fixes, the PortMaster microphone fallback, and the visual user guide.

SQ80 follows the new voice allocation and live-switching paths. Beads, Lucky,
personal appearance, browser storage, existing personal song identities, and
private-library compatibility remain. New RNS/RNN commands follow the existing
personal commands; project FX continue to serialize by name. Ableton Link stays
excluded. Private instruments and songs remain outside public packages.

Web publication and an update through the mounted handheld card are selected.
Installation preserves the existing launcher, settings, songs and USER assets,
with a focused rollback for replaced app files. Vita remains paused. Validation
and deployment receipts record the tested source and results.

## October 18 collection loading follow-up

Collection folders open directly with Right or EDIT/A. Folder/file header icons
replace the extra Browse folders step, and the third row reports actual loading
phases, including Unzipping. Selection stays below the previews and keeps its
idle marquee. Empty/null EXS labels use instrument filenames.

Preparation reuses resolved assets and collection directory indexes within one
load. Bounded temporary ZIP readers avoid repeated extraction between size
checks and decoding; CRC checks and PCM limits remain intact. No source audio
is converted. Device validation and rollback receipts accompany installation;
this follow-up does not deploy the web app or change charging behavior.

## October 18 device personal daily

This R36H daily combines the native FM channel-mask optimization with collection
browsing and soundbank compatibility on the October 17 installed source. The
selected soundbank row stays below the eight preview slots and long selected
names scroll after one second. Nested collection folders, existing Vorbis
decoding, compatible SFZ envelopes and an adaptive decoded-sample budget are
included. Original library audio is preserved without conversion.

The current public and sideloaded library layouts are authoritative, including
intentional deletions. Private library synchronization remains separate from
application packaging. The existing launcher, songs, settings and unrelated
USER banks are retained. Exact validation and rollback details are in daily
receipts. Web deployment and Vita are outside this update. Ableton Link remains
excluded.

## October 17 main harmonization

Both personal targets incorporate upstream main through `f1f9f197`, using the
privacy-cleaned equivalent history. The accepted MONK controls and ALL bank,
FORMANT insert, long tick-based LFOs, insert-menu layout, audio-input recorder
and synchronized tape echo are shared with upstream. SQ80/ESQ, Lucky, personal
appearance, browser persistence and the approved rough demos remain included.
Existing personal instrument and FX identities remain stable. Ableton Link,
its SDKs, build flags, controls and runtime are excluded from both targets.

The device and web are selected for this update; earlier web holds are
superseded. Build and deployment receipts identify the exact tested commit.
Private songs, settings and USER banks are preserved. Vita remains paused.

## October 15 handheld upstream reconciliation

The handheld candidate integrates upstream `1cf6141`, including the accepted
SID preset cleanup, waveform stability, SoundFont and Beads changes. SQ80,
MONK, Lucky and the private-library workflow remain personal additions.
Personal song IDs and all existing user data are retained. The upstream
manual is canonical for accepted features; personal controls are retained in
`docs/personal-controls.md`. Web deployment and Vita are outside this update.
The native build and 57 focused SID/SoundFont/Beads/MONK cases passed
(11,817,154 assertions). Full integration and hardware validation are pending.

## Permanent scope exclusion: Ableton Link

Owner decision, 2026-10-15: exclude Ableton Link and Link Audio from the personal
fork on every target, including R36H and web. This is a hard exception to the
normal policy of adopting accepted upstream features. Do not vendor/fetch the
SDK, compile or link its implementation, expose controls, or ship its runtime.
Turning the feature off by default does not satisfy this exclusion.

For every upstream integration, inspect Link-related changes and omit/remove the
implementation and dependencies before a personal build or deployment. Keep the
upstream `main` mirror intact. Reopening this scope requires an explicit new
owner instruction; upstream adoption or licensing changes do not reopen it.

The owner reports that the licensing arrangement covers the original project,
not this fork, and does not use the feature. This records the owner's scope
choice, not an independently verified legal conclusion. The older
`docs/ableton-link-findings.md` research is historical and is not authorization
to implement Link. The current personal source contains no Link implementation;
this policy update requires no application rebuild or device installation.

## October 15 Instrument live waveform stability

The shared Instrument view holds its live trace through brief monitor gaps and
draws it after control refreshes. Song rests remain in live mode with a flat
trace; stopping playback and the release tail restores the static preview after
250 ms of inactivity. Instrument changes and page reentry reset the live view.
This changes display state only. Audio rendering and preset values are unchanged.

## October 15 SID preset cleanup

The device personal build keeps SID source details in EDIT. On Instrument,
source-controlled fields show an asterisk and explain their owner; the legend
appears only when a visible home-page control is source-controlled.
Instrument and EDIT prevent changes to unused base values, while ordinary
controls and Phrase/Table overrides retain their existing semantics. Preset
selection shows the full sound name. SID USER presents one collection per ZIP
and uses single-file import filenames to retain model suffixes and avoid stale
embedded names. Existing source programs and Factory sounds are unchanged.

The private master library gains 64 original editable sounds in eight Studio
collections. Generated USER files remain outside Git and factory packaging.
The 18 existing sideload sounds remain intact. `tools/sid_prep/build_studio.py`
reproduces the additions; `Makefile.audit` builds the real-loader/audio auditor.
Local validation includes the native suite, focused SID/library regressions,
offscreen SDL captures, and all 82 sounds at 44.1/48 kHz and three pitches.
Exact hardware build, installation and rollback results are in task receipts.
Web deployment remains unchanged; no Link implementation or dependency is added.

## October 14 SoundFont preset-error review

The loader accepts the exact one-sample WAV loop-end boundary mismatch and a
counted zero alignment byte in an odd-sized PCM frame layout, with warnings.
The private collection scan covers 1,670 WAV/AIFF files: no load failures and
seven compatibility warnings, including the earlier sampler-specific loop fix.
Original assets and authored SFZ/EXS loop mappings remain unchanged.

Failed loads keep the requested preset selected, clear its playable handle, and
show Error plus the reason at the bottom of Instrument. Cycling continues from
the failed item in either direction. Stale async failures cannot overwrite a
replacement instrument; healthy presets retain the live waveform.

The owner selected `dcbbc87` and `6ef8508` for a handheld-only daily over
`50f3832`. Exact build and installation results are recorded in daily receipts.
Personal web remains paused; this integration commit skips automatic workflows.
Native and device validation gate installation. Existing songs, banks, settings
and the regular launcher are preserved with a verified application rollback.

## October 14 combined personal daily

The owner released the build/deployment hold and selected `dacc21e` on the
prepared upstream v1.0.16-alpha plus MAIN `583702d` integration. Both device and
web use this combined source. The new SID home layout and SQ80 uppercase EDIT
styling join SQ80, Beads, Lucky and MonkSynth. Stock Trance Gate stays ID 14;
experimental Beads uses 15. The converted Low Water song is installed alongside
the matching app only, with the old app and song preserved together for rollback.

Validation/deployment receipts record the final source and outcomes. Earlier
preparation holds below are historical and are superseded by this daily request.
Private USER banks, settings, samples and unaffected songs remain preserved.
Beads PR work and Vita remain held.

## October 14 alpha preparation — build and deployment held

Local candidate `candidate/personal-alpha-20261014` combines v1.0.16-alpha
(`3ae1364`) and post-release MAIN through `583702d` with the personal daily.
The post-release commits include the 16-bit lane FX refactor and Trance Gate.
Installed MonkSynth work is preserved in its own checkpoint (`af12cff`); the
original personal worktree, device installation and deployed web app are untouched.
SQ80 Rev4, Beads, Lucky and the accepted Song copy fix remain included.

The FX integration repairs mixed-row copy/paste and remaining byte-sized
playback, modulation and hint paths. Follow stock numbering and formats first:
Trance Gate retains upstream ID 14; experimental Beads is appended at 15.
Experiments adapt to stock refactors; old personal songs are migrated as needed
instead of changing stock IDs. No remapping is needed for stock Trance Gate songs.

`tools/migrate_legacy_beads.py` converts explicitly identified old personal songs
from Beads=14 to Beads=15, including track/master slots and stored ZIP projects.
It writes separate copies and preserves embedded audio and all other project data.
Low Water R3's held web bundle has been converted; a local converted copy and
original are staged in `../song-migrations/20261014-stock/`. Of 62 local song
paths checked, two copies of this one song needed conversion. The connected device
inventory covers 38 projects including autosave; only its matching Low Water
Draft 2 needs conversion, now staged. No browser surface is connected, so
browser user saves still need capture at rollout.
Live songs stay on the old numbering until the matching build is installed.
Do not run legacy conversion on stock Trance Gate files: the old number is
ambiguous without source provenance. Re-check this catalogue at future MAIN merges.

No compilation, app runtime tests, push, CI trigger, build or deployment is authorized
by this preparation. Add the owner's next selected branch here before releasing
the hold. The review and required later validation are recorded in
`docs/personal-alpha-20261014-review.txt`; preparation metadata is in
`docs/personal-alpha-20261014-preparation.json`. Earlier receipts below describe
previous builds, not validation of this candidate.

## October 13 Rev4 combined personal daily

Selected on personal baseline `98c82dc`: upstream MAIN `7ab9232` (merged PRs
#65, #67, #68, #70, #72 and #73), Song selection copy fix `9bd6e95`, and SQ80 Rev4
`eb21139`. Edit uses title case directly above the Velocity label. The earlier
hold is superseded by the owner's combined web/device daily request.

Upstream SID editor source is retained, insert categories keep Beads, and
Settings keeps Lucky alongside the Android working-folder entry. Lucky touch
targets and Quit now follow the displayed controls. Web Import project accepts
CCT, LSDj songs and SAV files with the upstream song picker. LSDj export remains
limited to upstream's desktop/Android targets. Exact tested commit, deployment,
preserved-file inventory and rollback are recorded in build/install receipts.
Web MIDI input/output from merged PR #73 is included. The Song copy fix is now
accepted upstream through PR #72. Beads PR follow-up remains held; Vita remains paused.

## SQ80/ESQ Rev3 checkpoint

Bank now sits above Preset with the patch EDIT control beside the name. Bank
EDIT+Left/Right changes FACTORY/USER; a bare EDIT opens the chooser on release.
Preset stepping is retained, and EDIT chords cannot trigger a release popup.
Selected with Beads pitch intervals for the next October 13 personal daily.
Native tests (665), Node checks (11) and local browser checks passed; exact web
and handheld build/install results are recorded in deployment receipts.

## Beads pitch interval checkpoint

The existing pitch byte now reaches exact semitones, including 40 = −12,
80 = unison and C0 = +12. Coarse edits move one semitone; fine edits keep
intermediate positions. Mapping stays monotonic and nearly linear. Saved byte
values are preserved, with tuning changes below 10 cents. This is a separate
checkpoint selected with SQ80 Rev3 for daily integration. Beads PR follow-up
remains held.

## October 13 combined personal daily

This candidate combines web Shift/Play right alignment (`f49f940`), lazy Beads
allocation with off-audio-thread pooled ownership (`8c7fc19`), and SQ80/ESQ Rev2
(`d5232b3`) on personal baseline `14b31a3`. The audio-command merge retains both
SQ80 previews and Beads snapshot ownership. Generated contribution web bundles
are excluded; both targets build from the combined committed source.

Device installation must preserve songs, samples, private USER banks and all
settings, with a verified rollback. Build/install receipts and the web
`build-info.json` identify the exact tested source. Beads PR follow-up is held
until the owner has evaluated the combined daily. Vita remains paused.

## SQ80/ESQ Rev2 checkpoint

Rev2 is now included in this combined personal candidate. It adds eleven performance FX, the revised main/editor
layout, velocity-preserving neutral preset auditions, and FACTORY/USER browsing.
USER browsing now keeps collections on the left and presets on the right.
Factory includes twelve original starters and one MIT-licensed example.
[Behavior and limitations](docs/sq80esq.md) and
[local validation](docs/sq80esq-rev2-validation.json). Existing installed/deployed receipts
continue to describe Rev1 or earlier builds.

## SQ80/ESQ experiment

The experimental/sq80esq branch adds the standalone C++ synth to both personal
targets, with USER preset browsing, a native parameter editor, playback override
indicators and persistent browser bank import. Private waveform/preset assets
stay outside source and public bundles. See [integration notes](docs/sq80esq.md).
Validation and installation receipts identify the deployed candidate.

## October 12 device evaluation

Both personal targets now include today’s standalone SID preset editor
(`f79a5fd`) and Beads granular insert (`3b84c30`), alongside merged PRs #61–63
and main’s PortMaster key-capture fix (`7c3ed6f`). Beads exposes eight insert
controls, including packed Shape/Freeze. The SID page edits owned instrument
parameters and marks playback overrides.

Audio controls, normalization and defaults remain stock main: 512 frames on
PortMaster and 1024 on web. The evaluation installation sets the device to
512 while retaining the previous settings in the verified rollback.
Decisive Battle — NERV FM is added to the browser’s editable rough demos
collection, with all nine samples embedded and existing browser edits preserved.
The original song credits are retained in that collection. Validation and
installation receipts identify the exact final source and binary.
The combined application at `53db569` passed 644 native cases, 40 focused
Lucky-enabled SID/Beads/buffer cases, nine Node checks and isolated browser
recovery and all 18 project round-trips. Physical listening remains owner
evaluation. Later documentation/test-only commits do not change ARM application
source.

## October 12 main harmonization

Shared source incorporates upstream `0bffff9`, including merged PRs #61, #62
and #63 (browser recovery, Night screen, spoken stereo test). Accepted shared
implementations are retained alongside personal appearance, Lucky and private
USER library support. Audio settings match main exactly: 32, 64, 128, 256,
512 and 1024 frames. The maintainer introduced configurable buffers in
`a05359a`; the temporary personal 4906 option was removed in `88f8c13`.
631 native tests and nine browser-storage/relay checks passed. Application
builds and deployment are held at the owner’s request; installed targets remain
unchanged. The spoken stereo test is included in both targets’ packaging.


## Local night title (October 11)

Graphics → Boot screen now saves Original/Night. Night is the personal default, with a one-time migration for existing profiles. Later Original/Night choices remain saved.
Night uses the original landscape and train silhouettes, seams and scroll
speeds with a softer palette. Its entire sky is procedural: layered gas wisps,
1,100 seeded stars, subtle twinkles and localized lavender highlights. The
sky closes its four-minute orbit exactly; fourteen shooting stars appear per
cycle. No generated landscape joins or static sky texture are used. The sky
updates at 10 Hz, with the original scenery cadence retained.

The contribution is isolated on `contribution/night-title` in `../night-title-pr`.
Four focused native tests pass (47 assertions), including identical complete
images when galaxy generation is split across draws. The browser build passes
Graphics save/reload and all six scenery-pixel comparisons. On the R36H, the
ARM64 build passes the Graphics toggle, persistence, renderer and exact-loop
checks. Galaxy generation is spread into roughly 15.7 ms slices instead of a
47 ms burst; the offscreen software renderer's total timings are not a measure
of physical display frame rate.

Installed device source: `83dd43f5340ae91e41a4e70b7dde4197e36c4120`.
Binary SHA-256: `d17d0d0b340892278613ebbaf5924a42fdc04fce016b2f3f57ab483509a953ef`.
The installation and rollback inventories were verified; 1,558 existing files,
including settings, songs, samples and private banks, remain unchanged.
Rollback: `/roms/choochootracker-backups/pre-night-20261011-83dd43f5340a/previous-install`.
The user confirmed Night looks good and smooth. The matching personal web update is
published through the existing exact-commit build workflow; its public
`build-info.json` remains the authority for the deployed version.

## Two personal build targets

`personal/r36h` is the common source for the **device personal build** (R36H)
and the **personal web app**. `main` remains a clean upstream mirror. Integrate
new upstream changes into personal while retaining the personal experiments;
reconcile features adopted upstream instead of keeping duplicate versions.
Check both affected targets when shared code changes. Neither target is a
separate application codebase, and personal is not merged back into main.

Browser settings, songs and imported USER presets are local to each browser/site;
device data does not synchronize automatically. Personal assets stay private
except for the owner's explicitly published `roughdemos` song collection.
In the browser, `[rough demos]` lives in `/projects/rough demos` and refreshes
from the bundle on reload. A one-time cleanup removes exact shipped copies from
the old `/user/projects/roughdemos` and `/user/projects/rough demos` folders,
preserving edits and extra files. User saves belong in `/user/projects`;
Continue remains the separate, single `/user/autosave.cct`. Isolated Chrome
checks cover both old folder names, Decisive Battle revision 1, persisted
cleanup, and preservation of edits, Continue and private presets. The web canvas
has no focus frame.
Its web-only packaging contains 19 projects, including the separately named
Continue snapshot, Decisive Battle, and the owner-approved 90s Rave - Neon
Afterglow arrangement under `90s rave/`. Browser edits remain local. The five
older sample-based saves are converted to self-contained archives for both
the handheld and web collection, with original backups kept outside Git.
Lucky uses a browser fetch/decoding adapter in the web target and its existing
native implementation on the device. Platform-specific gaps and test limits
must still be documented explicitly.
See [web deployment](docs/personal-web-deployment.md) for the build and storage
details.

## October 11 source integration

Personal now incorporates upstream `46f76a1`, including merged PRs #38, #51,
#52, #53, #54, #56 and #57 and their follow-up fixes. This adds the FM operator
editor, M8 instrument conversion, EQUAL/AUTO/LAZY sample slicing, phrase fill
and Note Lock tools, stereo pan, master inserts and parametric EQ, audio
settings, lower idle CPU use, and corrected browser touch coordinates.
Lucky remains above Quit in the reorganized Settings menu. The owner tested full songs successfully at the smaller buffer. Audio uses the standard 32–1024 choices; the extra personal 4906 option has been removed. The current handheld 256-frame setting stays saved. Web appearance, storage, controller defaults, recovery and the
approved roughdemos collection remain personal additions.
See [integration notes](docs/personal-harmonization-20261011.md).

## Device history

`personal/r36h` is the full device personal build. It includes Lucky and the
personal additions alongside upstream main, plus the Rotary speed, contextual
insert help, active TF1/TF2 selection and Shift navigation changes tested on
`experimental/rotary-rate-popup`.

October 7 reconciliation adopts upstream through `e8fa361`, including merged
PR #48's preset packs, USER browser, native gain compensation and sample-stretch
ownership fix. These use the shared upstream implementation. The separate
`contribution/rotary-insert-workflow` branch starts from upstream main and contains
only the Rotary/insert/navigation changes and their documentation and tests.

The installed application now includes community source imports and OPL cleanup from `b5bc205f24ca`.
Lucky and the personal features stay enabled. The regular launcher, settings,
songs and existing private banks remain intact. Supported community collections
are private USER assets; SynprezFM was already present and was not duplicated.
See [device validation](docs/preset-polish-device-20261007.json) for the checks
and rollback record.

Rotary uses the full 00-FF speed scale, from 0.01 to 25 Hz. Previous
Slow/Fast/Hyper/Chaos settings correspond approximately to 8F/CB/E7/FF.
Existing songs are not rewritten; human listening remains user evaluation.

- **Main** mirrors the original developer’s code.
- **Personal** is my full combined build: upstream code, my additions that haven’t
  been merged upstream, and my experiments. Unmerged features stay in Personal,
  including declined ones. Once a feature is incorporated upstream, Personal uses
  the maintainer’s version—with his fixes and changes—instead of my earlier code.

Other contributors’ unmerged work is not included.

Vita is a **paused project**, separate from the experiments. Its code stays at
`256fedf`; this update does not merge, build or deploy Vita. Resume only on request.

## Earlier personal additions (history)

This table records earlier additions, not the current installed implementation.
Updates use upstream's accepted versions; retain a deliberate personal difference
only when it is still needed or explicitly requested.

| Addition | Upstream proposal | Recorded personal behavior |
| --- | --- | --- |
| Share Tech Mono, Departure Mono, Spleen, Cozette + NostromoAmberDa2 | [#17](https://github.com/paiheulevrai/Choochootracker/pull/17) | Fonts and theme available |
| Reactive pixel piano | [#18](https://github.com/paiheulevrai/Choochootracker/pull/18) | Dark keys, orange theme outline and waveform-color highlights |
| Project page spacing | [#19](https://github.com/paiheulevrai/Choochootracker/pull/19) | Version and file actions separated |
| Track visuals | [#20](https://github.com/paiheulevrai/Choochootracker/pull/20) | One Detailed / Audio waveform choice per track |
| Persistent waveform | [#21](https://github.com/paiheulevrai/Choochootracker/pull/21) | Available with an ON/OFF setting |
| Mixer level meters | [#22](https://github.com/paiheulevrai/Choochootracker/pull/22) | Meters and numeric TRK labels |

The personal launcher cover also carries a diagonal red **GITHUB FORK** stamp.
This is a personal-only asset change, separate from the six upstream proposals.

The exact included commits and upstream base are in
[personal-features.json](personal-features.json). MIDI support is now included through upstream `main`, along with sample slicing,
key jazz, Settings submenus and the aChChid workflow improvements.

## Current personal source

The October 7 update adds upstream M8 song import/export, MIDI CC editing, chord
voicings, safer project saves, support reports and Opt navigation. Get Lucky
follows Support report in personal Settings. Factory/USER ZIP collections, bank
cycling, seamless USER stepping and calibrated native engines remain included.
See [the integration report](docs/personal-harmonization-20261007.txt).

Upstream base: `6406a749b8b01d2c3dc1292b5986444c856e81b6` (October 7 harmonization). Personal includes
upstream’s track-visuals/GPU integration, corrected Graphics touch mapping, sample
editor and processing updates, sample stretching, and bounce/export improvements.
Native chip instruments and Get Lucky remain included. New saves use upstream’s
self-contained sample archives when audio is loaded; older text songs still open.
Native patches remain stored inside the song. Existing songs are converted when
explicitly saved, not rewritten in bulk.

Insert effects now uses the maintainer’s accepted implementation from
[PR #31](https://github.com/paiheulevrai/Choochootracker/pull/31) and
[review PR #34](https://github.com/paiheulevrai/Choochootracker/pull/34), including
the follow-up review polish: grouped module chooser, CPU readout, full-height
Insert FX page and hint cleanup. The earlier temporary exception is removed.
Native-chip preset audition and Get Lucky remain included. See
[the insert review report](docs/personal-insert-review-20261004.txt) and
[the preceding sync report](docs/personal-upstream-sync-20261004.txt).

Native tracker FX now use a compact per-engine list. OPLL/VRC7/OPL2/OPL3 keep
operator-1 envelope and multiplier controls; Genesis/Arcade keep the multiplier
and LFO controls; DX7 keeps operator levels and feedback. The operator selector
is removed. Live fixed-operator modulation and synthesis remain unchanged.
SID waveform, macro speed and partner ratio use native numbering.
See [the simplification notes](docs/fx-simplification-20261005.txt).

Personal-only relative FM commands and native phrase-volume migration are
removed. A one-time conversion updates the affected personal songs and our
bundled native audition. Upstream legacy-format support remains. See
[the complete audit](docs/absolute-fx-audit-20261005.txt). The October 6 harmonized source is installed; the earlier simplification receipt remains as history.

## Preset collection browser update

All 11 native engines use Collection: ALL, Factory Presets and/or named included
collections, and USER. ALL also lists compatible USER sounds under Unsorted,
including ZIP contents and DX7 bank voices. USER preserves folder navigation.
OPLL/VRC7 tone sets become one Factory Presets list per engine. DX7 combines
YSE with ChooChoo Factory Presets and keeps OpenDX7 Originals separate.

## Current installed device build

The extra library is named **Non-attributable**. Factory-source duplicates are
excluded from its two OPL packs; Factory assets and offline originals remain.
Unnamed empty OPL slots are hidden, and playable unnamed voices use source
program/bank labels. Both Downloads GB folders explain the shared four examples
and their source-controlled duty, pitch and envelopes.

Source: `b5bc205f24ca6ad42acd111f7d9a067434c85f19`. Community source formats load through
the engine USER folders, including bank files and ZIPs. Host validation, focused
ARM import/save-load tests and offscreen production browser checks passed.
Existing settings, songs, private banks and the launcher were verified unchanged.
The complete previous install is retained for rollback. Listening remains user
evaluation; performance benchmarks remain skipped at the user's request.

## Previous installed device build (October 7 collections)

Source: `c0a083ab980ea022ab0d497a19434a08fc885796`, harmonized with upstream main
`6406a74`. Includes upstream 1.0.15 changes and the personal Factory/USER ZIP
collections, bank cycling, continuous USER preset stepping, calibrated engines
and Get Lucky. Vita is paused and was neither synchronized nor deployed.

527 host tests and 25 focused ARM tests passed. The complete ARM package,
web bundle, all 11 engines' Bank navigation, Sega/GB lists, USER stepping and
Settings coexistence checks passed. No listening or calibration tests were run.
853 existing files, settings, all 32 private SYX banks and the launcher were
verified preserved. A complete rollback was verified before replacement.
See [the installation receipt](docs/personal-harmonization-device-20261007.json).

## Previous installed device build (USER preset stepping)

Source: `117d476ff29fb4e5a783ed9e197962e1557417f5`. USER now supports EDIT +
Left/Right preset stepping, continuing across banks, folders and ZIPs and
wrapping at either end. The exact selected file and voice are tracked during
the session, so duplicate names retain their position. Reopening USER follows
the current sound. Existing Bank cycling and flat Sega/Game Boy lists remain.

Four focused host checks and the device navigation check passed, covering bank
boundaries, CNI folders, duplicate names, wrap, browser focus and missing files.
No sound tests were run. Web rebuilt. Settings, all 32 private SYX banks and
other user files are preserved; the launcher and verified rollback are intact.
See [navigation receipt](docs/user-preset-step-device-20261007.json).

## Previous installed device build (October 7 Bank cycling)

Source: `209b3eef3f96001d3e14761ab42163ddb1bfbe34`. Hold EDIT + Left/Right on
Bank to cycle all available sources, with wraparound, on all 11 Bank-enabled
engines. The loaded instrument stays unchanged. Normal EDIT tap still opens
the chooser. Existing flat Sega/Game Boy preset browsing remains included.

Only the focused UI button check was run on the handheld: order, wrap, release,
tap and unchanged instrument/dirty state. No sound tests, as requested. Web
rebuilt. Settings, all 32 private SYX banks and other user files are preserved;
the regular launcher is unchanged and rollback is verified.
See [UI update receipt](docs/bank-cycle-ui-device-20261007.json).

## Previous installed device build (October 7 flat preset UI)

Source: `14023d88133eec58d76be078625cd0e46d1b853c`. The Instrument page now says
**Bank:**. Sega PSG, GB Pulse and GB Noise retain ALL, Factory and USER;
ALL/Factory open flat preset lists, with USER sounds included in ALL.
USER retains folder/ZIP browsing. Other engines keep their category menus.

Only focused UI navigation and visual checks were run; no sound tests, as
requested. Web rebuilt. Settings, all 32 private SYX banks and other user files
are preserved, the regular launcher is unchanged and rollback is verified.
See [UI update receipt](docs/bank-flat-ui-device-20261007.json).

## Previous installed device build (October 7 collections)

Source: `aa794cbf1daaf853281904bbd62be90910d53d28`. Engine-specific collections,
Factory Presets, separate named downloaded collections and top-level USER.
ALL also includes USER presets and DX7 bank voices under Unsorted. YSE joins
DX7 Factory Presets; OpenDX7 Originals remains separate. Native gain compensation
and the rest of the personal build remain included.

Validation: 516 host cases, 14 final focused cases, 66 ARM cases and production
browser checks passed. Web rebuilt. Audio startup passed with the waveform off
and on. The balanced audio probe had one render deadline miss and no logged
ALSA underruns; human listening remains pending. All 32 private SYX banks,
settings and other user files were preserved; the launcher is unchanged and
rollback is verified. See [device validation](docs/factory-user-collections-device-20261007.json).

## Previous installed device build (October 7 ZIP presets)

Source: `bd1fe7166b3e524ca20b3cfacef413281b4c0518`. October 7 ZIP collections, USER folder browsing
and measured native engine gain compensation. Production code was compiled at
`0b4bca780ab0892d8c75e634984e76f697fd09fc`; the later source commit changes only the device test harness.
Personal features and Mod Lucky remain included.

The 62 selected ARM tests, production browser checks, package integrity and
70-second direct-card audio check passed. Startup passed with the waveform
display off and on. Settings, songs, fonts, themes and all 32 private SYX banks
were preserved; the regular launcher is unchanged and rollback is verified.
Human listening remains pending. See [device validation](docs/preset-packs-device-20261007.json).

Included collections live in `instruments/FACTORY/`; new personal files go in
`instruments/USER/<engine>/`. Existing `instruments/banks/<engine>/` files remain
accessible through USER → Previous banks folder.

## Previous installed device build (October 6 harmonization)

Source: `df6edc1aa3548d3bf5d9b3cacdd81ba53ce214c1`. October 6 upstream harmonization, native waveform previews
and maintainer fixes, with personal additions and Mod Lucky preserved.
Host suites, ARM production build, package integrity, runtime libraries and
focused device FX UI checks passed. Settings, songs, assets and launcher preserved.
Human listening remains for user evaluation. See the
[installation receipt](docs/personal-device-harmonization-20261006.json).

## Previous installed device build (FX simplification)

Source: `ee0e85c3cde127e24c138d8e58e14ead0cf5a6a2`. Compact tracker FX lists and consistent description
colors; Preset/Range retains its distinct color. Synthesis and live modulation
are unchanged. Settings, songs, assets and launcher were preserved.

Focused host/device checks and the production build passed. No full suite,
performance or listening tests were run. See the
[installation receipt](docs/fx-simplification-device-20261005.json) and
[simplification notes](docs/fx-simplification-20261005.txt).

## Previous installed device build (absolute FX)

Source: `f4b086d5f18100ba239b018a2abaf2dc10bb81af`. Absolute native FM controls, fixed-operator
modulation, SID native numbering, and compact descriptions/preset/range readouts.
Eleven personal/test songs were converted once; their original files are retained
in the verified rollback. Personal compatibility translations are removed.
AY, upstream compatibility and stock songs are unchanged.

Host and ARM functional suites, production UI and startup checks passed. GitHub
CI passed for this source. Performance tests were skipped at the user's request;
startup used the ALSA null output and human listening is pending. See
[the installation receipt](docs/absolute-fx-device-20261005.json).

## Previous installed device build (R2)

Upstream base: `a02a88098a03b10518806268f039b9d9b8b2f5a9` (October 2).
The device channel is **device personal build**: upstream code plus my unmerged
additions and experiments. Pending or declined submissions stay included; once
incorporated upstream, the maintainer’s version replaces my earlier version.

| PR | GitHub status | How it reaches the handheld |
| --- | --- | --- |
| #1 Stick live toggle | Merged | Upstream main, including ALWAYS ON |
| #17 Fonts/theme | Merged | Upstream main |
| #18 Piano | Closed as superseded by #23 | Upstream's integrated implementation |
| #19 Project spacing | Merged | Upstream main |
| #20 Track visuals | Merged | Retained personal Detailed / Audio waveform setting; upstream proposal accepted |
| #21 Persistent waveform | Merged | Upstream feature with retained personal layout, default OFF |
| #22 Mixer meters | Closed as superseded by #23 | Upstream's integrated implementation |
| Track Insert FX | [#31 merged](https://github.com/paiheulevrai/Choochootracker/pull/31) | That R2 build used our earlier implementation; the current build uses the maintainer’s merged review |
| Native chip instruments | Personal feature; no upstream PR | Ten chip types, shared FM browsing, local DX7 import and 876 native presets; ARM64 tests and configured-device audio validated |
| I’m Feeling Lucky | Personal experiment; no upstream PR | One Settings row, compiled only with `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1` |

R2 adds FM onset smoothing, optional amp ADSR and tone macros, Sega low-note
extension, chip automation and the ADSR graph fix. Host and ARM tests, personal desktop/Web, SDL UI, mixed benchmarks and
physical ALSA checks pass. R2 listening acceptance remains pending; the installed
source and rollback are recorded in personal-build.json and the progress log. See [the current report](docs/chip-instruments-report.md).

Piano/meters are not duplicated just because their original proposal commits
remain in Git history. GitHub's ahead/behind counts describe history; they are
not a count of missing features. No original PR needs closing for #18 or #22:
the maintainer already closed them with an explanation.

The updated source uses upstream’s Graphics menu and rendering integration,
with compatibility for our native chip types. MIDI and Chord mode come from main.
The installed `personal-build.json` records
the exact source commit, upstream base, binary hash, checks and rollback location.

## Branches and contribution groups

The two primary branches are [`main`](https://github.com/aiaaaa/Choochootracker/tree/main),
the upstream mirror, and [`personal/r36h`](https://github.com/aiaaaa/Choochootracker/tree/personal/r36h),
the combined source for the device personal build and this fork's default branch.

The groups below describe the work's purpose. Existing branch names and PRs stay
intact; a group's name does not require renaming its branches. Submission moves
work from Upstream candidates to Submitted contributions. Later status changes
update that entry rather than moving it to another group.

### Experimental

Personal explorations with no commitment to an upstream submission.

| Work | Branch | Notes |
| --- | --- | --- |
| SQ80/ESQ synth | [`experimental/sq80esq`](https://github.com/aiaaaa/Choochootracker/tree/experimental/sq80esq) | Opt-in C++ engine and preset editor; [instructions](docs/sq80esq.md) |
| Get Lucky (I’m Feeling Lucky) | [`experimental/mod-lucky-integrated`](https://github.com/aiaaaa/Choochootracker/tree/experimental/mod-lucky-integrated) | Opt-in sample discovery; [build instructions and limits](docs/mod-lucky.md) |

### Native instruments accepted upstream

[PR #39](https://github.com/paiheulevrai/Choochootracker/pull/39) is merged.
Personal now adopts the maintained waveform previews, compact native FX grouping,
SID preview guards and CI/Windows fixes. Earlier proposal branches and revision
notes remain as history. See [the synchronization record](docs/personal-harmonization-20261006.txt).

### Submitted contributions

Every submitted proposal stays here, whether pending, merged, closed, or
incorporated differently. These branches retain proposal history; normal updates
come through upstream `main`, with personal choices reconciled on `personal/r36h`.
Status checked on October 6, 2026; follow the PR links for subsequent changes.

| Contribution | Branch | PR | Status |
| --- | --- | --- | --- |
| Stick live toggle | [`feature/stick-live-toggle`](https://github.com/aiaaaa/Choochootracker/tree/feature/stick-live-toggle) | [#1](https://github.com/paiheulevrai/Choochootracker/pull/1) | Merged |
| Fonts and theme | [`contribution/fonts-16x24`](https://github.com/aiaaaa/Choochootracker/tree/contribution/fonts-16x24) | [#17](https://github.com/paiheulevrai/Choochootracker/pull/17) | Merged |
| Reactive piano | [`contribution/piano-visualization`](https://github.com/aiaaaa/Choochootracker/tree/contribution/piano-visualization) | [#18](https://github.com/paiheulevrai/Choochootracker/pull/18) | Closed; incorporated through #23 |
| Project page spacing | [`contribution/project-page-spacing`](https://github.com/aiaaaa/Choochootracker/tree/contribution/project-page-spacing) | [#19](https://github.com/paiheulevrai/Choochootracker/pull/19) | Merged |
| Track visuals | [`contribution/track-visuals`](https://github.com/aiaaaa/Choochootracker/tree/contribution/track-visuals) | [#20](https://github.com/paiheulevrai/Choochootracker/pull/20) | Merged |
| Persistent waveform | [`contribution/persistent-waveform`](https://github.com/aiaaaa/Choochootracker/tree/contribution/persistent-waveform) | [#21](https://github.com/paiheulevrai/Choochootracker/pull/21) | Merged |
| Mixer level meters | [`contribution/mixer-level-meters`](https://github.com/aiaaaa/Choochootracker/tree/contribution/mixer-level-meters) | [#22](https://github.com/paiheulevrai/Choochootracker/pull/22) | Closed; incorporated through #23 |
| Track insert effects | [`feature/track-insert-fx`](https://github.com/aiaaaa/Choochootracker/tree/feature/track-insert-fx) | [#31](https://github.com/paiheulevrai/Choochootracker/pull/31) | Merged |
| Native chip and FM instruments | [`contribution/native-chip-instruments`](https://github.com/aiaaaa/Choochootracker/tree/contribution/native-chip-instruments) | [#39](https://github.com/paiheulevrai/Choochootracker/pull/39) | Merged |

### After an upstream merge

Use the maintainer’s merged version, including his fixes and changes, in place
of my earlier implementation. Check for and remove duplicate code or controls.
The old branch can remain as history. Insert effects now follows this policy too.

This policy applies at the next tested update; it does not mean the installed
build has already been updated. Keep all other unmerged personal additions.

## Updating

Updates are deliberate, tested installs, not unattended downloads at boot.
Start with a clean worktree on `personal/r36h`:

```sh
git fetch upstream main
git fetch origin
# Review upstream changes before merging them.
git log --oneline HEAD..upstream/main
git merge --no-ff upstream/main
make -C tracker -f Makefile.test -j4
```

Resolve source conflicts individually, adopting the accepted upstream version
of merged contributions and keeping only selected additions not covered by it.
Check changed or squashed merges for leftover personal code even when Git reports
no conflict. Generated `web/dist` conflicts require a fresh combined web build,
not choosing one branch's bundle as the finished result. Follow
[build notes](docs/build-notes.md) for the web and ARM64 PortMaster builds, then
validate `releases/choochootracker.zip` with `unzip -t`.

Before installing, check the combined Settings entries, waveform ON/OFF and
instrument previews, Track visuals preferences, piano colors and note states,
Mixer meters, Project actions, and font/theme loading. Check playback and
controls on the handheld. Audio snapshots have previously added about 9.5% to
native render time; visual toggles currently do not remove capture overhead.

Update the manifest's upstream commit and any feature decisions, build and validate the
web target, and push `personal/r36h`. Fast-forward the fork's `main`
to the tested upstream base separately. Keep failed candidates off the device.

## One handheld installation

Use one regular `ChooChooTracker` launcher and `/roms/ports/choochootracker` as
the durable installation. Preserve the active personal settings, font, theme,
songs, instruments and autosave when consolidating or updating. Keep a verified
rollback copy before replacing files; conflicting user files must be preserved.
Retire the extra test launcher only after the combined package is validated.

Install builds made from `personal/r36h`. A stock PortMaster update can replace
the custom executable, so use this fork's tested packages for future updates.
A source-commit record alongside the installed binary identifies its exact build.

## Optional sample discovery build

The code and reproducible build instructions live in this fork. Normal builds
on native targets default `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY` to **0** and omit the row and its
dependencies. The personal web target and dedicated personal PortMaster target
enable it. The web build uses the same pinned decoder with a browser adapter;
see [web deployment](docs/personal-web-deployment.md). For PortMaster:

```sh
make -C tracker -f Makefile.personal PortMaster
```

Prepare the pinned backend with the existing target toolchain first; see
[Mod Lucky](docs/mod-lucky.md) for dependency setup, desktop commands, limits,
source attribution, test results and device status. This build selection is
compile-time only. It does not add a runtime toggle or submenu.

## Community source presets (October 7, 2026)

USER folders now import engine-compatible SYX, TFI, OPM, WOPL/OPLI/WOPLX/SBI,
FUI, DMP and GoatTracker GTI5 INS sources directly, including bank files and ZIPs.
The [user manual](docs/USER_MANUAL.md#portable-factory-collections-and-user-presets)
lists supported versions and exclusions. Imported sequence data stays in the
owned patch, using CNI 9 / CCT 10 while keeping the 696-byte instrument snapshot.
Host validation passed 540 cases plus the ready-library ZIP browser audit.
Community downloads remain private assets outside this repository. SID-Wizard
SWI and advanced tracker dependencies are explicitly outside this update's scope
by user agreement on October 7. Compatible static FM patches and the supported
PSG/GB/SID sequences remain included. These changes are installed on the handheld.
ARM functional checks passed; user listening remains pending. Performance benchmarks remain skipped at the
user's request.

The community collections and SynprezFM are permanent private assets for every
device personal build. Updates carry the existing USER library forward alongside
songs and settings. Offline originals and ready packs remain outside Git; they
are never part of factory assets or shared release packages. Identical collection
copies are not added again.

Listening feedback retains the small Game Boy and SID libraries as import
examples. The four GB source instruments stay available to both pulse and noise;
their authored duty, fixed-note and volume sequences can override base controls.
Both private GB folders in Downloads explain this behavior. The OPL2/3 WOPL
importer now hides unflagged empty slots and names playable unnamed voices by
their source program/bank. The extra library is named **Non-attributable**, with
source credits and licenses retained. FatMan 2-op, FatMan 4-op and DMXOPL3 WOPLX
copies are excluded from its OPL USER packs because Factory already includes
those sources. Their offline originals remain intact.


## MonkSynth integration — October 13, 2026

The personal source includes JonET/monksynth's MIT-licensed vocal core, pinned to
`53fdf3927e9d75930e92e5c919196d19a3ae7704`. Instrument type 29 is appended without
renumbering existing instruments. The page uses shared filter/ADSR controls,
with continuous vowel landmarks and an eight-control effect picker ordered
Vowel, Voice character, Breath, Glide, Vibrato depth/rate, Cutoff, Resonance.
Existing macro IDs are shared contextually; the 254-command namespace is unchanged.
Native and web targets discover the C++ wrapper through their existing synth
source lists. See the user manual and vendored source record. This engine is installed in the device personal build. Public web deployment
remains pending.

Validation passed: all 663 host tests (113,564,925 assertions), including six
MonkSynth regressions; offscreen production UI and touch mapping in both waveform
header modes; Emscripten compilation; Chrome audio output and saved-project
reopening. The eight-second vocal sweep rendered in 0.034 seconds on the audited
Mac (not a handheld performance claim). DSP storage adds about 5.13 MiB for all
32 tracker voice slots. The pinned source and packaged license match upstream
byte-for-byte. Local logs, captures and an audition WAV are in `.tmp/monksynth/`.
Handheld verification passed: ARM64 build with Lucky and SQ80 enabled,
six MonkSynth engine regressions, production UI checks and voice benchmark.
See personal-device-validation.json for the exact source snapshot and results.
Physical listening remains pending user evaluation.

## SID home cleanup — October 14, 2026

`feature/sid-home-cleanup` is prepared on daily candidate `7d37f79`, including
v1.0.16-alpha and post-release MAIN's 16-bit lane FX refactor through `583702d`.
SID now has labeled Bank/Preset rows, EDIT beneath PAN/VOL, separate Wave/PW
fields, a Filter section with Mode above Cutoff/Resonance, and one native ADSR
row. SQ80 uses the shared uppercase EDIT action styling.

Validation: production SDL UI with SQ80 enabled, both waveform header modes,
exact touch/drag targets, editor routing and Phrase/Table FX bounds/defaults;
seven SID editor regressions (14,592 assertions), including active lane FX;
Emscripten web build and final changed-source compilation. Native patch ranges,
lane command IDs, normalized SCP/SCT values and saved automation are preserved.
Prepared for daily integration and review; no device or public web deployment.

## Song-only waveform — October 14, 2026

`feature/song-only-waveform` branches from daily candidate `44e29a2`.
The persistent header visualizer now appears only on SONG. Graphics calls the
option **Song waveform** and retains the existing saved preference. Other pages
keep their full row capacity, spaced Modulation controls, and full instrument
waveform/sample/envelope previews. Audio and lane FX behavior are unchanged.

Validation: two layout regressions (142 assertions); offscreen production SDL
checks across nine pages in both setting states, including transitions from
SONG, bottom Phrase row edits and Modulation touch targets. All eight non-SONG
page captures match byte-for-byte with the setting on/off. SID/SQ80 and Monk
production UI routing, touch and lane FX checks also pass. The Emscripten web
build passes with SQ80 and Lucky enabled. Captures and build logs are local
under `.tmp/song-waveform/`. Prepared for daily integration;
no device or public web deployment.

## Combined evaluation: SID, SONG waveform and SoundFont

Includes SID home cleanup dacc21e, SONG-only waveform 61994d2, and native
SoundBanks dd4fc00. SID layout, SONG waveform and SoundFont are intended as
separate upstream contributions after owner evaluation. SQ80/ESQ, Monk and Beads
remain personal experiments. Preserve SQ80=28 and Monk=29; SoundFont=30 avoids
reinterpreting existing songs. Generated SoundBank branch web assets are excluded;
the personal pipeline builds fresh output. Source library originals remain private.
Combined build and runtime results are recorded in the evaluation receipt.

SoundFont banks share the private USER library at `instruments/USER/soundfont/`
(browser: `/user/instruments/USER/soundfont/`), preserving collection folders
and relative sample references.

### SoundFont direct editor (2026-10-14)

The isolated experimental/soundfont-edit branch adds instrument-owned tuning,
filter and envelope overrides, defaulting to the bank-authored sound. The active
preset row has an Edit action including volume and pan. Eight contextual Lane FX
expose pitch, volume, pan, cutoff, resonance, attack, decay and release; their IDs
are appended after the existing commands. Instrument defaults and source banks
remain unchanged by track automation.

The duplicate Bank Manager flow is replaced by scan status/Rescan and one folder
browser. Preset cycling wraps across indexed presets; asynchronous preparation
keeps the latest selection and retains playable audio on failure. The branch
remains isolated until a requested daily integration; no deployment is included.


### SoundFont evaluation daily follow-up (2026-10-14)

The owner authorized integrating experimental/soundfont-edit through c95918e
onto the installed 9fb8149 personal baseline for both web and R36H. This includes
the direct editor, eight Lane FX, scan status/Rescan, one folder browser and
wrapping preset cycling. Existing personal experiments and upstream baseline
remain present. Daily build receipts record the exact commit, verification and
deployment; private USER banks, songs and settings stay external and preserved.


### SoundFont device review follow-up (2026-10-14)

Based on the verified 5e8679e daily. Keep preset notifications on Instrument,
separate compatibility warnings, remove the library path label, and clean
collection display without renaming private assets. Follow the SoundFont pan
generator instead of forcing header-flag panning. Preserve authored key ranges.
Add a per-instrument PBN bend step (new instruments 1 cent; older files Stock),
replace menu duplicates FVL/FPN with Hold/Sustain while preserving old IDs and
playback, and update active-voice decay/sustain/release controls. Both web and
R36H require verified daily builds; private songs/banks/settings remain external.

### SoundFont collections and compatibility review (2026-10-14)

The collection browser opens directly on the active preset and presents one
collection column and one flattened preset column. EDIT+Left/Right follows that
same sorted order across collection boundaries. Native keys shows the authored
note-on range without changing playback mappings. Generic custom-modulation
warnings move to manual guidance that distinguishes a bank file from its presets
and lists supported and unsupported programming.

The private Dropbox library is the canonical collection reference. Clean names
retain meaningful musical numbers; source audio and definitions remain unchanged.
Original downloads/support files belong under documents. The owner confirmed
there are no existing SoundFont songs to migrate, so this update uses the new
paths directly without a compatibility map. No private assets enter Git or the
public web deployment. Exact test/deployment results belong in the daily receipts.

### SoundFont note-transition and WAV review (2026-10-14)

Follow-up to installed 2a119c6: smooth retriggers, KILL and immediate note release
with a 3 ms output correction, including removed chord voices. The correction
retains no old sample pointers and does not alter authored note ranges or source
audio. Unknown sampler-specific WAV loop types warn and leave that embedded loop
disabled; explicit SFZ/EXS zone loops remain authoritative. Standard alternating
and backward WAV loops remain unsupported.

Local reproduction uses the private Stargate sounds without adding assets to Git.
Validation receipts are under `.tmp/soundfont-retrigger-20261014/`. This follow-up
was subsequently selected by the owner for a new web and device personal daily.
The two fixes remain separate commits (`7934432`, `588cfbf`); validation and
installation receipts record the final build and rollout.


## MONK and Lane FX handheld evaluation — October 15, 2026

Integrated current upstream main (1cf6141) while preserving personal engines and
private library paths, with Ableton Link excluded. The standalone Lane FX popup
cleanup remains a separate upstream contribution, ahead of MONK.

MONK now uses the complete native unison, envelope and stereo delay engine with
all sixteen parameters saved in CNI/song data. Home exposes the nine performance
controls; EDIT exposes all sixteen. Five original MIT factory presets and the
empty USER/monk directory follow the shared browser. Removed the tracker post
filter and duplicate envelope. At the owner's request, there is no legacy MONK
DSP branch: migrate the two device songs and autosave directly, retaining their
original files for rollback. Convert envelope time bytes and set Level to 40 hex
to retain the former track mix attenuation.

Handheld evaluation follow-up: the FX group list starts one row below the help
block, and all groups use up to eight columns except the five-command Modulation
rows. All sixteen MONK controls now appear in its synth group; the shared Trigger
group keeps TDC/TCL without duplicate envelope entries.

### Collection browser follow-up (2026-10-19)

Collections use the full left panel height, starting at the top; presets retain
the selected row below the eight previews. Folder and three-line sound icons
appear on entries as well as headers. Right pages through presets without loading;
EDIT selects, and PLAY starts/stops the song. The bottom control legend is removed
at the owner's request. Instrument-page cycling follows the same nested folder
order and caches that order after a scan. Exact rename receipts keep saved bank
references working after private library cleanup. No private assets are bundled.

Song-stop smoothing is maintained as a separate commit and local branch for a
future contribution; see `docs/playback-stop-smoothing.md`.

## October 19 codec removal

MP3 and Opus implementations and dependencies are removed. Existing Vorbis,
WAV/AIFF and SoundFont playback remain. Unsupported private samples are reviewed
separately in OffFormat; private libraries are not part of this source tree.
