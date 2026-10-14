# Soundbank acquisition and attribution

Acquisition date: 2026-10-14. All collections are USER banks, stored outside Git.
Original archives, mappings, recordings and folder structures are unchanged.
Alternate distributions can duplicate the same recordings; totals are storage
footprints, not counts of unique recordings. A successfully downloaded repository
may still omit sample assets referenced by its original mappings.

The machine-readable record is [soundbank_manifest.json](soundbank_manifest.json).
It records publisher, original source/download URL, version/commit, exact artifact
SHA-256, compressed/unpacked sizes, format inventory, attribution requirements,
license-document checksums and compatibility results. External `.jsonl.gz` member
inventories record every ZIP/tar member and Debian data-archive member's checksum. Original documentation is
preserved under [docs/soundbanks/licenses](docs/soundbanks/licenses).

| Collection | Original publisher | Sample terms | Redistribution review |
| --- | --- | --- | --- |
| [VSCO 2 Community](https://github.com/sgossner/VSCO-2-CE) | Versilian Studios / Sam Gossner and contributors | CC0 1.0 | Retain contributor credits; verify selected original documents before Factory inclusion. Complete raw and SFZ branches acquired. |
| [VCSL](https://github.com/sgossner/VCSL) | Versilian Studios / Sam Gossner and contributors | CC0 1.0 | Preserve original per-folder notes and main-branch license alongside SFZ branch. Complete raw and SFZ branches acquired. |
| [Meta Open Sound](https://github.com/facebookincubator/Meta-open-sound) | Meta Creative Audio | CC0 1.0 | Retain LICENSE and author identity. Complete Ogg repository and all three WAV releases acquired. |
| [Stargate Sample Pack](https://github.com/stargatedaw/stargate-sample-pack) | Stargate DAW and credited contributors | CC0 1.0 | Compilation: preserve source-specific README/Freesound credits and review per-asset provenance before Factory selection. |
| [FluidR3Mono](https://packages.debian.org/sid/fluidr3mono-gm-soundfont) | Frank Wen, Michael Cowgill, Ethan Winer, Michael Schorsch | MIT | Preserve complete FluidR3Mono license and copyright notices. Original SF2 source tar and Debian SF3 package acquired. |
| [Salamander Grand Piano](https://freepats.zenvoid.org/Piano/acoustic-grand-piano.html) | Alexander Holm | CC BY 3.0 | Credit Alexander Holm, retain source and license links, indicate modifications if any. Complete original 48 kHz/24-bit WAV+SFZ distribution acquired; no audio modifications. |
| [GeneralUser GS](https://github.com/mrbumpy409/GeneralUser-GS) | S. Christian Collins and credited contributors | Custom GeneralUser GS license | Original license notes uncertainty about historical sample provenance. Not approved for Factory distribution. Preserve every acknowledgement and review terms. |
| [Akai S612 Modular](https://github.com/publicsamples/Akai-S612-Modular) | Modular Samples / publicsamples | Public-domain dedication using content-adapted Unlicense wording | Preserve LICENSE and publisher. Complete current repository acquired, but many original audio dependencies are absent. |
| [Korean Gugak](https://www.catseoul.com/gugak-library/) | Seoul Center for Arts & Technology, formerly CATSNU / Seoul National University | Publisher personal/noncommercial notice; attribution required | All 23 EXS24 download folders acquired, including older versions and original documentation. Dedicated private USER collection with Wind, Strings and Percussion families; not packaged in Git or public builds. |

The current publisher supplies complete Dropbox folder archives. These contain
original distribution ZIPs; recent versions wrap Mac disk images, while older
versions contain EXS/WAV directly. Preserve every original and distinguish
acquisition from native-loader compatibility. Disk images and nested archives
are not supported runtime instrument containers.

The [National Gugak Center digital-sample service](https://www.gugak.go.kr/site/program/board/basicboard/view?boardid=42216&boardtypeid=60&currentpage=22&lang=ko&menuid=001005006002&pagesize=10)
separately announces KOGL Type 1 (source attribution). That notice has not been
verified as covering these CATSNU/CATSeoul EXS24 files. Preserve the collection's
[own notice and required acknowledgement](docs/soundbanks/licenses/Korean_Gugak/publisher-terms-20261014.txt).
The original publisher HTML is preserved in external acquisition storage.
No Kontakt conversion or publisher installer execution is performed.

[SFZ Instruments Catalog](https://sfzinstruments.github.io/) was used as a
discovery resource, not treated as a downloadable collection or licensing grant.
Additional collections selected beyond the initial list were FluidR3Mono,
GeneralUser GS, Akai S612 Modular and Salamander. Their rankings and provenance
are in the manifest; the compatibility report records practical loading limits.

## Implementation licenses

The new integration uses the repository's MIT license. Existing miniz (MIT) is
reused for ZIP inflation; its packaged notice remains intact. stb_vorbis is pinned
to `nothings/stb` commit `f58f558c120e9b32c217290b80bad1a0729fbb2c`, used under its
MIT option; its notice is included in `tracker/packaging/common/licenses/`.
EXS offsets were investigated using Matt Allan's MIT-licensed
[renoise-exs24](https://github.com/matt-allan/renoise-exs24); its full notice is in
[EXS-parser-LICENSE.md](docs/soundbanks/EXS-parser-LICENSE.md). TinySoundFont (MIT)
was investigated but is not a dependency or a second renderer.

Synthetic test assets are generated by `tools/soundbanks/make_fixtures.py`. The
small real Vorbis fixture is an original compressed FluidR3Mono Gun sample;
its source, license and test construction are recorded alongside the fixtures.
No multi-gigabyte sample collection is committed or added to release packaging.
