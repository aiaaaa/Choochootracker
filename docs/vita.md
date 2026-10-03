# Vita device personal build

This is a native VitaSDK/SDL2 candidate port, not a PortMaster package. Title ID
`CCTRK0001` stays fixed across updates. No proprietary PVR files, graphics plugin,
clock override, reduced engine list or altered audio quality is required by the build.
Compilation is not hardware validation; use the checklist below before promotion.
The native link retains relocation records (`-Wl,-q,-z,nocopyreloc`), matching
VitaSDK's toolchain rules. Packaging checks code and constructor relocations;
omitting them can produce an installable executable that crashes before `main`.

## Branches and updates

`main` is intended as the upstream mirror. Follow **PERSONAL_FORK.md** to reconcile
upstream into `personal/r36h` first, including features upstream merged differently.
Vita consumes that combined source; it does not cherry-pick each personal feature.
`personal/vita` contains the small downstream adaptation. `personal-features.json`
records its personal source commit and title identity alongside existing features.

Initial audit: clean personal source `c0a8c7e9ee7e177e94294ede7e6cad09c59f06c8`.
At audit, upstream was `3da2646` and `main`/`origin/main` still `0177aa9`:
the mirror was behind, and Vita did not silently advance the personal integration.
Dirty older `appearance-dev` (50 files) and `mod-lucky` (18 files) worktrees were
preserved. Their in-progress files were not copied over the combined source.

```sh
# Once: install/start Docker Linux containers and pull the immutable SDK.
docker pull vitasdk/vitasdk@sha256:2e92c60b69cb06d3f20814f6c882e943d8efd80fc96e643e043803bc702fc4ac
scripts/vita.sh doctor
scripts/vita.sh build --profile personal
scripts/vita.sh verify --artifact releases/vita/candidates/<printed-name>.vpk --profile personal

# Next revision, AFTER the established upstream -> personal/r36h integration:
scripts/vita.sh update --source-ref origin/personal/r36h --profile personal
```

`doctor` inspects without pulling, merging or writing project files. `build` requires
a clean committed checkout, builds its exact HEAD, and never fetches application
source. First personal build downloads checksum-pinned libxmp and curl source.
`ordinary` is the default profile and does not build/link Lucky, curl or libxmp.
`personal` always sets `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1`; dependency failure fails
the build rather than silently dropping Lucky.

`update` fetches origin, resolves the requested source to an exact commit, records
the previous Vita/source revisions, and merges in a fresh sibling worktree on
`candidate/vita/<unique-id>`. It leaves original branches intact. Conflicts remain
in that candidate for review with the conflicting paths printed. No automatic
ours/theirs, promotion, push, release or installation occurs. The candidate's
`.tmp/vita-update.json` records the operation; its committed personal manifest
records the new exact source and previous Vita revision. To follow local work,
commit only intended changes in its own checkout and pass that local ref.

Objects and dependencies are isolated under `.tmp/vita/<configuration>/<revision>`;
the configuration key includes SDK pin, dependency recipes and profile. Each successful build writes
a new candidate VPK, `.sha256`, and `.manifest.json` into `releases/vita/candidates/`.
Repeated builds reuse compatible objects and do not merge features or copy assets
into user data. A lock prevents simultaneous builds sharing an output directory.
Failed runs retain their log and leave existing candidate/LKG files untouched.

After hardware validation, deliberately fast-forward `personal/vita` to the tested
candidate (if still possible; otherwise review/merge and rebuild). Copy its VPK,
checksum, manifest and completed checklist to your chosen last-known-good archive.
No script overwrites or selects a last-known-good artifact for you.

## Pinned environment and validation commands

`scripts/vita-sdk.json` pins the official `2026.08-20260912` SDK image by digest,
using `linux/amd64` on local and CI hosts. This includes GCC 15.2.0, SDL2 2.32.8,
OpenSSL and zlib. An ARM Mac needs Docker's amd64 emulation; no native Mac SDK or
replacement desktop/PortMaster compiler is installed. No moving SDK `latest` tag.
The manifest includes the complete installed target package inventory.

Lucky keeps libxmp-lite **4.7.3**, archive SHA256
`b6a98797e4fb9c9a705f5d53112aa5214561857e929a644928b9e658930d9440`.
Curl **8.17.0**, archive SHA256
`e8e74cdeefe5fb78b3ae6e90cd542babf788fa9480029cfcee6fd9ced42b7910`, is rebuilt
against the SDK's actual OpenSSL. The image's prebuilt curl has an OpenSSL ABI
mismatch. The focused recipe follows the SDK's Vita configuration, with threaded
DNS and HTTP(S) only; OpenSSL’s unavailable console password UI is disabled. TLS peer and hostname verification remain ON, using the
image's pinned Mozilla-derived CA bundle packaged at `app0:/certs/` and hashed in
the manifest. Correct device time and on-device certificate/network testing are
still required. Updating trust roots requires a deliberate pin/build review.

The same wrapper runs ordinary host regression tests inside the SDK container,
cross-compiles, packages with VitaSDK tools, and verifies content hashes, profile,
title ID and required assets. Host Lucky tests use host libraries in a separate
directory, never Vita libraries:

```sh
MOD_LUCKY_DEPS_DIR="$PWD/.tmp/vita-host-xmp" scripts/build-mod-lucky-dependency.sh
make -C tracker -f Makefile.test -j4 BUILD_DIR=build/vita-host-off
make -C tracker -f Makefile.test -j4 BUILD_DIR=build/vita-host-on \
  CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1 MOD_LUCKY_PREFIX="$PWD/.tmp/vita-host-xmp/prefix"
python3 scripts/test_vita_workflow.py
```

Host-enabled tests require the host's curl development library and C++17 compiler.
They use authored MOD/XM/S3M/IT fixtures; no random music is downloaded by tests.
The enabled suite also runs a separate process through the Vita-guarded Lucky
backend and bank paths, then a fresh process reload. This checks the mount-path
logic on a host filesystem; it does not emulate Vita newlib, device TLS or audio.
CI runs these tests and the same wrapper, uploading candidate artifacts only.
Dependency updates mean editing reviewed immutable pins, rebuilding both profiles,
checking notices and SDL panel IDs, rerunning tests, then hardware validation.

## Controls and touch MOD sources

Controller navigation remains primary. Defaults are remappable in existing input
settings: D-pad navigation, Cross EDIT, Circle OPT, Start transport, Select SHIFT,
L live/motion arm, Square record, Triangle erase. R is available for remapping.
Both analog sticks retain their independent existing axes. No essential action
requires L2/R2 or stick clicks; rear touch is not a substitute button bank.

In an instrument's existing **MOD → Source → TOUCH**, choose **FRONT** or **REAR**.
The existing Axis parameter selects **X**, **Y**, or **Gate**; destination and amount
work through normal synth and Track Insert FX routing. Source IDs 8 and 9 are
appended; IDs 0–7 retain their meanings. Update/build checks reject upstream ID
collisions. This does not create another song format. These assignments survive
save/load on updated builds without hardware and evaluate to zero. Unmodified
older versions' handling of unknown source IDs is not guaranteed.

X follows left-to-right and Y bottom-to-top using SDL's panel active area, not
screen pixels. Rear coordinates use SDL's device orientation while holding the
screen toward you; physical rear orientation remains a hardware checklist item.
X/Y are bipolar around center with the same amount scaling as linear sticks;
Gate is unipolar 0/1. An inactive axis contributes exactly zero, not its low endpoint.
Touch respects existing Free/Toggle/Hold live modes. Ownership is decided at down:
an unarmed contact cannot become modulation halfway through its gesture. One
primary contact per panel; secondary contacts never take over. After primary lift,
disarming, reset, focus loss, suspend or project replacement, lift **all** contacts
on that panel before a fresh gesture. Front/rear numeric contact IDs are independent.
Every Vita touch is consumed before the UI/renderer path, and synthetic mouse events
are disabled/filtered; touches cannot edit cells or press virtual buttons. Existing
stick recording/erase remains available; R1 does not add touch automation recording.

## Data, audio and Lucky

Read-only package assets live in `app0:/assets`. First launch copies missing seeds
into `ux0:data/choochootracker/`, without overwriting any existing file. Settings,
autosave, projects, samples and fonts use that stable writable root. Imported banks
go into the configured sample directory's `mod_lucky/` subdirectory under the data
root, never a disposable preview cache. Back up the whole data root before installing
any candidate. Reinstalling an older VPK alone does not roll back songs/settings;
restore the matching backup if needed. Absolute sample paths from another machine
must be relocated using the existing sample loader; they are not automatically portable.

Audio uses existing SDL output and engine, requesting 48 kHz stereo PCM16 and a
1024-frame initial buffer (Vita requires multiples of 64). The obtained format and
buffer are logged to `vita.log`; a rate/channel/format mismatch fails audio setup.
No audio callback download, decode, filesystem work or blocking lock is added.
Touch uses lock-free snapshots. Suspend/focus loss stops transport, clears controls,
invalidates interrupted Lucky work, and pauses output. Resume never starts playback.
MIDI is explicitly unavailable: project MIDI data is retained, with no ALSA,
CoreMIDI or WinMM linked. Hardware MIDI support is not claimed.

Lucky remains the existing single **I'm Feeling Lucky — NEXT / PLAY / LOAD** row.
Animated NEXT prepares; ready PLAY waits for a fresh press; LOAD uses that exact
cached candidate. Existing capacity checks, cancellation, bank staging, attribution,
loop fallbacks and tuning rules apply (see [mod-lucky.md](mod-lucky.md)). Limits stay
8 MiB module, 16 MiB decoded samples, 256 KiB HTML and bounded preview buffers.
The Vita heap requests 192 MiB as one fixed newlib block. The previous 256 MiB
reservation failed on hardware before the first filesystem allocation; this
smaller request leaves space for native libraries and graphics. Preparation requires 96 MiB
allocator headroom and import 64 MiB, including an 8 MiB reserve. Main/worker
stacks are 2 MiB and SDL threads 1 MiB. The UI logs free memory, callback counts,
maximum render duration and over-budget callbacks every five seconds; these
are diagnostics, not proof that no hardware underruns occurred. Combined
engines/project plus bank staging still must be measured on hardware. Early
`startup-memory.log` uses native I/O without heap allocation and records the heap
probe and free memory in hexadecimal bytes. A failed probe exits before filesystem
setup. `vita.log` starts before asset copying. Allocation failure remains a failed import,
not permission to discard a project or omit engines.

## Integration points and hardware checklist

Review conflicts in `Makefile.common` source discovery, SDL mainloop/gfx/audio/input,
shared entry/file paths, app lifecycle/project replacement, MOD enum/evaluation/UI,
and Lucky's CA/path hooks. Compare actual upstream implementations before retaining
a hook. SDL upgrades require checking Front=1/Back=2, panel normalization, native
GXM renderer and suspend notifications. Re-run both profile builds after changes.

- Launch, letterboxing/font/themes, controller navigation, save and reopen.
- Both sticks; both panels simultaneously, corners/orientation, gate, secondary
  fingers, lift/re-arm and Free/Toggle/Hold modes without cursor edits.
- Route touch to synth and Insert FX while notes sustain and songs play.
- Exercise all current engines and representative heavy Insert FX chains.
- Lucky real HTTPS NEXT, separate PLAY, exact-bank LOAD, restart/reload; offline,
  timeout and leaving Settings during acquisition/import.
- Suspend/resume and PS-button backgrounding during song, preview and download;
  no stuck inputs, late playback or import.
- Extended playback: record `vita.log` format, memory high-water/remaining memory,
  callback duration and audible underruns; tune only after measurement.
- Install a subsequent candidate without deleting `ux0:data/choochootracker`,
  confirm existing projects/banks, then test backup/rollback.

No Vita hardware or emulator validation is implied by this document. See the
[validation report](vita-validation.md) for commands actually run and current results.
