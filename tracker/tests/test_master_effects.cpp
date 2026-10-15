#include "doctest.h"
#include "project.h"
#include "synth/master_effects.h"

#include <cmath>
#include <vector>

TEST_CASE("Ping-pong delay follows the project tick duration") {
  MasterEffects* effects = new MasterEffects();
  effects->init(1000.0f);

  Project project;
  projectInit(&project);
  project.tickRate = 100.0f;
  project.delayTicks = 1;
  project.delayFeedback = 50;
  project.delayReturn = 100;
  project.delayFilterCutoffHz = 450;

  std::vector<float> bus(48 * 2, 0.0f);
  std::vector<float> output(48 * 2, 0.0f);
  bus[0] = 1.0f;
  effects->process(nullptr, bus.data(), output.data(), 48, &project);

  CHECK(std::fabs(output[10 * 2]) > 0.1f);
  CHECK(std::fabs(output[20 * 2 + 1]) > 0.01f);
  delete effects;
}

TEST_CASE("Reverb high-pass removes low-frequency send energy") {
  Project project;
  projectInit(&project);
  project.reverbReturn = 100;
  project.reverbTime = 180;
  project.reverbDamping = 255;
  project.reverbFilterCutoffHz = 20000;
  std::vector<float> bus(4096 * 2, 1.0f), lowOutput(4096 * 2, 0.0f), highOutput(4096 * 2, 0.0f);

  MasterEffects low{}, high{};
  low.init(48000.0f);
  high.init(48000.0f);
  project.reverbHighpassCutoffHz = 20;
  low.process(bus.data(), nullptr, lowOutput.data(), 4096, &project);
  project.reverbHighpassCutoffHz = 2000;
  high.process(bus.data(), nullptr, highOutput.data(), 4096, &project);

  float lowEnergy = 0.0f, highEnergy = 0.0f;
  for (int i = 1400 * 2; i < 4096 * 2; ++i) {
    lowEnergy += fabsf(lowOutput[i]);
    highEnergy += fabsf(highOutput[i]);
  }
  CHECK(lowEnergy > highEnergy * 3.0f);
}

TEST_CASE("Beads reverb has its own high-pass send filter") {
  Project project;
  projectInit(&project);
  project.reverbAlgorithm = 1;
  project.reverbReturn = 100;
  project.beadsReverbTime = 180;
  project.beadsReverbTone = 255;
  std::vector<float> bus(12000 * 2, 1.0f), lowOutput(12000 * 2, 0.0f), highOutput(12000 * 2, 0.0f);
  MasterEffects low{}, high{};
  low.init(48000.0f); high.init(48000.0f);
  project.beadsReverbHighpassCutoffHz = 20;
  low.process(bus.data(), nullptr, lowOutput.data(), 12000, &project);
  project.beadsReverbHighpassCutoffHz = 2000;
  high.process(bus.data(), nullptr, highOutput.data(), 12000, &project);
  float lowEnergy = 0.0f, highEnergy = 0.0f;
  for (int i = 7000 * 2; i < 12000 * 2; ++i) { lowEnergy += fabsf(lowOutput[i]); highEnergy += fabsf(highOutput[i]); }
  CHECK(lowEnergy > highEnergy * 3.0f);
}

TEST_CASE("Reset clears an existing delay tail") {
  MasterEffects effects;
  effects.init(1000.0f);
  Project project;
  projectInit(&project);
  project.tickRate = 100.0f;
  project.delayTicks = 1;
  project.delayFeedback = 50;
  project.delayReturn = 100;
  project.delayFilterCutoffHz = 450;
  std::vector<float> bus(12 * 2, 0.0f), output(12 * 2, 0.0f);
  bus[0] = 1.0f;
  effects.process(nullptr, bus.data(), output.data(), 12, &project);
  CHECK(effects.tailActive());
  effects.reset();
  CHECK_FALSE(effects.tailActive());
  std::fill(bus.begin(), bus.end(), 0.0f);
  std::fill(output.begin(), output.end(), 0.0f);
  effects.process(nullptr, bus.data(), output.data(), 12, &project);
  CHECK(std::fabs(output[10 * 2]) < 0.0001f);
}
