# Native chip and FM instruments

This contribution is based on upstream main `8c87091`. It extracts the current
instrument implementation from the user-accepted R36H personal build. Personal
module discovery, launcher branding, deployment receipts and unrelated sample
stretch/import fixes are not part of this branch. The original development and
deployment history remains on `personal/r36h` and `feature/native-chip-instruments`.

## Included

SID; OPLL and VRC7; AdLib/OPL2 and OPL3; Sega PSG; Game Boy Pulse and Noise;
Genesis/YM2612 and Arcade/YM2151 FM; and DX7/MSFA instruments. Native patches
are owned by the instrument and saved in CNI/CCT files, including song archives.
The contribution retains the tested archive integration for mixed sample/native
songs and embedded wavetable layouts.

The shared preset browser provides bank/category selection and owned audition
previews. DX7 also supports a persistent folder of original SysEx banks.
The checked-in library contains 1,196 preset files, with source manifests,
conversion tools and runtime notices. Entries and unique sounds are different
counts; duplicate source identities and exclusions are recorded by the tools.

Tracker FX use the final compact per-engine list documented in the user manual.
The operator selector has been removed, retained operator commands address
operator 1, and live modulation keeps its explicit operator targets. Preset/range
readouts use native bounds. No retired personal FX translation is included.

## Dependencies and limits

The sources include ymfm, emu76489, gb_apu, the MSFA scalar DX7 component, and
the floooh/chips SID implementation. License/provenance notices accompany each.
Preset content has its own source licenses; consult the packaged notices.
No Dexed application, JUCE, alternate SID model or network importer is added.

SID is a digital/per-cycle 6581-style approximation with a per-note filter,
not a calibrated analog revision model. Native voice allocation and handheld
budgets remain as tested in the accepted personal build. Polyphony, release tails,
other instruments and inserts share the CPU budget; the user manual describes
practical limits. The historical chip report/CSV files retain earlier workload
measurements and their qualifications.

## Review checks

The accepted combined build has host/device validation and user listening behind
it. This isolated contribution builds against current upstream. Five focused cases
passed (927 assertions): compact FX availability, single-command motion recording,
OPLL CNI/CCT persistence, mixed native/sample archives, and DX7 folder rescanning.
The regenerated web build also passed JavaScript syntax and WebAssembly validation.
The extraction was checked with focused tests, without a full suite, stress run,
or new device installation.

### PR #39 MIDI lifetime follow-up (2026-10-06)

Review found that the MIDI tests and standalone converter passed uninitialized
projects into loaders that release the previous instrument data on success.
These callers now initialize an empty project first, matching the tracker UI.
The MIDI API documents that successful imports replace the destination and
failed imports leave it unchanged. No audio-engine behavior changed.

The five MIDI tests pass (49 assertions), including repeated replacement of a
sample-owning project and preservation after missing, malformed, or empty MIDI
input. The converter entry point also passed a MIDI/CCT/MIDI/CCT round trip with
allocation scribbling enabled, linked against the cached core and test support
objects; this was not a standalone release-package build.

### PR #39 FM controls and full-suite follow-up (2026-10-06)

The failing FM test still required all live modulation destinations to appear
in tracker FX. Its assertions now distinguish native modulation metadata from
the intentionally smaller per-engine tracker list, which retains its explicit
availability tests. No removed tracker command was restored.

Additional coverage switches one OPL3 instrument between two-operator,
four-operator, dual-voice, and back to two-operator configurations, checking
actual operator availability. CNI and CCT reload checks preserve each topology,
operator settings, and modulation destinations. The existing instance-level
availability rules passed these checks without synth or runtime changes.

`make -C tracker -f Makefile.test -j4` passed locally: 501 test cases,
112,209,328 assertions, zero failures or skipped cases. This includes the MIDI,
native preset, and existing project/instrument persistence tests. Linux GitHub
CI must still rerun after the branch update; local Docker was unavailable.
