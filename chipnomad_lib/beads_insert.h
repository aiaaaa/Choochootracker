#pragma once
#include <cstdint>

// Owned by an insert slot. All storage and rate-conversion coefficients are
// prepared at engine construction; process/reset never allocate.
class BeadsInsert {
 public:
  explicit BeadsInsert(float sampleRate);
  ~BeadsInsert();
  BeadsInsert(const BeadsInsert&) = delete;
  BeadsInsert& operator=(const BeadsInsert&) = delete;
  bool ready() const { return impl_ != nullptr; }
  void reset();
  void process(float* left, float* right, int frames, const float* parameters);
 private:
  struct Impl;
  Impl* impl_;
};
