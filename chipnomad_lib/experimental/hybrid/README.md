# Hybrid C++ Synthesizer

A standalone C++17 implementation of a three-oscillator hybrid synthesizer,
with ESQ-1/SQ-80-style program compatibility. The engine has no runtime dependency
on a plugin host, emulator, Python, or network service.

## Features

- Eight voices, three sample oscillators per voice, sync and amplitude modulation.
- Four envelopes, three LFOs, statistical noise/humanization and a resonant four-pole filter.
- Adaptive interpolation, mono/polyphonic glide, sustain and voice replacement.
- Per-channel controllers, timed events and allocation-free audio rendering.
- External waveform banks, 102-byte program records and program/bank SysEx import.

## Build and test

Requires CMake 3.22 or newer and a C++17 compiler. No external libraries are required.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Tests generate their own synthetic waveforms and run without external sound assets.

## Render

Supply an external wave bank in the [documented format](WAVE_BANK.md):

```sh
mkdir -p output
./build/hybrid_render /path/to/waves.hwb output/demo.wav
./build/hybrid_render /path/to/waves.hwb output/preset.wav /path/to/presets.syx 0 44100
```

The renderer writes stereo 32-bit float WAV. The optional arguments are a preset
file, zero-based program index and sample rate. Output peaks may exceed unity.
Wave banks and preset collections are not included in this source package.

## Integrate

Link the `hybrid_engine` CMake target and include `hybrid_synth.hpp`.
Load a `hybrid::WaveBank`, attach it with `Synth::setWaveBank`, then set a patch
and pass timestamped events to `Synth::render`. The wave bank must remain alive
and unchanged while attached. Load assets and change configuration outside the
audio callback; serialize access to each synth instance.

`preset.hpp` provides program import and lossless raw preset export. The default
`PresetProfile::legacyPlugin` follows the measured compatibility import behavior;
`PresetProfile::hardware` uses the documented hardware flag/source ordering.
Wave IDs stay stable: the loader accepts 256 entries, with stock counts of 75 or
32 and additional experimental entries beyond that range.

## Scope

This is a synthesis engine and command-line renderer, without a plugin UI or
tracker integration. Compatibility is approximate. Statistical modulation,
some voice transitions, dynamic pan and envelope details can differ from the
comparison target. Split/layer flags are preserved but not rendered.
`writePreset` exports the retained raw program bytes; it does not serialize edits
to the decoded C++ patch.

The implementation was developed through compatibility research and behavioral
testing. See [provenance](PROVENANCE.md) for the development and asset boundaries.
No distribution license has been selected for this source package.
