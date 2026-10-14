# Library compatibility report

Measured 2026-10-14 on the development Mac, except the separately labeled R36H results.

“Loaded” means complete preset preparation and a 1024-frame finite-render smoke test at 48 kHz, using a fresh 256 MiB PCM cache. It does **not** mean full sampler fidelity, every articulation tested, or suitability for the 64 MiB handheld default. Warnings remain partial compatibility. Counts include alternate distributions; raw collection assets are not all separate multisample instruments.

| Collection / distribution | Format | Preset outcomes | Loaded | Failed | Failure reasons | License | Collection archive bytes¹ |
| --- | --- | ---: | ---: | ---: | --- | --- | ---: |
| VSCO 2 Community Edition / SFZ | SFZ | 75 | 75 | 0 | None in this smoke test | CC0-1.0 | 4,595,601,174 |
| Versilian Community Sample Library / SFZ | SFZ | 183 | 179 | 4 | PCM cache budget: 3; Unsupported audio asset (expected WAV/AIFF): 1 | CC0-1.0 | 8,095,720,116 |
| Meta Open Sound / SFZ/Ogg | SFZ/Ogg | 3 | 2 | 1 | Ogg Opus is unsupported; Vorbis is supported: 1 | CC0-1.0 | 1,929,215,940 |
| Meta Open Sound / SFZ/WAV nylon | SFZ/WAV nylon | 1 | 1 | 0 | None in this smoke test | CC0-1.0 | 1,929,215,940 |
| Meta Open Sound / SFZ/WAV steel | SFZ/WAV steel | 1 | 1 | 0 | None in this smoke test | CC0-1.0 | 1,929,215,940 |
| Meta Open Sound / SFZ/WAV piano | SFZ/WAV piano | 1 | 0 | 1 | PCM cache budget: 1 | CC0-1.0 | 1,929,215,940 |
| Stargate Sample Pack / WAV/AIFF singles | WAV/AIFF singles | 348 | 341 | 7 | Unsupported WAV alternating/backward loop: 2; Audio asset contains a partial PCM frame: 1; Invalid audio pitch/loop metadata: 4 | CC0-1.0 | 197,458,267 |
| FluidR3Mono 2.315 / SF3 | SF3 | 197 | 197 | 0 | None in this smoke test | MIT | 105,170,228 |
| GeneralUser GS / SF2 | SF2 | 287 | 287 | 0 | None in this smoke test | LicenseRef-GeneralUser-GS | 65,366,576 |
| Akai S612 Modular / EXS/SFZ | EXS/SFZ | 120 | 2 | 118 | Missing source sample: 113; SFZ region missing sample: 5 | LicenseRef-PublicDomain-Unlicense-Content | 16,938,533 |
| Salamander Grand Piano V3 / SFZ/WAV | SFZ/WAV | 1 | 0 | 1 | PCM cache budget: 1 | CC-BY-3.0 | 1,257,837,016 |
| Gugak EXS24 — Korean Instruments / EXS/WAV legacy ZIPs | EXS/WAV legacy ZIPs | 89 | 89 | 0 | None in this smoke test | LicenseRef-CATSeoul-Personal-Noncommercial | 5,701,648,138 |
| Gugak EXS24 — Korean Instruments / EXS/WAV v1.3.1 original files | EXS/WAV v1.3.1 original files | 41 | 41 | 0 | None in this smoke test | LicenseRef-CATSeoul-Personal-Noncommercial | 5,701,648,138 |

¹ Collection totals repeat for alternate distributions in this table; do not sum the table’s size column.

## Totals and limits

- covered banks: **865**.
- preset outcomes: **1,347**.
- loaded: **1,215**.
- failed: **132**.
- presets with warnings: **732**.
- unique warning messages: **38**.
- detected missing asset references: **110**.

Acquired **9 collections / 37 complete original artifacts**, **21,964,955,988 archive bytes** (20.46 GiB), with **29,360,276,887 member bytes** across distributions. Akai's current repository is complete as downloaded but does not contain the advertised original collection's full audio. No Factory content is packaged.

Six collections are license candidates pending compatibility/provenance review; Stargate and GeneralUser require additional provenance review. Gugak is acquired for private USER evaluation under its publisher notice; the separate National Gugak Center KOGL Type 1 notice has not been verified as covering these EXS24 files. Zero collections are Factory-approved. These are review statuses, not blanket redistribution permission.

Missing-asset counts are unique references encountered before preparation stopped at the first failure per preset, not an exhaustive count of every missing file. Five malformed SFZ files have no discoverable preset and are counted as failed instrument outcomes. Full per-member hashes and instrument names remain in the manifest/external inventories and [machine-readable report](compatibility.json).

Metadata scanning found **17,330 banks/assets and 18,003 preset entries**, with 5 parse errors. First scan: **5122.20 ms**; cached scan: **1590.96 ms**. This pre-Gugak scan includes raw samples and duplicate distributions, and decodes no sample audio. Gugak playback results above are additional. It is distinct from the smaller mapped-instrument playback suite above.

## Recurring compatibility gaps

- VCSL: three pianos exceed 256 MiB. With 1024 MiB configured, Kawai, Steinway B and Knight all prepare (455,933,860 / 893,137,864 / 274,112,236 PCM bytes respectively). They are not suitable for the 64 MiB handheld default. Gong 2 references unsupported MP3. CC-triggered pedal samples, pedal conditions and release-decay opcodes remain unsupported and reported.
- Meta: the repository piano uses Ogg Opus, which the Vorbis decoder explicitly rejects. The complete WAV piano release prepares at 1024 MiB (590,935,736 PCM bytes). The guitars load, but their full selected articulations also exceed the 64 MiB default. No source conversion was performed.
- Salamander: metadata and 637 regions parse; the full preset exceeds even the 1024 MiB PCM budget. This is acquired and indexed, not successfully loaded. Streaming or a disk-backed sample strategy would be the next substantial memory improvement.
- Akai: 102 EXS definitions and 18 SFZ definitions are present; only two EXS instruments have all required samples and prepare. Most references are absent from the publisher repository; five SFZ definitions are malformed. EXS group/instrument synthesis parameters warn. This is limited real-world EXS evidence, not broad EXS compatibility.
- Gugak: all 23 publisher EXS24 folders acquired, including older versions. Latest v1.3.1 tests load 41/41 presets (10 with unsupported-parameter warnings). Those ZIPs contain Mac disk images: an optional macOS acquisition helper copied unchanged Data-tree files from read-only mounts without running installers. The application does not read DMG or nested ZIP containers. Legacy ZIP testing covers 89 presets: all 89 prepare after preserving original EXS directory suffixes to disambiguate duplicate Ajaeng bow-sample filenames. Source audio and mappings are unchanged. Original archives, manuals and attribution remain in the private sideload bundle. EXS presets do not reproduce separate Logic channel strips, effects or plug-in settings. At the 64 MiB handheld cache budget, 40/41 current presets prepare on the Mac; the combined Geomungo preset needs 79,795,564 PCM bytes. This budget test is not an R36H playback measurement.
- Stargate: four samples have out-of-range loop metadata, two advertise unsupported loop modes and one contains a partial PCM frame. These fail explicitly rather than silently repairing source audio.
- SF2/SF3: GeneralUser and FluidR3Mono prepare completely, but unsupported custom modulators/generators can alter timbre. SF3 testing covers mono Vorbis sample streams and two observed sample-end conventions.
- SFZ articulation switches, sustain pedal, dynamic envelopes, rt_decay and additional filter modes are the highest-value remaining parser/renderer improvements. Large-preset streaming is needed for the largest pianos; increasing the handheld cache alone is not a solution.

## Reproduce

Run `tools/soundbanks/acquire.py`, then `inventory.py`, and the probe commands in the [guide](README.md). Retain JSONL under the external data root’s `results/` directory. Run `python3 tools/soundbanks/report.py /external/soundbank-data` after those tests to regenerate this report. The report script requires actual test records and never marks an untested acquisition as loaded.

See [validation and performance](validation.md) for platform build results and isolated hardware measurements.
