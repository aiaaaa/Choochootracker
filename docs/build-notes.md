# Build notes

Run all commands from the repository root unless stated otherwise.

## Windows

Use MSYS2 UCRT64:

```sh
cd tracker
make -j4 windows
```

The executable and bundled files are written to `tracker/build/windows/`.

The ChooChooPlayer visualizer uses the same Windows toolchain:

```sh
cd tracker
make -j4 choochooplayer
```

Its self-contained package is written to `choochooplayer/build/windows/`.
Run `launch-alf-dance.bat` there to preview the bundled `alf dance.cct` project.

## Linux / Steam Deck (x86_64)

For a native local test, use the existing Ubuntu-24.04 WSL distribution with
`g++`, `make`, `libsdl2-dev`, and `libasound2-dev` (for MIDI I/O, see below)
installed. From `tracker` in that distribution:

```sh
make -j4 -f Makefile.linux linux-package \
  COMMON_CFLAGS='-std=c++17 -Wall -g -Os -DTEST' VERSION=1.0.9-deck-test
ldd build/linux/choochootracker
tar -tzf build/linux/ChooChooTracker-$(date +%Y-%m-%d)-1.0.9-deck-test-Linux-x86_64.tar.gz
```

The archive includes the executable, runtime assets, manual, and licenses.
SDL2 and its system dependencies must be available on the target. A build
against Ubuntu 24.04 is a local compatibility test, not a Steam Linux Runtime
compatibility guarantee; check dependencies and playback on the target Deck.

Use SteamOS Devkit Client to pair the Deck and upload the extracted package.
Set the launch command to `./chipnomad.sh` and disable Steam Play for this
native build. The existing complete Windows package can also be tested with
Steam Play enabled and `./choochootracker.exe` as its launch command. Local
devkit testing does not require uploading a Steam store build.

For the Deck test, set `audioBufferSize: 512` in the extracted application's
`settings.txt` while it is closed, then relaunch. On the tested Deck with
SteamOS 3.8.16 and Proton 11.0, PSY played cleanly and its scope, waveforms,
and Table cursor became fluid at 512 frames. With the default 4906 frames
at 48 kHz, those displays advanced in visible jumps despite a 60 FPS overlay:
the engine processes about 102 ms of audio per callback. At 512 frames this
falls to about 10.7 ms. This observation validates that test configuration;
it does not establish a safe buffer size for every supported device.

### MIDI I/O

Desktop builds (Linux/Windows/macOS) link RtMidi (vendored at
`chipnomad_lib/external/rtmidi`) for realtime MIDI in/out - see
`docs/USER_MANUAL.md`'s MIDI section for what it's used for. Backend
selection is per platform Makefile: `-D__LINUX_ALSA__ -D__LINUX_ALSASEQ__`
plus `-lasound` on Linux, `-D__WINDOWS_MM__` plus `-lwinmm` on Windows,
`-D__MACOSX_CORE__` plus the CoreMIDI/CoreAudio/CoreFoundation frameworks on
macOS. `Dockerfile.linux` installs `libasound2-dev` for the Docker-based
Linux build.

### AppImage

```sh
make -j4 -f Makefile.linux appimage
```

Produces `releases/ChooChooTracker-<date>-<version>-x86_64.AppImage`. Bundles
only `libSDL2` and its `libsamplerate` dependency into `usr/lib` - the two
libraries the tar.gz release's README tells users to `apt install` - and
lets everything else (X11, ALSA, glibc...) resolve from the host, to avoid
the ABI-mismatch risk of bundling core OS libraries into an AppImage.

Requires `appimagetool` on `PATH` - it is not downloaded automatically, since
the only available URL for it is a `continuous` release tag that isn't
pinned to a fixed version, which would make the build not reproducible.
Install it first, e.g.:

```sh
curl -L -o /usr/local/bin/appimagetool https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x /usr/local/bin/appimagetool
```

`fileGetDefaultDirectory()` (`src/corelib/corelib_file.cpp`) detects the
`APPIMAGE` environment variable the AppImage runtime sets and resolves
settings/autosave to `$XDG_DATA_HOME/ChooChooTracker`, falling back to
`~/.local/share/ChooChooTracker` when `$XDG_DATA_HOME` is unset, instead of
next to the executable, since an AppImage mounts itself read-only -
confirmed by running the produced AppImage under Xvfb with an isolated
`$HOME` and checking that `settings.txt`/`autosave.cct` land there rather
than inside the `/tmp/.mount_*` squashfs mount.

`packaging/common/*` (bonus themes, instruments, sample projects, title art)
is bundled read-only under `usr/share/choochootracker/common`. Since the
AppImage itself can't be written to, `AppRun` seeds a writable copy into the
same `$XDG_DATA_HOME`-aware directory every time it runs, copying each
top-level bonus directory independently and only if it's missing on the
destination - so an interrupted first run, or a user who already created
their own e.g. `projects` folder before the rest was seeded, still gets the
remaining bonus content on a later launch instead of never seeing it.
`initDefaultAppSettings()` (`src/common.cpp`) points
`projectPath`/`samplePath`/`themePath`/etc at that same directory when
`fileIsRunningFromAppImage()` is true - normal desktop builds keep their
existing behavior (relative paths, resolved via the launcher script's `cd`)
untouched. Confirmed under Xvfb with a fresh `$HOME`: first launch populates
every subfolder, the files are writable, and the Project screen's Load
browser lists the bundled example songs from the seeded (not the read-only)
copy.

## Web

Emscripten is installed locally at `.tmp/emsdk`; do not search for or install
another copy. In PowerShell:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
. .\.tmp\emsdk\emsdk_env.ps1
$env:PATH += ';C:\msys64\usr\bin;C:\msys64\ucrt64\bin'
$empy = $env:EMSDK_PYTHON -replace '\\','/'
$empp = "$env:EMSDK/upstream/emscripten/em++.py"
Set-Location tracker
& 'C:\msys64\usr\bin\make.exe' -j8 -f Makefile.web web-deploy `
  'COMMON_CFLAGS=-std=c++17 -Wall -g -Os -DTEST' `
  "EMXX=$empy $empp"
```

The deploy target updates the checked-in browser bundle in `web/dist/`; Vercel
serves it directly. Commit that directory after every WebAssembly source
change. The bundled SDK requires its own Python, hence the explicit `EMXX`.
If Windows reports `clang++.exe: permission denied`, unblock the bundled
compiler with `Unblock-File -LiteralPath .tmp\emsdk\upstream\bin\clang++.exe`,
verify `clang++.exe --version`, then retry.

## PortMaster (ARM64)

Use the existing Ubuntu WSL2 toolchain:

```sh
cd tracker
make -j4 -f Makefile.portmaster PortMaster-deploy \
  COMMON_CFLAGS='-std=c++17 -Wall -g -Os -DTEST'
```

The package is copied to `releases/choochootracker.zip`. The local build copy
is `tracker/build/portmaster/choochootracker.zip`. The explicit flags avoid a
current GCC 9 LTO internal compiler error. Validate the release archive before
uploading it:

```powershell
& 'C:\Program Files\7-Zip\7z.exe' t releases\choochootracker.zip
```

## Android / Google Play

The Android app is `com.paiheulevrai.choochootracker`, targets API 36, and
uses no storage permissions. It builds both 64-bit and 32-bit ARM libraries.
Install the Android SDK/NDK selected by `ANDROID_HOME` (or set
`ANDROID_NDK_ROOT`). Build each native ABI with MSYS2 UCRT64. This only needs
to happen after native C/C++ changes:

```powershell
$env:ANDROID_NDK_ROOT = "$env:LOCALAPPDATA\Android\Sdk\ndk\30.0.16248370"
& 'C:\msys64\usr\bin\bash.exe' -c `
  'export PATH=/ucrt64/bin:/usr/bin:$PATH; cd /c/Users/<you>/Desktop/choochootracker/tracker; make -f Makefile.android android ARCH=arm64-v8a'
& 'C:\msys64\usr\bin\bash.exe' -c `
  'export PATH=/ucrt64/bin:/usr/bin:$PATH; cd /c/Users/<you>/Desktop/choochootracker/tracker; make -f Makefile.android android ARCH=armeabi-v7a'
```

The native build also needs SDL 2.32.10 headers in
`.tmp/SDL2-2.32.10/SDL2`; retrieve the matching SDL source archive once and
copy its `include/*.h` files into that directory if it is absent.
If an NDK wrapper reports `clang++.exe: Permission denied`, unblock the shared
NDK compiler (not the wrapper) before retrying:

```powershell
Unblock-File -LiteralPath "$env:ANDROID_NDK_ROOT\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe"
```

The first Gradle package build can download its wrapper distribution, so it
needs network access.

For a signed release, create one upload key once. Keep its `.jks` file in a
safe backup and its password in a password manager: losing either prevents
future updates with the same upload identity. Do not commit the key or its
password. `tracker/platforms/android/*.jks` and `keystore.properties` are
ignored by Git.

The build accepts either an ignored `keystore.properties` file or environment
variables. On Windows PowerShell, the latter avoids putting a secret in a
project file:

```powershell
$env:JAVA_HOME = 'C:\Program Files\Android\Android Studio\jbr'
$env:CCT_KEYSTORE_FILE = 'choochootracker-upload.jks' # relative to platforms/android
$env:CCT_KEY_ALIAS = 'choochootracker-upload'
$env:CCT_KEYSTORE_PASSWORD = '<upload-key-password>'
$env:CCT_KEY_PASSWORD = $env:CCT_KEYSTORE_PASSWORD
Push-Location tracker\platforms\android
& "$env:JAVA_HOME\bin\java.exe" -classpath gradle\wrapper\gradle-wrapper.jar `
  org.gradle.wrapper.GradleWrapperMain --no-daemon --console=plain `
  assembleRelease bundleRelease -x buildNativeArm64 -x buildNativeArm32
Pop-Location
```

The `-x` options avoid concurrent/redundant native builds: Gradle only packages
the two libraries produced above. Every bundle uploaded to Play Console needs a
strictly greater `versionCode`; update `versionCode` in
`tracker/platforms/android/app/build.gradle` before each upload. The displayed
`versionName` may stay unchanged for a replacement internal build.

The signed Play artifact is
`tracker/platforms/android/app/build/outputs/bundle/release/app-release.aab`.
It contains `arm64-v8a` and `armeabi-v7a` native libraries plus all bundled
content from `tracker/packaging/common`: projects, samples, instruments,
themes, fonts, AY/SR wavetables, waveforms and title assets. Confirm it before
uploading:

```powershell
& 'C:\Program Files\Android\Android Studio\jbr\bin\jar.exe' tf `
  tracker\platforms\android\app\build\outputs\bundle\release\app-release.aab
```

The same Gradle command produces the signed direct-install APK; install it with
`adb install -r <apk>`. For handoff, copy and name the files outside Gradle's
output directory, for example `releases/ChooChooTracker-1.0-code2-release.aab`.
Upload that AAB to Play Console's Internal testing track. Complete the values
and artwork in `docs/play-store-listing.md` before submission.

For audio traces and the separately installed debug APK, see
[Android audio diagnosis](android-audio-debugging.md).

## Validation

```sh
cd tracker
make -f Makefile.test -j4
```

If MSYS2 reports exit code 127 after `Built: build/tests/run_tests.exe`, run
`build/tests/run_tests.exe` directly; the executable is the authoritative test
result in that environment.

## Optional Mod Lucky personal build

`CHOOCHOO_EXPERIMENTAL_MOD_LUCKY` defaults to `0`. An opt-in native build adds
one Settings row for preparing/playing a random module and importing its PCM
bank. Normal builds have no dependency on libxmp or libcurl. See
[the experiment's build and validation notes](mod-lucky.md) for the pinned
backend, exact macOS commands, ARM prerequisites and test targets.

## Native chip instruments (local development branch)

The branch vendors ymfm (`81aec25ccbb98f4873a255f7551ac4dadac59b4a`, BSD-3-Clause),
emu76489 (`c0fa097060e022db237163d79704025435042997`, MIT), gb_apu
(`3d73d0df027a82d854cacd72a179c2d6a1a9703e`, MIT), and the separately licensed
Apache-2.0 MSFA scalar component from Dexed's `Source/msfa`
(`2e182b3db85c09083ab13c8b9b00565ce7d9ff85`). This does not add Dexed/JUCE, MTS,
Android glue, alternate DX7 cores or their presets. Each vendor directory has
license/provenance records and modifications documented separately. Runtime
notices are under `tracker/packaging/common/licenses/`.

The normal build is offline and includes generated `.cni` presets plus their
catalogue and provenance manifests from `packaging/common/instruments/chips`.
Rebuild the approved data using the canonical C++ serializer:

```sh
make -C tracker -f Makefile.test -j4 chip-factory
python3 tools/chip_banks/convert.py --output tracker/packaging/common/instruments/chips --writer tracker/build/tests/chip_factory
python3 -m unittest discover -s tools/chip_banks -p 'test_*.py'
make -C tracker -f Makefile.test -j4
```

The content tool verifies pinned source hashes and records aliases separately
from distinct parameter patches. OpenDX7 data are parsed as allowlisted literals;
downloaded JavaScript is never executed. Unapproved collections stay outside
shipping assets. DX7's 1,000-sound target is still unmet; the 67-sound starter
and source-specific exclusions are recorded in the DX7 manifest/evidence files.

Optimized offline DX7 measurements (not handheld or hardware underrun results):

```sh
make -C tracker -f Makefile.test -j4 benchmark-native-chips BUILD_DIR=build/chip-optimized CFLAGS='-std=c++17 -Wall -O3 -DNDEBUG -DTEST -DTEST_PORTMASTER_INPUT'
tracker/build/chip-optimized/benchmark_native_chips 30
```

`Makefile.native-chip-ui` builds the production SDL offscreen integration harness.
Run from `tracker/packaging/common` with `SDL_VIDEODRIVER=dummy` and
`SDL_AUDIODRIVER=dummy`; its argument is an existing writable capture directory.
The harness checks preview/cancel/load, local SysEx selection, table/insert
isolation, and sequenced playback with UI drawing. It does not save user settings.
Use the existing local SDL/toolchain configuration; do not install another SDK.
No package has been installed on the handheld by this work.

Genesis/Arcade extend the same ymfm pin with its OPN/OPM/SSG translation units;
no additional player is linked. Original MIT patch recipes are in
`tools/chip_banks/four_op.py`. The canonical factory writer also emits 64
OPLL/VRC7/Sega/GB `.cni` files and `builtins.tsv`. There are 876 packaged native
instruments in total; 812 shared FM catalogue entries represent 704 distinct
normalized FM parameter sets (aliases remain identified). Of these, DX7 has
67 distinct voices, not 1,000. UI/preset files do not control DSP allocation.

Local user conversion, separate from approved factory content:

```sh
python3 tools/chip_banks/import_bank.py my-bank.syx --output my-new-library --writer tracker/build/tests/chip_factory
make -C tracker -f Makefile.test -j4 chip-auditions
cd tracker
build/tests/chip_auditions packaging/common/instruments/chips ../.tmp/chip-audit/auditions packaging/common/projects/native-chip-audition.cct
```

The audition tool generates thirteen bank WAVs, numerical peak/RMS/DC records,
and a thirteen-section song owning its selected patches. Machine checks are not
human listening; `docs/chip-preset-auditions.tsv` records source CNI identities.
Large audio stays ignored under `.tmp/chip-audit/auditions/`.

The optimized benchmark also measures all native families at 1/8/32 voices,
OPL3 four-operator and dual modes, and actual mixed/FM-heavy eight-track songs
with insert FX and sends. Run it from `tracker` so fixture paths resolve.
`--soak` runs a paced 600-second host render while another thread repeatedly
loads a synthetic 10,000-entry metadata index at
`../.tmp/chip-audit/scale-catalog.tsv`. Synthetic entries are not bundled sounds.
This measures host render deadlines, not audio-device underruns or R36H thermals.

The final Yamaha adapters clock one key-off sample before reasserting key-on,
so repeated tracker notes actually retrigger ymfm envelopes. This adds at most
one native sample to the existing streaming FIR delay; it never drops output.


### R36H validation of native chips

Use the existing paired-device GCC 9 compiler and cached development headers,
with the already built ARM64 Mod Lucky prefix. The source can be staged outside
`/roms/ports` and built using `Makefile.personal`; no SDK installation is needed.
GCC requires the documented positional-initializer compatibility patch in gb_apu.
Set `COMMON_CFLAGS` and `MOD_LUCKY_PREFIX` to that configured target environment.

```sh
make -C tracker -j2 -f Makefile.personal PortMaster-deploy
make -C tracker -j2 -f Makefile.test CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1
make -C tracker -j2 -f Makefile.native-chip-device native-chip-validation CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1
# Run the UI fixture from tracker/packaging/common with an existing output dir:
SDL_VIDEODRIVER=offscreen SDL_AUDIODRIVER=alsa ../../build/portmaster/native-chip-ui /absolute/capture-directory
# Run the audio fixture only with the physical audio device available:
SDL_AUDIODRIVER=alsa tracker/build/portmaster/native-chip-audio tracker/packaging/common/projects/native-chip-audition.cct
```

The developer-only audio fixture plays the portable bank demo for 70 seconds,
checks finite/non-silent output, and reports actual SDL callback render timing.
It does not claim subjective listening or measure ALSA underruns directly.
Keep the frontend’s prior state intact when temporarily releasing its audio.
Tests must use the matching target include flags and dependency prefix too.
Retain the existing installed audio settings; benchmark results at smaller
buffers are measurements, not an instruction to change the user's buffer.


PortMaster builds now append `-O3` only for the new native chip cores and their
adapters through `Makefile.native-chip-flags`. The rest of the application keeps
its existing flags. There is no fast-math, oversampling reduction or altered
patch data. Device test/benchmark builds use `NATIVE_CHIP_OPT_FLAGS=-O3` to match
that profile; normal debug tests leave this override empty. The bounded device
sweep is `benchmark_native_chips 30 --bounded`: all 1/4/8/16/32 DX7 raw voice
points, 1/8 for other families, and mixed/FM-heavy/FM-heavy-chord songs with FX.
The ten-minute `--soak` uses the chord scene plus concurrent 10k-index scans.
