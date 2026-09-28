# Linux handheld / retro console constraints

Read this before changing performance, audio buffers, filesystem paths,
fullscreen/resolution, input timing, or anything justified by "it works on
desktop". The product is a tracker you play on a small Linux handheld.

## Target machine

Primary: Anbernic RG353V, Rockchip RK3566, aarch64, ArkOS, PortMaster.

Treat other PortMaster devices as "should work" until measured. TrimUI has
been used. RG35xx and Miyoo Mini are leftover ChipNomad SDL1.2 targets, not
the current design center.

Desktop Windows exists to iterate. Desktop x86 timings do not predict ARM64
console load.

## What the hardware gives you

Capabilities that the app relies on:

- SDL2 from firmware
- stereo audio out
- D-pad, ABXY, shoulders, analog sticks
- landscape 4:3-class panel, far above 320x240 (`port.json` requires `!lowres`)
- enough RAM for eight voices, Clouds reverb, and short PCM in memory
- SD card for projects/samples (slow, removable, weird mount paths)

What it does not give you:

- spare CPU for eight heavy Plaits-Alt engines plus audio-rate modulation
  plus nonlinear filters plus Clouds at 96 kHz
- guaranteed low-latency audio (buffer is large by default on purpose)
- a writable UNIX home you can trust without PortMaster bind mounts
- mouse, keyboard, or precise touch as the primary UI
- OpenGL UI, plugin hosts, or streaming from SD for sample playback

## CPU and audio deadline

Master mix is 48 kHz. That change is the main handheld performance win.
September 2026 RG353V measurements (`docs/performance-optimization-2026-09-01.md`):

- `alf dance.cct`: about 8% CPU, 10% peak
- `psy.cct`: about 21%

Earlier 96 kHz peaks above 50% caused crackles and UI slowdown.

In-app Mixer `CPU nnn%` is smoothed callback compute time versus buffer
duration. It does not include scheduler gaps. Still the number to watch on
device.

Rules:

- Profile complete songs, not isolated oscillators.
- Silent voices must skip DSP.
- Shared reverb/delay run once per callback, not per track.
- FLFO / audio-rate modulation forces per-sample voice updates. Treat as
  expensive until measured.
- Driven Tilt and Aggro/Acid filters cost more than Clean.
- Do not add SIMD or `expf` bit hacks without a measured hotspot. Recursive
  filters resisted SIMD; 48 kHz plus hoisting won.
- Desktop `benchmark-song` is for A/B only. Finish on the console meter.

If a new engine cannot meet eight voices + Reverb + Delay on RG353V, it may
still ship, but the UI should warn. Do not silently drop voices.

## Audio buffer

Default `audioBufferSize` is 4906 frames at 48 kHz (~102 ms) on native
builds. Web uses 1024. Settings persist in `settings.txt`.

Large buffers hide CFW scheduling. Small buffers make scopes and table
cursors smoother (Steam Deck note: 512 frames ~10.7 ms looked fluid; 4906
looked jumpy at 60 FPS overlay). Do not change the default without a
PortMaster device test. If a device needs another size, document it as a
settings.txt tweak, not a silent code change.

Callback larger than reserved buffers: do not realloc on the audio thread.
See `docs/agents/AUDIO.md`.

## Display

Tracker UI is a 40 column x 20 row character grid, historically a 640x480
logical canvas. Handhelds use the desktop display mode and scale the grid
with bitmap fonts (`font12x16` through `font48x54`).

Implications:

- No proportional fonts, no wrapping essays, no extra chrome.
- One status line at row 19. Screen map occupies the right strip
  (cols 34-39).
- 60 FPS draw, but audio callback length can make play cursors jump.
- Color themes are 10 RGB slots. Keep contrast high on a small backlit panel.

Do not introduce a second layout for "just handheld". The same 40x20 UI runs
everywhere. Android is the exception for touch overlay outside the canvas.

## Input reality

Buttons are chunky. Holds and chords are the language (SELECT+direction,
EDIT+direction, OPT+A clear).

Rules proven on RG353V:

- Ignore SDL key repeat. Use `keyRepeatDelay` / `keyRepeatSpeed` in settings.
- Do not drop short presses during chords. Held directions stay in the mask.
- Double-tap window is `doubleTapFrames` (default 20 frames).
- gptokeyb makes buttons look like a keyboard. Analog sticks are SDL axes.
- L1/L2/R2 are live/record/erase. They must remain available on shoulders.

If a change makes SELECT+D-pad navigation miss on device, it is a blocker
even if Windows is fine.

## Storage and paths

PortMaster cwd is the port directory. `fileGetDefaultDirectory` is `getcwd()`.
`bind_directories` maps `~/.choochootracker` onto `ports/choochootracker/conf/.choochootracker`.

Assume:

- SD cards are slow. Do not load big WAVs on the audio thread.
- Samples live in RAM. No streaming. One-shots and short wavetables only.
- Paths in `.cct` may be invalid on another device. Missing sample = silence
  + warning, never a crash.
- Autosave every minute. Storage can fail; keep save code boring and sync.
- Do not require writes outside the port folder.

PCM in RAM is the design. Long audio tracks and SD streaming are out of
scope (`docs/dev_readme.md`).

## Memory

Eight Voice objects, Plaits ~16 KiB work allocator per track, Clouds 64 KiB
reverb memory, mix/delay buffers sized to the audio callback, plus user
samples. RK3566 RAM is not the scarce resource. CPU and cache are.

Still: no per-callback `vector` growth, no sample reload while rendering,
no unbounded undo history.

## Other consoles

| Device class | Status |
| --- | --- |
| RG353V ArkOS PortMaster | design target |
| Other aarch64 PortMaster | expected if `!lowres` and glibc new enough |
| TrimUI | SDL init workaround exists |
| Steam Deck Linux | experimental; buffer 512 was a local test |
| RG35xx / Miyoo SDL1.2 | legacy ChipNomad packaging |
| Android handhelds | separate touch overlay, AAudio policy |

Do not encode RG35xx j2k keycodes or Miyoo Docker into new features.

## Validation on hardware

Minimum session after audio, input, or timing changes:

1. Install zip via PortMaster or copy to `ports/`.
2. Start, D-pad through MSCPIT, play, stop, save, exit.
3. Hybrid song: AY + Braids/Plaits + Sample, Reverb and Delay up.
4. Watch Mixer CPU. Listen for clicks on note retrigger and live edits.
5. Motion record a stick gesture, save, reload.

Unit tests cannot replace this.

## Caveats log

### 2026-09-01 — 96 kHz mix overruns the console

- Symptom: crackles, UI stalls, CPU peaks over 50%
- Cause: master mix, filters, tilt, sends at 96 kHz
- Rule: 48 kHz master; Braids may stay 96 kHz with half-band decimation
- Files: `docs/performance-optimization-2026-09-01.md`
- Status: fixed

### 2026-09 — Default 4906-frame buffer is latency vs stability

- Symptom: waveform/table cursors jump even at 60 FPS
- Cause: each callback covers ~102 ms of audio
- Rule: do not silently lower default buffer; document per-device tweaks
- Files: `tracker/src/common.cpp` `initDefaultAppSettings`
- Status: open

### 2026-08 — Short presses on RG353V

- Symptom: chords ate the D-pad tap
- Cause: repeat/combination handling
- Rule: internal repeat only; preserve held directions
- Files: `tracker/src/app.cpp`
- Status: fixed
