# Native chip instruments — local implementation report

This extends `feature/native-chip-instruments`, based on personal/r36h
`c0a8c7e9ee7e177e94294ede7e6cad09c59f06c8`. The existing OPLL checkpoint
`1118786` is preserved. The DX7 addendum supersedes only the earlier DX7 exclusion;
full Dexed, JUCE and unrelated engines remain excluded. The user has now authorized a branch on their fork and an update of the device
personal build, preserving its existing features. No pull request is authorized.
SSH was restored on October 4. An isolated ARM64 build/test/benchmark is underway
using the device’s existing GCC 9 toolchain; the installed app is unchanged.

## Delivered instrument paths

| Type / stable ID | Implemented and native persistence | Machine tests | Packaged CNI | R36H |
|---|---|---|---:|---|
| OPLL / YM2413, 17 | Yes | Pass | 15 | Pending |
| VRC7, 18 | Yes | Pass | 15 | Pending |
| OPL2, 19 | Yes | Pass | 241 | Pending |
| OPL3, 20 | True four-op and dual voice | Pass | 456 | Pending |
| Sega PSG, 21 | Tone, white/periodic noise | Pass | 14 | Pending |
| GB Pulse, 22 | Native duty/envelope/sweep | Pass | 10 | Pending |
| GB Noise, 23 | Native divisor/shift/width/envelope | Pass | 10 | Pending |
| DX7 FM, 24 | Six operators, all original voice parameters | Pass | 67 | Pending |
| Genesis FM / YM2612, 25 | Four-op native envelope/routing/LFO | Pass | 24 | Pending |
| Arcade FM / YM2151, 26 | Four-op native envelope/routing/LFO | Pass | 24 | Pending |

The instrument hierarchy, metadata, common FM browser, bounded voice lifecycle,
existing track inserts/sends, project handoff and serializers are extended in
place. Retired IDs 14/15 and MIDI 16 remain unchanged; no FX identifiers were
renumbered. Existing fonts/theme, piano, waveform options, mixer, MIDI and
personal experiments are retained. The personal desktop package enables
`CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1`; web retains that experiment's existing off policy.

FM has bank/category/All browsing, quick previous/next, full-name helper,
EDIT+PLAY audition, EDIT confirm and OPT cancel. Preview owns its patch and
never commits tables or track inserts. The same browser handles local DX7
SysEx. Simple chip pages expose their native controls and starter presets.
Optional favorites/search were not added. No operator editor or new FM macros.

## Dependencies and ownership

| Core | Revision | License / audit |
|---|---|---|
| ymfm | `81aec25ccbb98f4873a255f7551ac4dadac59b4a` | BSD-3-Clause, unchanged selected OPL/OPN/OPM plus closure |
| emu76489 | `c0fa097060e022db237163d79704025435042997` | MIT, documented Sega noise-clock correction |
| gb_apu | `3d73d0df027a82d854cacd72a179c2d6a1a9703e` | MIT, selected relicensed C++ Blip closure, C++ allocation cast and equivalent positional tables for GCC 9 |
| MSFA scalar core in Dexed | `2e182b3db85c09083ab13c8b9b00565ce7d9ff85` | Apache-2.0 file audit, namespace/host decoupling, small note adapter |

File hashes and modifications are under each `chipnomad_lib/external/*` vendor
folder. Runtime licenses and Apache NOTICE are under `packaging/common/licenses`.
No full Dexed/JUCE/MTS/MPE or extra DX7 core is linked. Source collections and
engine permissions were reviewed separately.

Patches are fixed owned values; Instrument remains 680 bytes, its union 624
bytes and Project 313,256 bytes. The audio command payload layout is 568 bytes (64 slots: 36,352 bytes);
ChipNomadState is 782,032 bytes on this host. Project snapshots have not grown.
Voices are allocated by bounded track/chord
slot, not by catalogue entry. DX7 has one shared LFO per track part and four
independent note states per part; unrelated tracks and preview are isolated.
DX7 uses a measured **16-active-note global budget** within those existing owned
slots, including release tails. Over-budget note events take releasing voices
first, then quiet held notes, then fresh attacks. Equal attacks preserve roots
across tracks before chord extensions, with deterministic slot/track ordering.
Other instrument polyphony is unchanged; DX7 preview is blocked during playback.
The same policy applies across platforms. Raw backend 32-note stress remains
available for measurement, bypassing the sequencer's admission policy. MSFA global tables are
initialized once off callback at 44.1 kHz, then streamed through the common FIR
for arbitrary host rates. This avoids changing global tables beneath another
renderer. Event quantization is bounded by 64 native samples (1.45 ms) plus
approximately 0.25 ms FIR delay. No remainder samples are dropped. Yamaha key
retrigger observes one native key-off sample before key-on. Genesis uses a
20 Hz DC blocker around the modeled ladder output. Native FM rate/level envelopes
are preserved; volume changes are post gain, DX7 strike velocity is an owned
wrapper default of 100, and pitch updates preserve live phase/envelope state.

CNI/CCT version 6 stores complete patches, tuning and bounded source metadata.
Old formats 1–5 remain readable; older applications cannot load version 6.
Copy/paste/clone and source-independent reopen are tested. Bad native imports
are transactional. Loading a bank never sends MIDI messages.

## Content and imports

| Bank | Source entries / packaged | Distinctness and exclusions |
|---|---:|---|
| FatMan 2-op | 181 / 181 | 53 percussion, explicit embedded MIT notice |
| FatMan 4-op | 181 / 181 | 180 true four-op, 53 percussion, MIT |
| DMXOPL3 | 335 / 335 | 252 dual, 5 four-op, 183 percussion, MIT |
| OpenDX7 originals | 31 musical / 31 | MIT; INIT excluded; data-only literal conversion |
| YSE author CC0 bank | 32 / 4 | Four unique templates; renamed duplicates excluded |
| ChooChoo DX7 originals | 32 / 32 | Independent CC0 recipes, not ROM copies |
| ChooChoo Genesis | 24 / 24 | Original MIT recipes |
| ChooChoo Arcade | 24 / 24 | Original MIT recipes |
| OPLL / VRC7 tables | 15 + 15 | Pinned BSD-licensed ymfm tone data |
| Sega / GB Pulse / GB Noise | 14 + 10 + 10 | Original MIT recipes |

Total: **876 CNI files**, **812 shared FM catalogue entries** and **704 distinct
normalized FM parameter sets**. OPL's 697 source aliases represent 589 parameter
sets; source-bank identities remain preserved. DX7 has **67 distinct parameter
patches from 95 parsed entries**. Source full-record hashes, parameter hashes and
aliases are separate in the manifests. The 10,000-entry index test is synthetic
and does not inflate the factory count.

The DX7 1,000-cleared-preset goal is **unmet**. Benson's mixed collection includes
factory/unknown origins; Bobby Blues' broad web compilation is not blanket
cleared, and the reviewed direct Pafreak submission contains DX7II extensions.
BlackWinny and CC0 mirrors do not establish every contributing author's grant.
These files stay outside shipping assets. Source-specific review records under
`tools/chip_banks/evidence` identify the evidence and exclusions. Broad Genesis
collections similarly had no verified bank-specific clearance; original banks
provide the requested fallback. Human listening/category refinement remains open.

Offline `import_bank.py` supports exact 42-byte TFI, original VOPM text,
verified WOPLX, and original DX7 SysEx. VOPM pan 0/64/127 maps to left/both/right;
partial pan, noise data and extended fields are rejected. AM enable uses the
original file's 0/128 field. Binary WOPL is not implemented. Runtime DX7 import
supports framed 155-byte voice and 4096-byte packed-bank payloads, including
bounded multiple messages, strict lengths/ranges/seven-bit/checksum/terminator
validation, and atomic rejection. Headerless dumps and bad-checksum overrides
are not enabled. Imported DX7 banks remain session metadata; selected instruments
can be saved as portable CNI/CCT. Offline user imports get a separate manifest,
with no redistribution permission inferred. ZIP inspection is offline, bounded,
path-safe and non-executing.

## Verification and artifacts

Baseline: 328 tests / 8,101,169 assertions passed before integration.
Final standard suite: **359 tests / 72,681,719 assertions passed**.
Personal experiment suite: **371 passed, 2 deliberately skipped**, 72,691,756
assertions passed. Python content tests: **13 passed**. A latent MIDI test passed
an uninitialized destination into a replacing/freeing loader; its setup is now
initialized. Production loader behavior was not changed for that test issue.

Coverage includes native save/reopen, failure transactions, same-patch independent
voices, fine pitch at multiple output rates, uneven render blocks, release/cut,
retrigger, all DX7 algorithms, direct MSFA and ymfm register references, all 876
native file reloads, every FM preset's finite audible render, 10k metadata scale,
local SysEx parsing, and production SDL browser/audition/clone workflows. Selected
MSFA scalar code also passed AddressSanitizer/UndefinedBehaviorSanitizer. Host
visual testing used SDL dummy output; no visible emulator/editor was launched.

Actual commands and build details are in `docs/build-notes.md` and the progress
checkpoint. Desktop macOS x86_64 personal build and the existing Emscripten web
build succeed. The checked-in web/dist bundle is regenerated. Windows/Android
builds were not run. No local ARM64 toolchain was available; the authorized
ARM64 build is now running on the handheld with its existing compiler. Existing Docker images were inspected only; none was an ARM64
Linux project builder. No second SDK was installed.

`tracker/packaging/common/projects/native-chip-audition.cct` owns thirteen bank
representatives and plays without the factory folder. `.tmp/chip-audit/auditions/`
contains thirteen WAVs with four selected sounds each (52 machine auditions),
short phrases, held notes and release segments. `docs/chip-preset-auditions.tsv`
records source preset paths and peak/RMS/DC. No subjective listening is claimed.
Large WAVs stay out of Git. All 876 generated CNI files and their catalogues/
manifests regenerated byte-identically in a separate ignored output directory.

OPL native register values and source volume-model/velocity-offset/duration
metadata are retained. Playback uses native full-velocity levels and tracker
post gain; it does not reproduce ADLMIDI player-specific MIDI volume curves,
and duration estimates never truncate a held note or release tail.

## Measured host performance

Optimized macOS x86_64 build, `-O3 -DNDEBUG`, 62 configurations, 30 seconds of
rendered audio per configuration. Exact CPU model was unavailable in this session.
At 48 kHz / 512 frames, DX7 with 16 voices measured mean 143.248 µs,
p95 196.915 µs, p99 281.532 µs, worst 386.248 µs. With 32 voices:
mean 292.164 µs, p95 419.497 µs, p99 550.174 µs, worst 689.200 µs.
Neither DX7 case missed its 10.667 ms render deadline.

The full stress sweep recorded **three deadline misses**, all in the 32-voice
OPL2 case (worst 22.245 ms). Other validation/build work overlapped portions of
host measurement. These outliers remain recorded; no universal glitch-free
polyphony claim is made. See `docs/chip-all-chip-benchmark.csv` for every row.

A separate paced 600-second FM-heavy eight-track song, with inserts/sends and
9,566 concurrent scans of a synthetic 10,000-preset catalog, measured p95 2.903 ms,
p99 2.939 ms, worst 4.409 ms against a 10.667 ms render deadline, with **zero
render deadline misses**. Peak resident memory was 112,623,616 bytes; this is a
high-water mark, not a memory-growth trace. See `docs/chip-soak-benchmark.csv`.
Measurements cover render duration, not scheduler wakeups, physical audio-device
underruns, handheld governor/thermals or a human listening test. The existing
four slots per track remain provisional; no handheld limit has been selected.

Final production SDL dummy UI smoke passed, including all ten pages, preset
preview/cancel/confirm, local DX7 import, clone, and sequencer playback. Its
48 kHz / 1024-frame song-plus-UI p95 was 1.503 ms, p99 1.807 ms, worst 2.366 ms.
The host tools now include their own generated header dependencies to prevent
stale object layouts after voice-header changes.

## Packaging

The reproducible desktop packaging command is `tools/chip_banks/package_desktop.py`.
The local archive is `releases/ChooChooTracker-native-chips-macos-x86_64.zip`;
it contains the personal executable, SDL framework, runtime assets, all 876
native presets, the portable demo, source-specific notices and this report.
The packager checks preset count, required notices and ZIP CRCs and records a
SHA-256 inventory. This macOS archive is not a handheld installation package.

## Remaining acceptance work

- Build the ARM64 device personal build with the existing approved target
  toolchain; the authorized isolated validation is now underway.
  Benchmark 1/4/8/16 DX7 voices, mixed/chord/effect loads and a 10–15 minute
  run before choosing the handheld voice limit. Do not infer it from this Mac.
- Complete human listening of the supplied bank WAVs and demo; refine ambiguous
  categories and balance only with explicit reversible wrapper settings.
- Acquire further author-cleared DX7 content if the 1,000-sound goal remains
  desired. The 67-preset starter and large user-library import path are delivered.
- Binary WOPL, OPM noise/partial-pan variants, DX7II/performance extensions,
  runtime archives, search/favorites and operator editing are unsupported as
  described above. No claim is made that these formats are silently equivalent.
- Before any later install, follow the existing device personal-build procedure,
  preserve all user assets/settings, and verify a fresh rollback copy. This work
  makes no changes to the current installation or its existing rollback records.
