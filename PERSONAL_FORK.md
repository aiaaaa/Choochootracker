# My R36H build

This is my personal ChooChooTracker build: the author's `main` branch plus the
features I want on my handheld. These additions stay here even if their upstream
pull requests are declined. Other contributors' unmerged work is not included.

## Included features

| Addition | Upstream proposal | Personal choice |
| --- | --- | --- |
| Share Tech Mono, Departure Mono, Spleen, Cozette + NostromoAmberDa2 | [#17](https://github.com/paiheulevrai/Choochootracker/pull/17) | Keep the fonts and theme available |
| Reactive pixel piano | [#18](https://github.com/paiheulevrai/Choochootracker/pull/18) | Keep the dark keys, orange theme outline and waveform-color highlights |
| Project page spacing | [#19](https://github.com/paiheulevrai/Choochootracker/pull/19) | Keep version and file actions separated |
| Track visuals | [#20](https://github.com/paiheulevrai/Choochootracker/pull/20) | Keep one Detailed / Audio waveform choice per track |
| Persistent waveform | [#21](https://github.com/paiheulevrai/Choochootracker/pull/21) | Keep it available with an ON/OFF setting |
| Mixer level meters | [#22](https://github.com/paiheulevrai/Choochootracker/pull/22) | Keep the meters and numeric TRK labels |

The personal launcher cover also carries a diagonal red **GITHUB FORK** stamp.
This is a personal-only asset change, separate from the six upstream proposals.

The exact included commits and upstream base are in
[personal-features.json](personal-features.json). MIDI support is now included through upstream `main`, along with sample slicing,
key jazz, Settings submenus and the aChChid workflow improvements.

## Current device personal build

Upstream base: `a02a88098a03b10518806268f039b9d9b8b2f5a9` (October 2).
The device channel is **device personal build**: current main plus retained personal
features and experiments explicitly requested for testing. Upstream acceptance
is not required for a personal feature to remain on the handheld.

| PR | GitHub status | How it reaches the handheld |
| --- | --- | --- |
| #1 Stick live toggle | Merged | Upstream main, including ALWAYS ON |
| #17 Fonts/theme | Merged | Upstream main |
| #18 Piano | Closed as superseded by #23 | Upstream's integrated implementation |
| #19 Project spacing | Merged | Upstream main |
| #20 Track visuals | Open | Retained personal Detailed / Audio waveform setting |
| #21 Persistent waveform | Merged | Upstream feature with retained personal layout, default OFF |
| #22 Mixer meters | Closed as superseded by #23 | Upstream's integrated implementation |
| Track Insert FX | Alpha experiment; PR pending | Two track slots, five native effects, Fxx and MOD |
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

Graphics keeps current-main Waveform FPS and renderer information alongside
Load font, theme, Track visuals and Persistent waveform. MIDI, current rendering
fixes and Chord mode come from main. The installed `personal-build.json` records
the exact source commit, upstream base, binary hash, checks and rollback location.

## Branches

- `main` mirrors upstream `main`.
- `personal/r36h` is the combined source for the handheld.
- `contribution/*` keeps each upstream proposal separate. Updating the personal
  build does not alter those PRs or their descriptions.

The personal branch merges the feature branches, including their shared audio
foundation once. It retains that history; upstream acceptance is not required.
If upstream accepts a changed or squashed version of a feature, compare the two
implementations during the next update and retain one coherent implementation.

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

Resolve source conflicts individually, retaining both upstream fixes and the
personal choices above. Generated `web/dist` conflicts require a fresh combined
web build, not choosing one branch's bundle as the finished result. Follow
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
