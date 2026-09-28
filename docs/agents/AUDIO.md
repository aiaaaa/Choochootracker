# Audio handling and engines

Read this before changing the callback, voices, filters, master effects,
sample rates, preview, export, or a new synth family.

## Source of truth

| Piece | Path |
| --- | --- |
| SDL callback | `tracker/platforms/sdl2/corelib_audio.cpp` |
| App audio manager | `tracker/src/audio_manager.cpp` |
| Render / mix | `chipnomad_lib/chipnomad_lib.cpp` `chipnomadRender` |
| Sequencer | `chipnomad_lib/playback.cpp` |
| AY chip | `chipnomad_lib/chips/` |
| Voices | `chipnomad_lib/synth/*_voice.*` |
| Shared filter/VCA | `chipnomad_lib/synth/voice_post_processor.h`, `multimode_filter.*`, `envelope.*` |
| Master FX | `chipnomad_lib/synth/master_effects.*` |
| Tilt EQ | `chipnomad_lib/synth/track_tilt.*` |
| Realtime rules | `docs/realtime-architecture.md` |
| 48 kHz rationale | `docs/performance-optimization-2026-09-01.md` |

## Signal path

```text
SDL callback (int16 stereo)
  audio_manager::audioCallback
    chipnomadRender(float stereo)          // song
    preview voices mixed in                // file browser audition
    float -> int16 with clip
```

Inside `chipnomadRender`, each block:

1. If tick boundary: adopt snapshot, Stop, settings, commands, live sticks,
   `playbackNextFrame`, motion events, update voices, publish status.
2. If audio-rate modulation is active, force 1-sample chunks.
3. Clear mix, reverb bus, delay bus (preallocated).
4. Render AY tracks that actually use AY1/AY2/AYSample.
5. Render modern voices (skip inactive).
6. Per-track volume, tilt, clip detect, reverb/delay sends.
7. MasterEffects once, then mixVolume, overload flag.

AY path is register emulation (`SoundChipAY` / ayumi). Modern engines bypass
AY. Empty or non-AY tracks must not run AY render (historical bug: treating
every instrument as AY).

## Rates and formats

| Domain | Rate / format |
| --- | --- |
| Master mix, Plaits, Sample, SCWF, BYOWTBL, aChChid, drums, MME, Sintered, filters, tilt, sends | 48 kHz float stereo |
| Braids oscillator | 96 kHz, 31-tap half-band FIR to 48 kHz |
| Plaits inner oversampled models | keep upstream decimator inside the engine |
| Clouds reverb | topology from 32 kHz design; delays and LFOs scaled to 48 kHz |
| Callback output | S16 stereo |
| Export | 16-bit stereo WAV / stems |
| Default buffer | 4906 frames native, 1024 web |

`settingsLoad` migrates old `audioSampleRate` so a leftover 96 kHz settings
file cannot undo the mix-rate change.

New engines must declare native rate: accept 48 kHz, rescale time constants,
or convert explicitly. Do not quietly assume 96 kHz.

## Callback rules

- No malloc, no vector growth, no file I/O, no mutex.
- `chipnomadReserveRenderBuffers` before `audioSetup`.
- If `stereoSamples > aBufferSize`: zero output, set render overflow, return.
- Preview uses its own reserved float buffer, mixed after the song render.
- CPU meter: elapsed ns vs buffer duration, 7/8 smoothing, cap 999.

Preview is PCM / AY / SCWF / BYOWTBL only, for the file browser. It is not a
second song engine. `stopSamplePreview` on cancel.

## Voice contract

One monophonic voice object per track per family, always allocated, idle
when unused.

Common methods: `init(sampleRate)`, `configure(...)`, `noteOn`, `noteOff`,
`kill`, `render`, `active()`, `envelopeLevel()`.

Tick-rate `update*Voices` copies instrument + FX + modulation into the voice,
then `applyVoiceEvents` turns tracker note on/off/kill/strike into voice
calls.

`VoicePostProcessor`: source -> multimode filter -> VCA envelope -> gain.
Percussive Braids models and Plaits TRIG/LPG keep internal envelopes; do not
stack a second ADSR on those paths.

Filter characters: 0 Off, 1 Clean, 2 Classic, 3 Aggro, 4 Acid. Modes LP/BP/HP.
Slope 12 dB (one SVF) or 24 dB (two stages). Cutoff 20 Hz–20 kHz exponential.
Resonance 00-FF, used squared in the processor.

Envelope shape 00-FF: log through linear (0x80) to exponential. Times are
squared 0-5 s (`envelopeTime` in `chipnomad_lib.cpp`).

VCA retrigger continues from current level (avoids clicks). Plaits TRIG holds
Mutable gate high for the note; a repeated note inserts one low block then
high so rising-edge engines retrigger. Legacy Plaits `LEVEL` loads as VCA.

## Engine map

| Type | Wrapper | Rate | Post | Notes |
| --- | --- | --- | --- | --- |
| AY1 / AY2 / AYSample | `SoundChipAY` | 48 kHz mix | no | One chip instance per track. AYSample: 8-bit mono, max 16384, 4-bit volume |
| Braids | `BraidsVoice` | 96->48 | yes | 47 models 0-46. Percussive STRIKE vs tonal ADSR. App-wide BITS/DRFT/SIGN |
| Plaits | `PlaitsVoice` | 48 | yes | 24 stock engines. IDs frozen. TRIG vs VCA |
| Plaits-Alt | `PlaitsAltVoice` | 48 | yes | Separate registry. Do not reuse stock engine IDs |
| Sample | `SampleVoice` | 48 | yes | PCM8/16 RAM, interp, loop/pp, no streaming |
| SCWF | `SCWFVoice` | 48 | yes | Dual single-cycle osc, detune, mix |
| BYOWTBL | SCWF + loader | 48 | yes | Serum tables, frame index A/B |
| aChChid | `AChChidVoice` | 48 | 303 filter | Open303; optional Braids as VCO |
| DrumSynth (Bogie) | `DrumSynthVoice` | 48 | yes | 12 VA/FM models |
| MME | `MMEVoice` | 48 | yes | Dual osc, Warps-inspired algos |
| Sintered | `SinteredVoice` | 48 | yes | Experimental digital percussion |

Vendored DSP: `chipnomad_lib/external/mutable/` (frozen), `external/ayumi`,
`external/open303`. Change wrappers, not snapshots, unless a tested DSP bug
or compile fix. Braids pin and stmlib pin: `docs/dev_readme.md`.

Plaits-Alt firmware UI sources are excluded from the tracker build.

## Mixing and sends

Per track:

- volume 0-100 (fader)
- instrument volume 00-FF (pre fader)
- mute/solo via `audioManager.trackStates` queued as `trackEnabled`
- tilt EQ
- reverb send, delay send (post fader)

Master:

- Clouds reverb: return, time, damping, filter
- ping-pong delay: return, ticks (tempo sync), feedback, filter, send-to-reverb
- global `mixVolume`

Mono voices are copied to both channels with a 0.25 scale in
`renderMonoVoiceTracks`. Stereo sample/SCWF paths use interleaved mixBuffer.

Clip flags per track (`trackClipping`) and master `audioOverload` feed UI
warning colors. Auto Mix (`chipnomadAutoMix`) renders offline and proposes
fader values; it is not the live callback.

## Modulation vs audio

Four slots per instrument: ADSR, AHD, LFO, SLFO (tempo), FLFO (audio rate),
stick linear / stick rate.

FLFO and audio-rate dests set `hasAudioRateModulation`, which splits render
to one sample. Measure before using FLFO on many tracks.

Sticks: SDL axes -> `chipnomadSetLiveStickAxes`. Enabled while L1/L2/R2 held.
Motion record: audio emits `{phrase,row,fx,value,erase}`; UI writes FX cells.
Queue overflow sets `!` on the message row. Never write phrases from audio.

Instrument FX (BMD, PMD, SPT, ...) are absolute P-locks until next trigger.
`SLE` glides engine parameters. Keep FX IDs appended; see CORE_ARCHITECTURE.

## Gain and loudness

Do not normalize every model to the same RMS. Sustained osc, physical models,
and percussion have different crest factors. Use trimmed median RMS, cap with
95th percentile peak and 1 dB headroom. Never set a family gain from the raw
median (`docs/development-notes.md`).

Plaits 6-op silence was a gate bug, not a gain bug. Reed Pipe quiet OUT vs
loud AUX is a sound-design choice (`auxMix` default 0).

## Export

Offline `chipnomadRender` into a local state. WAV mix and stems remain.
PSG/VGM export was removed. Export may allocate; live callback may not.

## Tests

`make -f Makefile.test -j4` from `tracker`. Cover new voices for finite
output, filter stability, retrigger, FX lifetime, and sample-rate conversion.
Song benchmark:

```text
make -f Makefile.test -j4 BUILD_DIR=build/benchmark \
  'CFLAGS=-std=c++17 -Wall -O3 -DNDEBUG -DTEST' benchmark-song
```

Then listen on RG353V with Reverb and Delay.

## Adding an engine (audio-specific)

Also follow CORE_ARCHITECTURE's subsystem list.

- Keep `render` allocation-free.
- Skip work when `!active()`.
- Plug into the matching `renderMonoVoiceTracks` or `renderStereoVoiceTracks`.
- Reuse `VoicePostProcessor` unless the engine's identity is its own filter
  (aChChid 303).
- Expose cutoff/reso/model as FX with new IDs at the end of `enum FX`.
- If inner rate != 48 kHz, document the converter next to the voice.

## Caveats log

### 2026-08 — Non-AY tracks still ran AY

- Symptom: invalid register work, wasted CPU, wrong muting
- Cause: any non-empty instrument treated as AY
- Rule: AY render only for AY1, AY2, AYSample
- Files: `chipnomad_lib/chipnomad_lib.cpp` `renderChipTracks`
- Status: fixed

### 2026-08 — Plaits one-block trigger

- Symptom: 6-op FM and gate-dependent models nearly silent
- Cause: wrapper emitted a one-block trigger instead of holding gate
- Rule: hold gate for note duration; one low block only to retrigger
- Files: `chipnomad_lib/synth/mutable_voice_base.h`
- Status: fixed

### 2026-09-01 — tanh in the sample loop

- Symptom: nonlinear filter/tilt CPU
- Cause: `tanhf` per sample
- Rule: 1025-entry tanh table; do not bring Schraudolph exp into coefficients
- Files: `chipnomad_lib/synth/audio_math.*`
- Status: fixed

### 2026-09 — SLE missed BYOWTBL dests

- Symptom: some wavetable params did not glide
- Cause: incomplete destination list
- Rule: SLE must cover detune, mix, table A/B, cutoff, resonance
- Files: playback FX SLE mapping
- Status: fixed

### packaging README still says 96 kHz

- Symptom: agents may restore 96 kHz mix
- Cause: stale PortMaster README
- Rule: master mix is 48 kHz; Braids internal 96 kHz only
- Files: `tracker/packaging/portmaster/README.md`
- Status: open (docs)
