# External wave bank format

`WaveBank::load` reads the `HYBWAV01` format. All multibyte integers are little-endian.
The format contains no compressed data. Supply waveform data and mappings separately
from this source package.

## Header: 24 bytes

| Offset | Type | Value |
| --- | --- | --- |
| 0 | 8 bytes | ASCII `HYBWAV01` |
| 8 | uint32 | Wave count: 256 |
| 12 | uint32 | Stock wave count: 75 or 32 |
| 16 | uint32 | Sample data size: 262144 or 65536 bytes |
| 20 | uint32 | Reserved: zero |

## Wave entries

There are 256 consecutive entries, each 144 bytes: a 16-byte ASCII name padded
with zeroes, followed by sixteen 8-byte keyboard regions. Region zero covers MIDI
keys 0–7, region one 8–15, and so on through keys 120–127.

| Region offset | Type | Meaning |
| --- | --- | --- |
| 0 | uint32 | Sample start address |
| 4 | uint8 | Oscillator control: resolution in bits 0–2; window-size code in bits 3–5 |
| 5 | int8 | Semitone adjustment |
| 6 | uint8 | Fine adjustment, in 1/256 semitone |
| 7 | uint8 | Playback mode: 0 looping, 1 one-shot |

The window length is `256 << ((control >> 3) & 7)`. Start addresses must align
to that window and lie within the sample data. The loader requires the file length
to equal `24 + 256 * 144 + sample_data_size` exactly.

## Sample data

Sample bytes immediately follow the wave entries. Values are unsigned 8-bit:
128 is the midpoint and zero is an oscillator stop marker. The oscillator maps
nonzero bytes with `(value - 128) / 127.0`. Addressing wraps within the loaded
sample memory. Region mappings, pitch corrections and window controls determine
playback; there is no WAV sample-rate field in this format.

See `src/wave_bank.cpp` for validation and `tests/test_engine.cpp` for a fully
procedural test-bank example. The test example contains no hardware waveform data.
