# PortMaster layer

Read this before changing packaging, the launch script, gptokeyb mapping,
PortMaster metadata, ARM64 flags, or anything that only exists on handheld
Linux distributions that run ports through PortMaster.

## What PortMaster is here

PortMaster is the install/launch layer, not the engine. The engine is a
normal SDL2 ARM64 binary. PortMaster supplies:

- a zip layout the Ports menu understands
- `control.txt` helpers (`get_controls`, `pm_platform_helper`, `pm_finish`)
- gptokeyb, which turns gamepad buttons into keyboard events
- firmware SDL2 (not bundled)
- a writable config bind so saves survive SD card layouts

Primary target: Anbernic RG353V, ArkOS, aarch64. Also reported working on
TrimUI. `port.json` requires `aarch64` and `!lowres`. Do not add an untested
armhf build "for completeness".

## Source of truth

| Piece | Path |
| --- | --- |
| Cross makefile | `tracker/Makefile.portmaster` |
| Launch script | `tracker/packaging/portmaster/ChooChooTracker.sh` |
| gptokeyb map | `tracker/packaging/portmaster/choochootracker.gptk` |
| PortMaster metadata | `tracker/packaging/portmaster/port.json` |
| ES game list | `tracker/packaging/portmaster/gameinfo.xml` |
| Port README | `tracker/packaging/portmaster/README.md` |
| Cover / shot | `tracker/packaging/portmaster/cover.png`, `screenshot.png` |
| Licenses | `tracker/packaging/portmaster/license/` |
| Runtime assets | `tracker/packaging/common/` |
| Compile flags | `-DPORTMASTER_BUILD -DGAMEPAD_SUPPORT -DTOUCH_INPUT` |

Human build notes: `docs/build-notes.md`, `docs/development-notes.md`.

## Package layout

`make -j4 -f Makefile.portmaster PortMaster-deploy` from `tracker` writes
`releases/choochootracker.zip`. Local copy:
`tracker/build/portmaster/choochootracker.zip`.

Zip root:

```text
ChooChooTracker.sh
choochootracker/
  choochootracker.aarch64
  choochootracker.gptk
  port.json
  README.md
  gameinfo.xml
  cover.png
  screenshot.png
  USER_MANUAL.md
  licenses/
  projects/ samples/ instruments/ fonts/ themes/
  AY_wavetables/ SR_wavetables/ waveforms/ title/ pitch-tables/
```

`port.json` `items` must list `ChooChooTracker.sh` and the `choochootracker`
directory. Install path on device is typically
`/<roms>/ports/choochootracker`.

Validate:

```text
unzip -t releases/choochootracker.zip
file tracker/build/portmaster/choochootracker.aarch64
aarch64-linux-gnu-readelf -d tracker/build/portmaster/choochootracker.aarch64
```

Expect a stripped aarch64 ELF, dynamically linked to SDL2, libstdc++, libgcc,
libm, libc. Do not bundle `libSDL2.so`. Firmware/PortMaster provide it.

`port.json` currently sets `min_glibc` 2.29. Older notes mentioned 2.27.
If you change toolchain, update `min_glibc` to the binary's actual
requirement and retest ArkOS.

## Launch script contract

`ChooChooTracker.sh` is the PortMaster entry. It must keep this sequence:

1. Locate PortMaster (`/opt/system/Tools/PortMaster`, `/opt/tools/PortMaster`,
   `$XDG_DATA_HOME/PortMaster`, `/roms/ports/PortMaster`).
2. `source control.txt` and optional `mod_${CFW_NAME}.txt`.
3. `get_controls`.
4. `GAMEDIR="/$directory/ports/choochootracker"`, `CONFDIR="$GAMEDIR/conf"`.
5. `cd "$GAMEDIR"`.
6. Tee stdout/stderr to `log.txt`.
7. `export XDG_DATA_HOME="$CONFDIR"`.
8. `export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"`.
9. `export HOTKEY=guide`.
10. `bind_directories ~/.choochootracker "$CONFDIR/.choochootracker"`.
11. Start gptokeyb with `choochootracker.${DEVICE_ARCH}` and the `.gptk` file.
12. `pm_platform_helper` then run `./choochootracker.${DEVICE_ARCH}`.
13. `pm_finish`.

The binary name uses `${DEVICE_ARCH}`. The makefile currently only produces
`choochootracker.aarch64`. If you add another arch, produce a matching
filename or gptokeyb/pm helper will miss it.

`bind_directories` is why settings and autosave persist. The app's default
directory is `getcwd()` (`corelib_file.cpp`), so the process must start in
`GAMEDIR`. Settings land in `settings.txt` next to the cwd after bind, under
the conf overlay. Do not write to `/roms` as if it were always writable.

## gptokeyb vs SDL gamepad

PortMaster input is keyboard-shaped. `inputInitDefaultKeyMapping` on this
target fills only keyboard slots. Slot 1 gamepad mappings are `none`.

`.gptk` maps:

| Control | Key sent |
| --- | --- |
| D-pad | arrows |
| A | x (Edit) |
| B | z (Opt) |
| Start | space (Play) |
| Select/Back | leftshift (Shift) |
| L1 | q (Stick Live) |
| L2 | w (Motion Record) |
| R2 | e (Motion Erase) |
| analog sticks | `"` (intentionally unused as keys) |

Analog sticks are not keyboard events. Live modulation uses SDL axis events
from the real controller (`appOnEvent` stick axes), while buttons come through
gptokeyb as keys. Do not "fix" stick mappings to WASD. That would steal
navigation or flood key repeat.

Guide is the hotkey (`HOTKEY=guide`) so PortMaster can kill/exit. Do not bind
Guide as a tracker action.

## Compile and SDL caveats

PortMaster CFLAGS add `PORTMASTER_BUILD`, `GAMEPAD_SUPPORT`, `TOUCH_INPUT`.

`PORTMASTER_BUILD` changes:

- SDL init omits haptic and sensor. Some CFW SDL2 builds (TrimUI Brick /
  NextUI) fail `SDL_INIT_EVERYTHING`.
- Key names in settings show A/B/L1 rather than SDLK names.

`GAMEPAD_SUPPORT` still compiles controller open/axis code so sticks work.
`TOUCH_INPUT` compiles the virtual pad used on Android/web; on a real
handheld with a gamepad, `vpadEnabled` turns off when a controller opens.

Release used LTO. GCC 9 on the documented Ubuntu 20.04 WSL toolchain can ICE
on LTO; `docs/build-notes.md` passes
`COMMON_CFLAGS='-std=c++17 -Wall -g -Os -DTEST'` for deploy (no `-flto` in
that override). If you restore LTO, watch for that ICE.

SDL2 pkg-config must use aarch64 libdir. Multiarch APT needs
`[arch=amd64]` on archive.ubuntu.com and `[arch=arm64]` on ports.ubuntu.com.
Missing qualifiers produce 404s on ARM64 indexes.

## Assets the zip must contain

Copy `packaging/common/` wholesale. The tracker expects sibling folders:

- `projects`, `samples`, `instruments`, `fonts`, `themes`
- `AY_wavetables`, `SR_wavetables`, `waveforms`, `title`, `pitch-tables`

Also ship `USER_MANUAL.md` and licenses for ChipNomad, Mutable, ayumi,
Open303, and this project.

Windows-style "just the exe" is invalid for PortMaster. The zip is the
complete port: script + binary + gptk + data + metadata.

## Device test gate

A packaging-only change still needs `unzip -t` on the zip. Behaviour changes
need RG353V: start from Ports, controls, play/stop, save, exit, audio on a
hybrid AY + modern-engine project, glance at Mixer CPU.

`port.json` `rtr` is true (ready to run). There is no extra runtime download.

## Stale text

`tracker/packaging/portmaster/README.md` still says audio runs at 96 kHz.
Master mix is 48 kHz. Prefer `docs/agents/AUDIO.md`. Update the packaging
README when you next touch that file for a release.

## Caveats log

### 2026-08 — SDL_INIT_EVERYTHING fails on some CFW

- Symptom: PortMaster build never opens a window on TrimUI / some SDL2 builds
- Cause: haptic/sensor subsystems missing
- Rule: PortMaster SDL init must exclude `SDL_INIT_HAPTIC` and `SDL_INIT_SENSOR`
- Files: `tracker/platforms/sdl2/corelib_gfx.cpp`
- Status: workaround

### 2026-08 — Analog sticks must stay out of gptokeyb

- Symptom: stick motion would type quotes or move the cursor
- Cause: gptokeyb can only emit keys; tracker wants analog axes
- Rule: keep analog lines as `"` no-op in `.gptk`; use SDL axes for modulation
- Files: `tracker/packaging/portmaster/choochootracker.gptk`
- Status: workaround

### 2026-09 — GCC 9 LTO ICE

- Symptom: PortMaster release compile dies inside LTO
- Cause: toolchain GCC 9 internal compiler error
- Rule: deploy with explicit `COMMON_CFLAGS` without `-flto` if ICE returns
- Files: `docs/build-notes.md`, `tracker/Makefile.common`
- Status: workaround
