# Experimental native SoundBanks

This implementation lives on `experimental/native-soundbanks`, based on upstream
main `583702d`. It is an experimental contribution for a later daily integration,
not a deployment to the device personal build. See [validation](validation.md)
and the [library compatibility report](compatibility.md) before packaging it.

## Using a bank

1. Copy an original SF2, SF3, SFZ or EXS instrument, a WAV/AIFF sample, or a ZIP
   containing those files into `instruments/USER/soundfont`. Retain relative sample directories.
2. In Instrument, select **SOUNDFONT**. Open **BANK MANAGER**, select a collection
   and instrument, then open **PRESET BROWSER** and select a preset with EDIT.
3. Wait for “Preset ready”. Existing note entry plays the prepared instrument;
   EDIT + PLAY previews the selected, loaded preset in the browser.
4. Save the project normally. It records a root-relative bank/member path and a
   stable preset ID, not the browser row number or decoded samples.

The two browser pages are children of Instrument. They use the existing scrolling
selection popup and controller mappings. Collections are grouped by their first
folder, or their ZIP filename. Mappings appear before a separate raw WAV/AIFF
sample list. This small extra collection level keeps thousands of source samples
from burying the actual instruments. All collections are USER banks.

Keep Korean instruments under `instruments/USER/soundfont/Korean_Gugak/`, retaining `Wind/` and
other original family directories. This preserves a dedicated Gugak identity in
the same USER bank infrastructure. All 23 publisher EXS24 folders have been
acquired into private external storage, including older versions. Recent Gugak
releases contain Mac disk images inside ZIPs: these are preserved originals,
not directly playable ZIP banks. Older EXS/WAV ZIPs are tested separately.
See the attribution record before considering any redistribution.

## Storage and configuration

Portable desktop/PortMaster builds use `instruments/USER/soundfont` relative to their normal
working data directory. AppImage, Android and Windows use the existing default
application data-directory helper. Set `CHOOCHOO_SOUNDBANKS` to override the root.
The sample cache defaults to 64 MiB of decoded PCM; set
`CHOOCHOO_SOUNDBANK_CACHE_MB` to an integer from 8 through 1024 before launch to
change it. This is a memory policy, not a download-size limit. Allow room for the
rest of the tracker, compressed buffers and temporary preparation data.

The web wrapper has **Import sound banks** and **Import bank folder** controls.
**Import USER ZIP** accepts archives rooted at `instruments/USER/` or `USER/`,
keeps existing files and preserves relative bank/sample paths. It accepts up to
1 GiB of new uncompressed files per import and 512 MiB per file.
They retain selected relative paths under `/user/instruments/USER/soundfont` and use existing
persistent browser storage. ZIP import is convenient where folder picking is not
available. Rescan in Bank Manager after importing. Browser storage quotas apply;
its virtual filesystem can keep imported archives and temporary files in memory,
so the native disk-backed memory profile does not apply to large web imports.

Metadata enumeration does not decode sample audio. The `.choochoo-index-v3` cache
uses source size/mtime, archive member CRC and dependency size/mtime. Changed
entries are rebuilt. Unsupported-feature warnings persist in the index; failed
entries and missing references are retried on rescan. Deleting the index is safe.

Native preparation runs on one background worker. Web preparation is deferred
until a loading state has been drawn and yields through existing Asyncify support;
full browser interaction during long preparation has not yet been playtested.
Only the selected preset's samples are decoded. Active presets and voices pin
shared samples; unpinned samples are released before another preparation. A load
that exceeds the budget fails and retains the previous playable preset. Clear
unused instruments or increase the budget before retrying. There is no disk
streaming yet, so large pianos may be indexed successfully but cannot be loaded
within handheld memory limits.

ZIP entries are read individually. Deflated members use a bounded-memory temporary
file deleted on close; originals are never changed and no permanent duplicate
collection is produced. A compressed SF2/SF3 member must be inflated in full to
make its RIFF data seekable, even for metadata enumeration. There is a 2 GiB
uncompressed member limit, a 64 MiB central-directory limit and a 200,000 member
limit. ZIP64 is supported; encryption, unsupported compression, invalid CRCs,
escaping paths and ZIP-inside-ZIP traversal are rejected or excluded.

Missing banks produce a visible error, never an unrelated substitute. References
survive cache rebuilding and moving the entire SoundBanks root. Renaming the bank
or its relative directories requires reselecting it. Relative sample lookup
repairs case differences, searches the nearest enclosing collection first, and
uses original EXS directory suffixes to distinguish reused sample filenames.
Lookup stays inside the configured root/archive; equally good matches fail
explicitly. Windows drive paths and legacy HFS separators are supported as
relocation hints, never as permission to open the original absolute path.

## Format coverage

| Format | Implemented | Explicit limits |
| --- | --- | --- |
| SF2 | Bank/program IDs, zones, key/velocity ranges, root/tuning, offsets, gain/pan, stereo sides, loops, amp ADSR/delay/hold, low-pass filter, exclusive classes | Custom modulators, modulation envelopes/LFOs and other unhandled generators warn. 24-bit extensions play at 16-bit precision. ROM samples fail. |
| SF3 | SoundFont version 3 mono Ogg Vorbis sample streams, including tested inclusive/exclusive sample-end conventions | Compressed stereo-per-record and Opus fail. No assumption that arbitrary SF2-compatible compression is supported. |
| SFZ | control/global/master/group/region inheritance, bounded includes/macros, default_path, key/velocity mappings, pitch/gain/pan, offsets, loops/one-shot/reverse, amp envelope, low-pass filter, choke, round-robin, random selection, velocity curves/fades, attack/release regions | Default channel/articulation only. Live keyswitches, CC-triggered regions, sustain-pedal logic, dynamic envelopes, non-low-pass filters and additional opcodes warn. Unsupported trigger/loop types fail. Not full SFZ. |
| EXS24 | Legacy endian variants, instrument name, zone/sample records (336/592/600), key/velocity/root/tuning, gain/pan, offsets, loops/reverse, sample ordinal references | Group synthesis/envelopes, instrument parameter blocks, fades, loop crossfades and routing warn. Gugak validates 130 presets across current and older releases; Akai remains limited by missing publisher samples. |
| WAV | PCM 8/16/24/32, float32, supported extensible PCM/float, mono/stereo, smpl root/fraction/forward loop | Ping-pong/backward loops and inconsistent metadata fail. Diagnoses a narrowly recognized omitted odd-chunk pad. |
| AIFF/AIF | PCM and AIFC NONE/twos/sowt, rate/channels, INST root/detune and MARK sustain loop | Other compression and ping-pong loops fail. Release-loop and advanced instrument metadata are not mapped. |

Referenced Ogg Vorbis samples are also decoded for SFZ. Standalone OGG, FLAC and
MP3 are not instrument types. SFPack is excluded: research did not establish a
permissively licensed, independently usable decoder/specification. SBK, sfArk,
Kontakt, GigaStudio, HALion and Reason formats are outside this feature.

The renderer uses existing envelopes, filter, tracker mixer, effects and note
routing. Voices are fixed per tracker/chord slot, with eight sample layers per
voice. Retriggering steals that slot deterministically. Preparation rejects
instruments that can exceed the layer bound; there are no note-time allocations,
file operations, archive decoding or format parsing. Pitch/volume MOD sources and
instrument/track pan use existing routes. Filter/envelope source parameters are
imported; a new editable filter/envelope MOD interface is not implemented.

## Reproducing acquisition and checks

Complete archives are external to Git. The root
[soundbank_manifest.json](../../soundbank_manifest.json) records source versions,
URLs, checksums, sizes, licenses and acquisition status. Original license documents
are under `licenses/`; member checksums live in the external `inventories/` folder.
[Attribution](../../SOUNDBANK_ATTRIBUTION.md) separates code and sample terms.

```sh
python3 tools/soundbanks/acquire.py /external/soundbank-data
python3 tools/soundbanks/inventory.py /external/soundbank-data
make -C tracker -f Makefile.soundbanks soundbanks-test soundbanks-probe
cd tracker
build/soundbanks/probe /external/library.zip --instruments --cache-mb=256
build/soundbanks/probe /external/SoundBanks
```

The directory command measures metadata discovery/indexing; the file/ZIP command
prepares each preset and renders 1024 frames. Run the latter from `tracker` when
using repository fixtures. It does not prove every key, layer or articulation
sounds like the original sampler. `--metadata` skips audio preparation. The
ordinary full regression command is `make -C tracker -f Makefile.test -j4`.

Do not package acquired collections automatically. A future Factory selection
requires collection-specific provenance, terms and compatibility review. No
Factory category, downloader, online dependency or Factory packaging is added.

## Private Gugak acquisition

`python3 tools/soundbanks/acquire_gugak.py /external/soundbank-data` downloads
all 23 publisher-linked EXS24 folders. `bundle_gugak.py DATA_ROOT PRIVATE_BUNDLE`
retains the complete transport ZIPs under `Original downloads/Korean_Gugak/`
and copies their unchanged original share files to `instruments/USER/soundfont/Korean_Gugak/`.
It preserves older-version paths, checks SHA-256 and refuses to replace different
existing files. It does not extract or convert sample audio, run installers,
or make disk-image releases directly playable. The private bundle is outside
Git and release packaging. Its existing synth preset packs remain unchanged.

`python3 tools/soundbanks/prepare_gugak_dmg.py PRIVATE_BUNDLE` is an optional
macOS acquisition helper for the publisher's v1.3.1 disk-image releases. It mounts
images read-only and copies their original Data trees byte-for-byte into each
instrument's `v1.3.1/` folder. It records and skips installer shortcuts, never
runs publisher installers, and preserves all original ZIPs. The resulting EXS
and WAV files load directly through the normal filesystem importer. This is
source distribution access, not format conversion or runtime DMG support.

`bundle_collections.py DATA_ROOT PRIVATE_BUNDLE` copies the other acquired
reference libraries into that same private bundle, verifies original archive
checksums and retains the existing FluidR3Mono/Salamander distribution trees.
The bundle is named `ChooChoo-Sideloaded-and-Non-attributable-Presets`: sideloaded
includes attributable collections that are too large to package with the app.

Reproduce the complete Gugak playback checks with
`python3 tools/soundbanks/probe_gugak.py PRIVATE_BUNDLE DATA_ROOT/results`.
This tests original legacy ZIPs and all prepared current EXS files separately,
including an additional 64 MiB cache-budget run. Use `inventory.py DATA_ROOT --private-bundle PRIVATE_BUNDLE` to include the original publisher-file and
disk-image preparation inventories when rebuilding the acquisition manifest.

The complete private preservation copy occupies **31,548,217,308 bytes (29.38
GiB)** before the existing synth presets, metadata and filesystem overhead.
That includes original wrappers, alternate formats and older versions; it is
not the minimum playback installation. The tested handheld currently has a
19 GiB `/roms` partition with 1.5 GiB free, so the complete set remains offline.
No library or app installation was made on the handheld during this work.
