# Vita candidate validation — 2026-10-03

Current hardware status: the user successfully launched candidate
`91cd87f086eb-personal-a6fc019d.vpk` and opened Mountain King. The on-device
C99 scanner probe passed and `vita.log` records the project load succeeding.
The previous NESW correction was user-confirmed. Mountain King playback exceeds
the audio render budget, and Lucky still fails to parse the CA trust bundle.
These are unresolved device issues; the port is not yet validated for this
workload. Touch, save/reopen, Lucky acquisition/import, suspend/resume and
extended mixed-engine playback remain unvalidated.

Earlier installation/startup failures and their corrections are retained below
as history. Native builds, package verification and host fixtures passed, but
these are distinct from device testing. Vita remains local: no GitHub push.

## Runtime follow-up candidate

- Adopt the R36H physical NESW actions: Triangle PLAY, Circle EDIT, Cross OPT,
  Square SELECT; Start is a PLAY alias, L live, R record, physical Select erase.
  Migrate only untouched old Vita defaults; preserve custom maps, fonts and themes.
- Use integer scaling: 640×480 at 1× centered on 960×544, title artwork at 2×.
- All seven device `.cct` seeds were downloaded and compared byte-for-byte with
  the package. All seven load using the host project parser. Controls may explain
  failed selection, but actual Vita loading still needs confirmation; log results.
- A live host random-page request returned HTTP 200 with TLS validation. The old
  device log did not capture curl errors. The installed 182,140-byte CA bundle
  matches the prior package exactly. Add precise errors and worker-side logs;
  **the device Lucky connection failure has not yet been diagnosed or fixed**.
- Host OFF regression passed: 335 tests / 8,101,254 assertions, plus touch, seed
  copy and new control-migration adapters. Host ON passed 347 tests / 8,111,291
  assertions (two opt-in skips), including Vita-guarded fixture import and a fresh
  process reload. Both native profiles passed their 335-test SDK-container suite,
  all three adapters, ten workflow tests each, and package verification.
  No random music is used in automated tests.

Final application revision: `c1df82a3c4b0f88a51d1ebe2002eb0018cca651e`.
Local changes are commits `8a67385` (controls, scaling, diagnostics and tests) and
`c1df82a` (keep the fixed canvas across resize and renderer fallback). Both commands
completed successfully:

```sh
scripts/vita.sh build --profile ordinary
scripts/vita.sh build --profile personal
```

| Artifact under `releases/vita/candidates/` | Bytes | SHA256 |
| --- | ---: | --- |
| `c1df82a3c4b0-ordinary-089cc029.vpk` | 15,029,753 | `06dabe8164d32b5c837c9f4839aa06c390035887a08ab80b692dc5dd40a03d54` |
| `c1df82a3c4b0-personal-5373d90d.vpk` | 18,846,730 | `02e93f7585e75eaa357e5dbb1efe2913bd8ad724d1cfef1e768c9656c308ca1e` |

Each has `.sha256` and `.manifest.json` sidecars with profile, exact commits,
SDK/dependency identities and validation results. Personal includes Lucky;
ordinary excludes it. New NESW/graphics behavior and song loading require a
manual device check. Lucky networking is diagnostic-only in this update: do not
report it fixed until an on-device NEXT/PLAY/LOAD sequence succeeds.

The final personal VPK and checksum were uploaded to
`ux0:/data/choochootracker-candidates/c1df82a3c4b0-personal-5373d90d.vpk`.
The full FTP read-back matched SHA256
`02e93f7585e75eaa357e5dbb1efe2913bd8ad724d1cfef1e768c9656c308ca1e`.
Installation remains manual. The existing installed app, user data and previous
bootable installer were preserved. Current settings and autosave were backed up
locally under the sibling `vita-validation/` directory. No GitHub push occurred.

The previously booted personal VPK remains locally available as a rollback
candidate (not a fully validated release), SHA256
`1031cf024a8551f906bc71496cbb17bff587adc04269020e18c62bd5d623d050`.

## Certificate and project-parser follow-up

The user confirmed that candidate `c1df82a` boots and the NESW buttons align.
Its log reports a first DNS timeout (curl 28), followed by curl 77 while loading
`app0:/certs/ca-certificates.crt`. Mountain King was opened successfully but failed
with `Invalid project settings`. These are actual device failures, not host-only
results. The user also reports a persistent green 60; the device plugin list
contains PSVshell, while ChooChoo has no FPS overlay drawing code. System/plugin
configuration and clock settings have not been changed.

The SDK's installed `newlib.h` disables `_WANT_IO_C99_FORMATS`. Matching newlib
4.1.0 scanner source rejects the second `h` in `%hhu`; the project's required
track-volume header uses eight such conversions. Rather than edit many shared
parsers or alter saved-file syntax, build just newlib's string scanner with C99
support in an isolated dependency directory. Package checks require this object,
and the device startup probe validates conversion widths before project I/O.

Lucky now reads the packaged CA bytes in its worker and passes them through
curl's documented in-memory trust-bundle API. Maximum CA input is 256 KiB, with
missing/empty/oversized inputs covered; peer and hostname verification stay on.
This avoids the failing OpenSSL file-store path. Actual device HTTPS success
still requires retesting; no claim is made that host tests prove device TLS.

Host checks passed: OFF 336 tests / 8,101,255 assertions; ON 349 tests /
8,111,298 assertions (two opt-in skips), plus Vita adapters and fixture
prepare/reload. Eleven workflow tests passed. The isolated scanner object
cross-compiled against the pinned SDK. Both complete native builds and their
336-test container suites passed, as did package verification (including the C99
scanner linkage check). Actual device project loading and HTTPS retesting remain
pending; compilation and host tests do not establish those results.

Application revision: `91cd87f086ebb31d0458d71d5298a76ff8c74409` on local
`personal/vita`. Commands: `scripts/vita.sh build --profile ordinary` and
`scripts/vita.sh build --profile personal`.

| Artifact under `releases/vita/candidates/` | Bytes | SHA256 |
| --- | ---: | --- |
| `91cd87f086eb-ordinary-b3542cdf.vpk` | 15,041,462 | `42b6a91f5cf0c2402080929a0955e8ed697092f2a68766e3f3c24bdc4499e080` |
| `91cd87f086eb-personal-a6fc019d.vpk` | 18,872,095 | `d25d12c18aff00348327e514bc67a5c153770a7eb0428c6c2e72845462cc22a7` |

The personal installer and checksum were transferred to
`ux0:/data/choochootracker-candidates/91cd87f086eb-personal-a6fc019d.vpk`.
Full FTP read-back matched the SHA256 above. Installation remains manual; prior
installers, installed application and user data were preserved. No GitHub push.

The green FPS overlay can be hidden from LiveArea with physical Select+Down;
PSVshell's `src/gui.c` decrements the display mode down to hidden. This does not
change clock settings. No plugin/configuration files were modified.


## Hardware performance review of 91cd87f

The user reported about 120% CPU during Mountain King. ChooChoo's own mixer CPU
meter measures smoothed audio-render time divided by the buffer deadline; it is
not total usage across all Vita cores. Retrieved log evidence:

- Output: 48,000 Hz, stereo PCM16, 1,024 frames; deadline 21.333 ms.
- After loading `projects/grieg-mountain-king-fm.cct`, maximum callback times
  reached 28.034–28.660 ms. Two consecutive reporting intervals had every callback
  over budget: 185/185, then 183/183. This is sustained overload, not only a spike.
- 606 of 3,842 total-session callbacks exceeded budget. That whole-session ratio
  includes idle/stopped time and must not be presented as the song's overload rate.
- Heap headroom stayed near 159.5 million bytes after loading; no out-of-memory
  event appeared. Native free memory was roughly 32–34 MiB outside the reserved heap.
- The actual compiler defaults and ELF attributes confirm ARMv7-A, NEON/VFPv3
  and hard-float ABI. Software float is not the cause. Current application flags
  use `-Os -flto`; performance-oriented compiler settings remain an unmeasured
  next experiment. No clock, quality, engine or project changes were made.

This establishes a real performance limit for this workload/current build, not a
proof that the hardware can never run it. A useful next comparison is an optimized
build at the same clocks and audio quality, followed by profiling the active
Plaits FM voices, filters and mixer path. A larger buffer alone would not cure
sustained rendering slower than real time. Audible symptoms were not described
in this report; render-budget counters are not direct hardware underrun counts.

Lucky reaches the in-memory trust loader but fails with curl 77:
`error adding trust anchors from certificate blob: 77`. Reading the CA file into
memory therefore did not solve the TLS backend problem. It occurs before HTTP
success/module acquisition and independently of song playback. Next diagnosis
needs OpenSSL's underlying PEM/X509 error, not weaker TLS verification or an
assumption that the Vita is too old. No live module preview/import has succeeded.

Raw log and derived interval metrics are saved outside Git in the sibling
`vita-validation/91cd87f-device-runtime.log` and
`vita-validation/91cd87f-performance-summary.json`. The current package and all
user files remain unchanged; no replacement build or GitHub push was made during
this read-only runtime diagnosis.

## Source and isolation

- Worktree: `/Users/hifi/workspace/r36h/choochootracker/vita`, branch `personal/vita`.
- Combined personal baseline: `c0a8c7e9ee7e177e94294ede7e6cad09c59f06c8`.
- Manifest's established upstream base: `a02a88098a03b10518806268f039b9d9b8b2f5a9`.
- At initial fetch, upstream/main was `3da2646`; main/origin/main were `0177aa9`.
  Upstream-to-personal integration remains the separate PERSONAL_FORK.md workflow.
- Adaptation commit: `22a6e1857318add2192886d19336f9fe605bb8a5`.
- **Built application revision:** `b04a45a0c361bbe4dc5f066d7501d0ff7399323c`.
  Subsequent documentation, CI isolation and verifier-test changes do not alter
  this recorded artifact identity.
- Older dirty `appearance-dev` and `mod-lucky` worktrees were preserved, including
  their 50 and 18 existing changed files. No unrelated work was staged or reset.
- No branch was pushed, no release/PR opened, and no web/device app automatically
  deployed or installed. The user performs the first installation in VitaShell.

Changes comprise the native Makefile, `platforms/vita` adapter and package assets;
small SDL/shared entry, filesystem, audio and lifecycle hooks; appended touch MOD
types/evaluation/picker; existing Lucky CA/path/headroom hooks; pinned build,
package/update/verification scripts; tests, CI, personal manifest and documentation.
The shared web bundle was regenerated as required, without deployment.

Full file list: `git diff --name-only c0a8c7e b04a45a`.

## Tests and commands actually run

The existing Makefile.test workflow was run before shared behavior changes:

| Configuration | Baseline | After adaptation |
| --- | --- | --- |
| Lucky OFF | 328 tests; 8,101,169 assertions | 334 tests; 8,101,239 assertions |
| Lucky ON | 340 tests; 8,111,206 assertions | 346 tests; 8,111,276 assertions |

All passed. The enabled suite has two existing opt-in tests skipped. Authored
MOD/XM/S3M/IT fixtures exercise the existing Lucky path; tests download no music.

Commands (from this worktree):

```sh
make -C tracker -f Makefile.test -j4
make -C tracker -f Makefile.test -j4 BUILD_DIR=build/tests-lucky \
  CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1 \
  MOD_LUCKY_PREFIX=../.tmp/host-mod-lucky/prefix
python3 scripts/test_vita_workflow.py
scripts/vita.sh doctor
scripts/vita.sh build --profile ordinary
scripts/vita.sh build --profile personal
scripts/vita.sh build --profile personal # unchanged source, repeated successfully
scripts/vita.sh update --source-ref test/vita-source-update --profile personal
```

The native wrapper also ran the OFF suite inside the pinned Linux SDK container:
334 tests / 8,101,239 assertions passed for both profiles and the update candidate.
The host-enabled suite used a separate macOS libxmp build, never the Vita library.

Additional checks passed:

- Actual Vita SDL adapter compiled against a small event mock: independent panels,
  overlapping numeric IDs, stable primary contact, ownership, orientation/clamping,
  release/re-arming, focus/suspend reset, synthetic mouse filtering and controller
  pass-through. Engine tests cover inactive neutrality, routing, picker and save/load.
- Separate processes ran the Vita-guarded Lucky backend/path fixture: preview A,
  replace with B, no autoplay, play cached B, import four samples from B without
  network calls, save and reload playable samples in a fresh process. A forced
  low-memory failure left the project unchanged. This is host execution of the
  guarded paths, not a Vita filesystem, audio or TLS simulation.
- Seven Python workflow tests: new source/repeated merge, conflicts, failed build,
  dirty-tree refusal, source-ID collision, package corruption, and package inventory,
  profile, exact identity and asset hashes. Temporary repositories and fixtures only.
- Real packages passed CRC, title ID, required assets, SELF signature, exact build
  identity, manifest inventory/hash and checksum-sidecar checks. Wrong-profile and
  wrong-checksum checks failed as expected. An altered icon with a recomputed outer
  checksum still failed the manifest asset-hash check.
- Ordinary link map excludes libxmp/curl and executable excludes Lucky symbols;
  personal includes them. Ordinary CI no longer acquires the ON test dependencies.
- Existing Emscripten SDK at `/private/tmp/choochoo-pr-emsdk` rebuilt the checked-in
  bundle using `Makefile.web web-deploy` (local bundle target). No web publication.
- Source `git diff --check` passed. Existing generated JS contains generator-style
  trailing whitespace; no manual generated-JS formatting was introduced.

Local logs are in sibling `../vita-validation/` and each build's `.tmp/vita/`
directory. GitHub Actions is configured but has not been run or published here.

## Current startup-fix candidates

Revision: `d3aa310bfbc09c49c31161024bc7244d7885c4c8`. Both canonical profiles
passed cross-compilation, the 334-test OFF suite, the Vita input adapter, nine
workflow tests and package verification. Host Lucky tests remain the previously
passing run: this fix changes target linking and validation, not the engine.

| Artifact | Bytes | SHA256 |
| --- | ---: | --- |
| `releases/vita/candidates/d3aa310bfbc0-personal-e86dbb82.vpk` | 18,828,838 | `c3ee3f5f659bc594600c5a36eeb99743bf7633123cebf3de93818305d21f6a87` |
| `releases/vita/candidates/d3aa310bfbc0-ordinary-50868df6.vpk` | 15,254,103 | `a34f356905209d85fb96eff779bfc2dc9c0f59dc9fed3b7879699db432d2ed55` |

Personal ELF: 110,032 code relocations and 11 constructor relocations.
Ordinary ELF: 38,598 code relocations and 9 constructor relocations.
These counts are recorded in each package manifest; current verification rejects
the earlier packages that lack checked relocation metadata. Both retain the
corrected indexed LiveArea images. The personal VPK was uploaded and read back
over FTP with matching SHA256; the superseded device-side candidate was removed.
A fresh hardware launch result is pending.

## Indexed-PNG candidates — installed but crashed

Packaging fix revision: `ab2686b5c1dd325f6efb79a4ed8d62e24902ee0e`.
All three LiveArea images are now opaque, non-interlaced, 8-bit indexed PNGs;
decoded RGB pixel comparison against the original images is exactly equal.
Doctor, build, packaging and verification check dimensions, encoding, palette,
PNG chunk CRCs, bounded decoded rows and the referenced XML assets. The exact
original VPK is rejected by these new checks. Eight workflow tests pass, including
RGB, RGBA, 16-bit, interlaced, wrong-dimension and truncated-PNG rejection cases.

Both canonical builds were run again at this exact revision and passed native
cross-compilation, the 334-test OFF suite, the Vita input adapter test and package
verification. No runtime code changed in this correction.

| Corrected artifact | Bytes | SHA256 |
| --- | ---: | --- |
| `releases/vita/candidates/ab2686b5c1dd-personal-84ca9aad.vpk` | 17,103,614 | `68d977ed57f448b4e4901d26b3a4f0c8175391c7bb4ab4f156f757a96089ffa7` |
| `releases/vita/candidates/ab2686b5c1dd-ordinary-d6e93e38.vpk` | 14,366,995 | `4673f97e2cbf5b37c9824c760331eac9e4503a07271d8ed1bea3dc8cbd62e9ee` |

The indexed-PNG personal candidate was uploaded to the same Vita candidate folder
and read back over FTP; SHA256 matched exactly. Installation succeeded and launch
failed. It is superseded; do not reinstall it for runtime testing.

### Launch crash diagnosis

Dump `psp2core-1790907681-0x0000662ced-eboot.bin.psp2dmp` was retrieved from the
Vita. No ChooChoo data folder/log had been created. The main thread stopped with
prefetch abort 0x30003, PC `0xffb41000`, LR `0x813658bb`; its code segment loaded
at `0x8106a000`. Mapping LR to the exact ab2686b ELF gives `0x812fb8bb`, inside
`__libc_init_array` immediately after its indirect constructor call. Register R5
still held the unrelocated init-array address `0x81410004`, while the data segment
was actually loaded at `0x81500000`.

The linked ELF reported **no relocations**. The native Makefile had omitted the
required VitaSDK `-Wl,-q` setting. The fix retains relocations and disables copy
relocations (`-Wl,-q,-z,nocopyreloc`), matching the pinned SDK toolchain. Packaging
now requires nonempty `.rel.text` and `.rel.init_array` records and records their
counts in the manifest. Host tests reject the missing/partial-relocation reports.
This diagnosis concerns port startup, not project corruption or the Lucky decoder.

## Second launch failure: heap reservation

The installed d3aa310 executable was downloaded and its SHA256 matched the new
VPK's `eboot.bin`: `58ce6d9e3ecc8a7efbd1287993489f5ab84a405b796b878285a4242f68c097c0`.
Core `psp2core-1790909730-0x0000cc2003-eboot.bin.psp2dmp` loads code at
`0x81058000`. PC `0x8135e62e` resolves to `_kill_r`, the intentional SIGABRT trap.
The stack resolves through `abort`, failed C++ exception allocation, `operator
new`, `filesystem::path`, `vitaPlatformInit` and `main`. This confirms progress
past the constructor relocation failure; it is not another missing-relocation
crash. The dump's memory-block records contain no newlib heap block.

The pinned SDK allocator reserves a single fixed block before `main`; if the
request fails, `_sbrk_r` returns ENOMEM even for tiny allocations. The port had
requested 256 MiB. The correction requests 192 MiB, derives the Lucky headroom
budget from that same constant minus the existing 8 MiB reserve, and retains
all engines and the 96/64 MiB preparation/import thresholds. No SFO extended
memory privilege or clock override is introduced. Peak usage remains unmeasured.

Before any allocating filesystem code, native I/O now records the heap break,
32-byte malloc probe, and free memory in `startup-memory.log`. If the heap probe
fails, startup returns cleanly. `vita.log` also opens before seed copying to
capture filesystem setup errors. The revised request still needs a real-device
launch test; compilation alone cannot establish that the memory budget fits.

Both profiles built from exact revision
`e146dee64d578c4364feb354baa04a6901053848` using `scripts/vita.sh build --profile
personal` and `--profile ordinary`. Both passed the 334-test/8,101,239-assertion
host suite, SDL touch adapter checks and package verification. The 9 workflow
tests passed. Relocation counts remain nonzero: personal 110,069 text/11 init-array,
ordinary 38,632 text/9 init-array. The ordinary ELF's heap variable was also
inspected and contains exactly 201,326,592 bytes (192 MiB).

| Heap-fix candidate | Bytes | SHA256 |
| --- | ---: | --- |
| `releases/vita/candidates/e146dee64d57-personal-0f5269b7.vpk` | 18,831,692 | `2d6c03dbffbcba37be0b6e3de066906fd04f69b822f5b0a20701cc9a207d6806` |
| `releases/vita/candidates/e146dee64d57-ordinary-d1393048.vpk` | 15,256,417 | `fcea560b8370394361c34df1d42240f96362e8cf7d66f232b6c7a69b714a0472` |

The personal package retains Lucky, all existing engines/Insert FX, and both touch
MOD sources. The latest changes affect only the Vita adapter and documentation;
shared engine and the existing Lucky implementation are unchanged.
The personal VPK and checksum were uploaded to
`ux0:/data/choochootracker-candidates/`; read-back SHA256 matched the local package.
The user installed and launched this candidate: it returned to the OS after
about two seconds, without the previous crash dialog. Retrieved logs confirm
`malloc_32_ok=1`, a valid heap break, 192 MiB reserved, 51 MiB system memory free
and 112 MiB CDRAM free at startup. These are startup measurements, not peak-load
or audio-performance results.

## First-run asset copy failure

The e146dee `vita.log` reports `filesystem error: cannot copy file: File exists`
for `app0:/assets/licenses/INSERT_FX.txt`. Its destination was created as a
zero-byte file. Startup catches that exception and returns before graphics
initialization, explaining the clean exit. Successful installation and heap
initialization still do not establish a working tracker UI.

The Vita seed adapter now uses bounded basic read/write calls and an exclusive
destination open instead of `std::filesystem::copy_file`. Existing regular files
(including empty user files) are preserved; write/close errors remove only the
destination newly created by that call. Other filesystem errors remain visible.
The host seed test covers binary data spanning multiple buffers, repeat copying,
existing nonempty/empty files, empty sources, missing sources, directory conflicts,
and a forced partial write failure with cleanup. The test is part of Makefile.test
and both canonical Vita profile builds. It does not claim power-loss recovery for
a copy interrupted by process termination.

Both canonical profiles built from
`954af1ca2527954e70e787e3d4527374637dba13` and passed package verification,
the 334-test/8,101,239-assertion suite, touch adapter checks, seed-copy checks and
9 workflow tests. The separate Lucky-enabled host command also passed 346 tests,
8,111,276 assertions (2 existing opt-in skips), touch/seed checks and Vita-guarded
prepare/import/reload fixtures. No random music was downloaded by these tests.

| Asset-copy fix candidate | Bytes | SHA256 |
| --- | ---: | --- |
| `releases/vita/candidates/954af1ca2527-personal-31d067ef.vpk` | 18,830,123 | `67711b3556a103b4b3402cbdc327a5fd39908d7a51659ec70ada17673465db46` |
| `releases/vita/candidates/954af1ca2527-ordinary-06aa252d.vpk` | 15,010,603 | `0c66e5730d58c121bbfc3b96e46b86db17feb8b726160a2fea4b0474d326f955` |

The device's empty `licenses/INSERT_FX.txt` was downloaded, checked as zero bytes,
and preserved under `licenses/INSERT_FX.failed-copy-e146dee.txt`, allowing the
new seed copy to populate its original path. No user songs/settings were changed.
Launch and all tracker runtime validation for this candidate remain pending.
The personal installer and checksum are in `ux0:/data/choochootracker-candidates/`;
the FTP read-back hash matches the local VPK. The superseded e146dee installer
and sidecar were removed from that candidate folder, retaining local artifacts.

## Completed unpacking, then C++ thread startup failure

On the first 954af1c launch, the user returned to VitaShell in under 30 seconds.
A size audit found 530 of 666 bundled files complete, 135 missing, and one
zero-byte interrupted wavetable (`SR_wavetables/WaveEdit/MICROBRU.WAV`). The
black screen was during first-run copying, before the render loop.

After allowing setup to finish, `vita.log` reported `Vita assets ready`, successful
network subsystem initialization (not an HTTPS test), then an uncaught
`std::system_error`: `Enable multithreading to use std::thread: Not owner`.
Core `psp2core-1790913577-0x0001c42ae3-eboot.bin.psp2dmp` loads application code at
`0x8102c000`; its stack maps to `std::thread::_M_start_thread`, Lucky's Service
constructor and audio startup. SDL timer and GXM display threads exist in the dump.

The exact ELF defines pthread_create/once but leaves pthread_cancel weak and
unresolved. The pinned libstdc++ uses that symbol to determine whether threading
is active. Vita GCC's `-pthread` driver specs retain the full pthread archive;
the port's manual `-lpthread` did not. The correction uses `-pthread` for compilation
and linking in both profiles. Packaging now checks and records strong definitions
of cancel in both profiles and create/once in personal, and verification rejects
missing activation metadata. Ordinary legitimately discards unused create/once;
an initially over-strict packaging check was corrected after its build caught this.
The regression test reproduces the weak-symbol case; it does not emulate a Vita.

The requested **Unpacking...** text and a file counter now draw through the normal
renderer after settings/font initialization and before asset copying or project/
audio setup. It handles quit/background events between copies. Other platform
startup paths are unchanged by the Vita-only entry hook.

After the crash, the interrupted zero-byte MICROBRU file was downloaded and
preserved as `MICROBRU.WAV.interrupted-954af1c`; next startup can seed its missing
original without overwriting user assets. Threaded runtime and visible unpacking
still require a new device test.

Personal built from `02960371386fc8d6d3559fdb53573ef3ae282190`, passed the
334-test/8,101,239-assertion host suite plus touch/seed fixtures, and package
verification confirms strong cancel/create/once definitions. The 10 workflow
tests pass, including unresolved/weak activation and profile-specific checks.
Artifact: `releases/vita/candidates/02960371386f-personal-a007771b.vpk`,
18,837,206 bytes, SHA256
`1031cf024a8551f906bc71496cbb17bff587adc04269020e18c62bd5d623d050`.
Ordinary cross-compilation and tests passed at that revision, but its package was
withheld by the over-strict symbol check; rebuilding after the check correction
is required before reporting an ordinary package success. The correction changes
packaging/tests/docs only; the application source is unchanged.

The corrected ordinary build from `0bf5dad40de51f7073528b69ead6a93da41ea16d`
then passed cross-compilation, the same host/touch/seed tests, 10 workflow tests,
and package verification (Lucky symbols/dependencies excluded). Artifact:
`releases/vita/candidates/0bf5dad40de5-ordinary-a136931e.vpk`, 15,022,932 bytes,
SHA256 `57caec53078369b3a11b481f545cd04ffc889240bad61f77893815cb42b4913a`.
Its activation proxy is defined; unused POSIX create/once are omitted as expected.

The successful personal 0296037 installer and checksum were uploaded to
`ux0:/data/choochootracker-candidates/`. Read-back SHA256 matched, and the previous
954af1c installer was removed from that folder under the existing authorization.
The current verifier also accepts this personal package; no application-code
change was needed for the ordinary verifier correction. The user was invited to
install/launch it. Visible unpacking, successful tracker startup and audio/Lucky
runtime remain pending user/device confirmation; none is inferred from these builds.

## Initial rejected artifacts (historical)

The initial artifacts listed below are retained as evidence, but **do not install
them**: they contain the rejected RGB LiveArea assets. A corrected candidate must
pass the new indexed-PNG checks and a fresh device installation attempt.

Paths are relative to this worktree unless noted. Each has `.sha256` and
`.manifest.json` sidecars. The title ID is **CCTRK0001** for both profiles.

| Artifact | Bytes | SHA256 |
| --- | ---: | --- |
| `releases/vita/candidates/b04a45a0c361-personal-94801a94.vpk` | 17,105,630 | `2235a690615f031bc2c7604403adc466c50312579ec5c29bcea2cad9d3106f46` |
| `releases/vita/candidates/b04a45a0c361-ordinary-17146ca4.vpk` | 14,369,010 | `d3cbfc8f63c503d98203e257722e023baa3042dcde2dec8ef4d2d733ef3c3d2f` |
| `releases/vita/candidates/b04a45a0c361-personal-d7e6bf7c.vpk` (repeat) | 17,105,630 | `5d0ee3c456045192b7b333e9ac474e716f00b977d6af63da5e1ce7eb346c4b77` |

The repeat has identical hashes for every packaged executable/asset file. Its
timestamped build manifest changes the final VPK hash; bit-identical ZIP output
was not promised. The first artifacts remain intact.

The personal profile sets `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1`, includes all existing
engines/Insert FX and adds Front/Rear touch MOD sources. Ordinary sets the flag 0.
SDL2 2.32.8 and GCC 15.2.0 come from the immutable SDK digest in vita-sdk.json;
libxmp-lite 4.7.3 and curl 8.17.0 are checksum-pinned. Notices and the hashed CA
bundle are packaged. SDK curl's OpenSSL ABI mismatch required rebuilding curl;
TLS verification remains enabled. No engine, quality or clock reduction was made.

## Update and failure demonstrations

Controlled local source `7103d67f73668ce239c933ccef04cd25717200be` adds only
`docs/vita-update-probe.md` on `test/vita-source-update`, based on c0a8c7e.
It is a workflow fixture, **not a personal source revision to promote**.

The real update command fetched origin, resolved that source, created
`candidate/vita/20261003-110732-4cbcc9` in sibling
`vita-update-20261003-110732-4cbcc9`, merged and recorded source identity, and built:

- Candidate revision: `b6260a71bd9f52e021c6348008cd062d4ed8ab0f`.
- VPK in that worktree: `releases/vita/candidates/b6260a71bd9f-personal-c2cd762c.vpk`.
- SHA256: `349e2cc09a25cae43a3ac37d34fc1ea3ff1e56662f77e4ccc890d1f6d2582d56`.
- Original personal/r36h and personal/vita branches remained unchanged by update.
- `.tmp/vita-update.json` records previous Vita revision, previous source base,
  selected ref and exact commit, worktree, branch and resulting revision.

Automated disposable-repository demonstrations created a real merge conflict and
an intentionally failing candidate build; both preserved the original branch,
clean original checkout and byte-identical last-known-good fixture artifact.
No existing real package was overwritten to test failures. Early port build
failures (SDK curl ABI and missing Linux test includes) also emitted no candidate;
they were fixed before the successful builds above.

## Device handoff and remaining validation

The first personal VPK and checksum were uploaded via VitaShell FTP to:

`ux0:/data/choochootracker-candidates/b04a45a0c361-personal-94801a94.vpk`

Read-back SHA256 matched the local VPK exactly. The subsequent device installation
failed at 99% with 0x8010113D; no application icon appeared. Runtime validation uses the checklist in
[vita.md](vita.md#integration-points-and-hardware-checklist): launch/navigation,
audio format and sustained playback, both sticks/panels and physical rear
orientation, MOD routing/Insert FX, save/reopen, Lucky HTTPS/preview/import/reload,
offline/cancellation, suspend/resume and subsequent installation preserving data.

No Vita emulator check was available. MIDI is deliberately unavailable. Touch
assignments are preserved on updated non-Vita builds with zero unavailable input;
older unmodified upstream readers are not promised to preserve the new IDs.
Existing sample-path relocation and Lucky loop/tuning limitations still apply.

Writable application data belongs under `ux0:data/choochootracker/`; imported
banks belong under its configured sample root's `mod_lucky/`. Back up that whole
data directory before subsequent candidates. A VPK rollback alone does not roll
back project/settings data. No artifact has been designated hardware-known-good.

## Next real source update

First reconcile upstream into personal/r36h using PERSONAL_FORK.md. Then, from a
clean personal/vita checkout with Docker running:

```sh
scripts/vita.sh doctor
scripts/vita.sh update --source-ref origin/personal/r36h --profile personal
# In the printed candidate worktree:
scripts/vita.sh verify --artifact releases/vita/candidates/<printed-name>.vpk --profile personal
```

Use a local combined-source ref instead if intended changes are newer than origin.
Resolve any conflict only in the disposable candidate, review, commit and rebuild
there. Hardware-test the printed candidate before deliberate branch/LKG promotion.

## Source harmonization after PR #39 (2026-10-06)

The user requested synchronization of the active forks, including Vita. This
integration consumes personal/r36h `5c5c10a461090a5b1af1b10af4e0ef166f82187f`,
based on upstream main `6e9eb2b79f7024027e8ff1308d131aa05c0096ed`.
The previous Vita revision is retained at backup/vita-pre-pr39-20261006.
Native chip instruments, simplified FX, waveform previews and upstream build
fixes are adopted while preserving the Vita controls, touch modulation, parser,
threading, asset-copy and TLS adaptations. The update was merged in an isolated
candidate worktree. All 11 Vita workflow tests pass.

This is requested source-branch synchronization, not last-known-good package
promotion. Installed hardware and user data remain unchanged. Existing device
performance and TLS limitations remain open until separately retested.
