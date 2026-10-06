# My R36H build

- **Main** mirrors the original developer’s code.
- **Personal** is my full combined build: upstream code, my additions that haven’t
  been merged upstream, and my experiments. Unmerged features stay in Personal,
  including declined ones. Once a feature is incorporated upstream, Personal uses
  the maintainer’s version—with his fixes and changes—instead of my earlier code.

Other contributors’ unmerged work is not included.

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

Upstream base: `8c8709182b90a492ae8b5e2fc0bb0e7516644388` (October 5 harmonization). Personal includes
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

The new native chips now use absolute operator envelope, frequency, detune and
LFO commands, with `FOP` selecting the operator. Modulation bindings name a fixed
operator. SID waveform, macro speed and partner ratio use native numbering.
AY and upstream command semantics remain unchanged.

Personal-only relative FM commands and native phrase-volume migration are
removed. A one-time conversion updates the affected personal songs and our
bundled native audition. Upstream legacy-format support remains. See
[the complete audit](docs/absolute-fx-audit-20261005.txt). This source revision is
awaiting its device installation record; performance tests are skipped at the
user's request.

## Current installed device build

Source: `5bfab9779f317f4fc6580292f285d0104917e52b` (October 5 upstream harmonization and native FX revision).
Includes upstream phrase volume 00–7F, seven new inserts, export fixes and build
workflow fixes. Older native songs retain their loudness through load migration.
Native FX editing uses the row instrument's preset value and legal range;
OL1–OL6 and FBK give absolute operator levels and feedback. The last DX7
commands and complete ranges fit with the persistent waveform off and on.

Get Lucky, all 1196 native presets, persistent DX7 banks and personal features
remain. Host and ARM64 suites, production UI, five physical ALSA workloads and
startup with waveform off/on passed. The receipt retains the initial SID timing
miss and ALSA underrun, plus the clean follow-up trials. User listening remains pending.

The regular launcher and 2008 existing user files were preserved, with
verified installation and rollback inventories. See [the device receipt](docs/native-fx-values-device-20261005.json)
for hashes, timing and rollback. The [prior FM macro receipt](docs/fm-macros-device-20261005.json)
remains historical. This receipt update does not change the native binary.

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

### Upstream candidates

Active work that may become a future PR. Listing here does not mean it is ready
for submission; each candidate still needs its own review and testing.

| Work | Branch | Notes |
| --- | --- | --- |
| Native chip instruments | [`feature/native-chip-instruments`](https://github.com/aiaaaa/Choochootracker/tree/feature/native-chip-instruments) | No upstream PR; [validation report and remaining work](docs/chip-instruments-report.md) |

### Submitted contributions

Every submitted proposal stays here, whether pending, merged, closed, or
incorporated differently. These branches retain proposal history; normal updates
come through upstream `main`, with personal choices reconciled on `personal/r36h`.
Status checked on October 4, 2026; follow the PR links for subsequent changes.

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
