Vita package artwork derives from `docs/play-store-assets/ChooChooTracker-play-icon-512.png`.
`icon0.png` is a 128x128 resize. `startup.png` is a 158x158 resize centered
in a 280x158 background matching the app (#050c1f). The 840x500 background
is that solid theme color. No SDK sample identity or artwork is used.
The minimal LiveArea XML follows VitaSDK's documented a1 layout.
These committed small assets avoid an image-tool dependency during builds.

LiveArea files use opaque, non-interlaced **8-bit indexed PNG** (IHDR color type
3). The initial RGB/color-type-2 exports caused installation error 0x8010113D at
99%, even though the ZIP and checksums were valid. `scripts/vita_assets.py` now
checks encoding, dimensions, palette, CRC, bounded decoded rows and XML references
in doctor, build, packaging and artifact verification.

One-time conversion used Pillow 12.0.0, RGB to `quantize(colors=256,
method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)`, then
`save(path, bits=8, optimize=True)`. Pillow is not needed to build this project.
If regenerating artwork, preserve this profile and rerun the workflow tests.

References: [LiveArea format notes](https://gist.github.com/hammerill/64411eebf071b93396b7d310ba8d6776)
and [VitaShell installer report](https://github.com/TheOfficialFloW/VitaShell/issues/312).
