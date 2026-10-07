# Native chip content tools

Factory input is pinned in `manifest.json`. `convert.py` validates hashes and
provenance, parses data without executing source code, deduplicates DX7 parameter
records, records OPL aliases, and calls the production C++ native writer/reloader.
`four_op.py` contains 24 independent original MIT recipes for each of Genesis and
Arcade. DX7 original recipes are separately CC0. Read the notices per source.

From the repository root:

```sh
make -C tracker -f Makefile.test -j4 chip-factory
python3 tools/chip_banks/convert.py --output tracker/packaging/common/instruments/FACTORY --writer tracker/build/tests/chip_factory
python3 -m unittest discover -s tools/chip_banks -p 'test_*.py'
```

Output is 1,140 CNI files: 1,064 shared FM index entries and a 76-entry simple-chip
inventory. `expansion.py` adds source-pinned emu2413 tone tables, supported WOPN v2
melodic programs from 16-Bit FM Music Station, supported YMulator OPM programs,
and deliberately authored OPLL programs. Full source forms and licenses ship
with the converted data. `expansion-manifest.json` records exclusions, including
unsupported source features, duplicates and silent carrier operators. DX7 still
contributes 67 distinct factory sounds. Ten-thousand-entry tests use
synthetic metadata only. Consult `docs/chip-instruments-report.md` for counts,
measured validation, provenance exclusions and pending listening/device work.

User content never enters the factory manifest implicitly:

```sh
python3 tools/chip_banks/import_bank.py SOURCE.tfi --output NEW_DIRECTORY --writer tracker/build/tests/chip_factory
```

Supported: exact TFI42, original VOPM text fields (0/128 AM enable; no noise or
partial-pan adaptation), WOPLX BANK1, and checksum-validated original DX7 voice/
32-voice/multi-message SysEx. Binary WOPL, arbitrary DMP, four-op Yamaha SysEx,
DX7II/performance extensions, raw unframed dumps and bad-checksum overrides are
not accepted. Conversion stages in a temporary directory and publishes a new
user directory only after native validation. Its manifest makes no licensing
claim about user files. Load its CNI files using the ordinary instrument loader.

Native DX7 users can drop `.syx` files into `instruments/USER/dx7/`, including
subfolders, then choose Bank → USER and open Preset. This requires no offline converter.

`chip-auditions` emits bank WAVs and a portable native song; `--expanded` renders
every preset in the new/simple/OPLL banks in addition to the original samples. See the
build notes. `package_desktop.py` packages an existing macOS personal build plus
all assets/notices, preserves the framework's internal symlinks, records hashes,
and verifies the ZIP. It does not build or install on the R36H.


Factory ZIP packaging: after generating loose factory CNI files, run
`python3 tools/chip_banks/pack_collections.py tracker/packaging/common/instruments/FACTORY`.
The packer preserves CNI bytes, includes source notices, verifies every entry,
then updates both catalogs and removes only the verified loose generated files.
It can also repack existing ZIP-backed catalogs. Do not run it on personal USER
libraries or add user-provided banks to factory packages.
