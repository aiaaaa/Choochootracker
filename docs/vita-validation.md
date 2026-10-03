# Vita candidate validation — 2026-10-03

Native cross-compilation, package verification, host fixtures and the repeatable
update workflow passed. The personal candidate was transferred to VitaShell over
FTP and read back with a matching SHA256. **The first installation failed at 99%
with 0x8010113D and created no home-screen icon.** Its RGB LiveArea artwork was
outside the required indexed format; the initial verifier missed that restriction.
The packaging correction converts those assets and validates their encoding before
building or packaging. That corrected package installed, but crashed immediately
on launch with C2-12828-1. Core analysis identified missing ELF relocation records
and a crash in global-constructor startup before `main`. A linker correction and
packaging regression check follow below. The next device attempt reached `main`
but aborted after a tiny allocation failed; the newlib heap reservation fix is
described below. No successful runtime, emulator or Vita
HTTPS/audio/touch result is claimed here.

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
User installation and launch results for this heap-fix candidate remain pending.

## Initial rejected artifacts

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
