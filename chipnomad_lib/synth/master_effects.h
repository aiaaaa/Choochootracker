#ifndef CHOOCHOO_MASTER_EFFECTS_H
#define CHOOCHOO_MASTER_EFFECTS_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

#include "clouds/dsp/frame.h"
#include "clouds/dsp/fx/reverb.h"

struct Project;

class MasterEffects {
 public:
  void init(float sampleRate);
  void reset();
  bool tailActive() const;
  void process(float* reverbBus, float* delayBus, float* output,
               size_t frames, const Project* project);

 private:
  float lowpass(float input, float coefficient, float& state);
  float lowpassCoefficient(float cutoff) const;
  float highpass(float input, float coefficient, float& inputState, float& outputState);
  float highpassCoefficient(float cutoff) const;

  float sampleRate_;
  uint16_t reverbMemory_[65536];
  clouds::Reverb reverb_;
  std::vector<clouds::FloatFrame> reverbFrames_;
  std::vector<float> delayLine_;
  size_t delayWrite_;
  float reverbFilterState_[2];
  float reverbHighpassInputState_[2];
  float reverbHighpassOutputState_[2];
  float delayFilterState_[2];
  size_t reverbTailFrames_;
  size_t delayTailFrames_;
};

#endif
