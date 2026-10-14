#pragma once
#include "../external/monksynth/voice.h"
#include "../project_instruments.h"
#include "voice_post_processor.h"
#include <cstddef>

// One FOF voice per tracker chord slot. All storage is allocated at startup;
// note changes and rendering never allocate or copy the DSP lookup tables.
class MonkSynthVoice {
 public:
  void init(float sampleRate);
  void configure(const InstrumentMonk* instrument, float pitchCents, float gain,
                 uint16_t cutoffHz, uint8_t resonance);
  void noteOn();
  void noteOff();
  void kill();
  void render(float* output, size_t frames);
  bool active() const { return active_; }
  float envelopeLevel() const { return active_ ? post_.envelopeLevel() : 0.0f; }

 private:
  MonkVoice voice_{};
  VoicePostProcessor<> post_;
  float frequency_ = 440.0f;
  int vowel_ = -1, glide_ = -1;
  bool active_ = false;
};
