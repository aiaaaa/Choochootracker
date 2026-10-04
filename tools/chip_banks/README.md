# Native chip content tools

Factory input is pinned in `manifest.json`. `convert.py` validates hashes and
provenance, parses data without executing source code, deduplicates DX7 parameter
records, records OPL aliases, and calls the production C++ native writer/reloader.
`four_op.py` contains 24 independent original MIT recipes for each of Genesis and
Arcade. DX7 original recipes are separately CC0. Read the notices per source.

From the repository root:

```sh
make -C tracker -f Makefile.test -j4 chip-factory
python3 tools/chip_banks/convert.py --output tracker/packaging/common/instruments/chips --writer tracker/build/tests/chip_factory
python3 -m unittest discover -s tools/chip_banks -p 'test_*.py'
```

Output is 876 CNI files, 812 shared FM index entries plus a 64-entry builtins
inventory. The FM index represents 704 parameter sets with source aliases
identified; DX7 contributes 67 unique sounds. Ten-thousand-entry tests use
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

`chip-auditions` emits thirteen bank WAVs and a portable native song; see the
build notes. `package_desktop.py` packages an existing macOS personal build plus
all assets/notices, preserves the framework's internal symlinks, records hashes,
and verifies the ZIP. It does not build or install on the R36H.
