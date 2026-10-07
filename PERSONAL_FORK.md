# My R36H build

Test branch `experimental/rotary-rate-popup` adds insert parameter descriptions
to the Phrase/Table FX popup, continuous Rotary speed from 0.01 to 25 Hz, and
Shift+Left/Right exits from branch pages to the MSCPIT row. It starts from the
October 7 device personal build. Rotary's saved speed bytes now use the full
00-FF scale; previous Slow/Fast/Hyper/Chaos settings correspond approximately
to 8F/CB/E7/FF. Songs and private assets are not rewritten during installation.
The device receipt records validation, installation and rollback separately.

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

Update the manifest's upstream commit and any feature decisions, commit the
regenerated web bundle, and push `personal/r36h`. Fast-forward the fork's `main`
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
default `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY` to **0** and omit the row and its
dependencies. The dedicated personal PortMaster target enables it:

```sh
make -C tracker -f Makefile.personal PortMaster
```

Prepare the pinned backend with the existing target toolchain first; see
[Mod Lucky](docs/mod-lucky.md) for dependency setup, desktop commands, limits,
source attribution, test results and device status. This build selection is
compile-time only. It does not add a runtime toggle or submenu.
