# ChooChooTracker build instructions

The English user manual is available at [`../docs/USER_MANUAL.md`](../docs/USER_MANUAL.md).

Run commands from this `tracker` directory.

## Windows

Requirements: MSYS2 UCRT64, GCC and SDL2.

```bat
build-windows.bat
```

Output: `build/windows/choochootracker.exe`.

## Tests

```sh
make -f Makefile.test
```

The suite covers the tracker engine, Braids, the PCM Sample voice and instrument FX behavior.

## PortMaster / ArkOS

The current target is AArch64, including the Anbernic RG353V. From Ubuntu under WSL2:

```sh
make -j1 -f Makefile.portmaster PortMaster-deploy
```

Outputs:

- `build/portmaster/choochootracker.aarch64`
- `build/portmaster/package/`
- `../releases/choochootracker.zip`

The ZIP contains `ChooChooTracker.sh`, the gamepad mapping, data files, licenses and the ARM64 executable.

Use `-j1` for the release build on the shared Windows/WSL workspace. See
[development notes](../docs/development-notes.md) for toolchain details and
[fork maintenance](../docs/fork-maintenance.md) for upstream policy.

## Web / Vercel

On `personal/r36h`, activate the existing Emscripten SDK, then from the
repository root run:

```sh
python3 tools/web_bundle.py build
python3 -m http.server 8080 --directory web/dist
```

The Web action pins Emscripten 6.0.10 and deploys fresh output from the personal
commit. Do not commit `web/dist`. See [deployment setup and validation](../docs/personal-web-deployment.md)
and [Windows SDK activation](../docs/build-notes.md#web). Native Windows `make`
is not suitable for this target because the Makefile uses Unix commands.
