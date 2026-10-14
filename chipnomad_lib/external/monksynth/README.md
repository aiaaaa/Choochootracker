# MonkSynth vocal DSP

Source: https://github.com/JonET/monksynth
Pinned commit: `53fdf3927e9d75930e92e5c919196d19a3ae7704` (retrieved 2026-10-13).
License: MIT; see LICENSE and the packaged LICENSE.monksynth.txt.

`voice.c` and `voice.h` are byte-for-byte upstream sources. The C++ bridge in
`../../synth/monk_dsp.cpp` exposes the C struct's envelope enumerators at file
scope without modifying the vendored DSP. No plugin SDK is required.

The tracker owns one FOF source per chord slot and supplies its existing filter,
ADSR, pan, insert effects and delay/reverb sends. The upstream plugin shell,
monophonic keyboard stack, ten-voice unison manager and private stereo delay are
not embedded. Chords use independent tracker voices. Render storage is created
before playback; the audio callback does not allocate.

Pitch compensation follows upstream's voice amplitude function with its default
0.1 output gain. Parameters retain the native normalized 0..1 ranges, mapped to
tracker bytes 00..FF. The native glide ramp is only reset when its target or
rate changes; repeated tracker ticks do not restart it.
