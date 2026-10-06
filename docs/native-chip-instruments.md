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
No new full suite, stress run or device installation is planned for this extraction.
