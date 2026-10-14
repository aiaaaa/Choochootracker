#include "monk_voice.h"
#include <algorithm>
#include <cmath>
#include <cstring>

void MonkSynthVoice::init(float sampleRate) {
  sampleRate = monk_sanitize_sample_rate(sampleRate);
  monk_voice_init(&voice_, sampleRate);
  post_.init(sampleRate);
  active_ = false;
  vowel_ = glide_ = -1;
}

void MonkSynthVoice::configure(const InstrumentMonk* p, float pitchCents, float gain,
                               uint16_t cutoffHz, uint8_t resonance) {
  if (!p) { kill(); return; }
  const float hz = 440.0f * std::pow(2.0f, (pitchCents - 6900.0f) / 1200.0f);
  const bool glideChanged = glide_ != p->glide;
  monk_voice_set_glide(&voice_, p->glide / 255.0f);
  // Resetting an unchanged target every tracker tick prevents the native
  // ten-step portamento ramp from ever completing.
  if (hz != frequency_ || glideChanged) monk_voice_set_pitch_target(&voice_, hz);
  if (vowel_ != p->vowel || glideChanged) monk_voice_set_vowel(&voice_, p->vowel / 255.0f);
  frequency_ = hz; vowel_ = p->vowel; glide_ = p->glide;
  monk_voice_set_voice(&voice_, p->character / 255.0f);
  monk_voice_set_aspiration(&voice_, p->breath / 255.0f);
  monk_voice_set_vibrato(&voice_, p->vibrato / 255.0f);
  monk_voice_set_vibrato_rate(&voice_, p->rate / 255.0f);
  post_.setGain(gain);
  post_.setFilter(p->filterEnabled != 0, p->filterCharacter, p->filterMode,
                  p->filterSlope24dB != 0, cutoffHz, resonance / 255.0f);
  post_.setEnvelope(true, p->attack * p->attack / 13005.0f,
    p->decay * p->decay / 13005.0f, p->sustain / 255.0f,
    p->release * p->release / 13005.0f, p->envelopeShape);
}

void MonkSynthVoice::noteOn() {
  monk_voice_note_on(&voice_, frequency_, 1.0f);
  // Native note_on sets the target but does not start its portamento ramp.
  monk_voice_set_pitch_target(&voice_, frequency_);
  post_.noteOn(true); active_ = true;
}
void MonkSynthVoice::noteOff() { post_.noteOff(); }
void MonkSynthVoice::kill() {
  if (active_) monk_voice_reset(&voice_);
  active_ = false; post_.kill();
}

void MonkSynthVoice::render(float* output, size_t frames) {
  if (!output) return;
  // Short, fixed chunks bound work after the tracker envelope ends and keep
  // pitch compensation following a glide without audio-thread allocation.
  while (frames) {
    if (!active_) { std::memset(output, 0, frames * sizeof(float)); return; }
    const size_t n = std::min(frames, size_t(32));
    const float compensation = monk_voice_amplitude(&voice_) * 0.1f;
    monk_voice_process(&voice_, output, uint32_t(n));
    for (size_t i = 0; i < n; ++i) output[i] = post_.process(output[i] * compensation);
    if (!post_.envelopeActive()) kill();
    output += n; frames -= n;
  }
}
