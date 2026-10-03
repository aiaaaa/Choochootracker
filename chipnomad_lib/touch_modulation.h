#pragma once
#include <cstdint>

// Hardware-independent, allocation-free UI-thread contact arbitration.
// Coordinates arrive normalized, top-left origin. Ownership is fixed on down.
// After primary lift/reset, ALL contacts must lift before the next down can arm.
class TouchModInput {
 public:
  enum class Event { down, motion, up };
  bool event(unsigned panel, int64_t id, Event event, float x, float y, bool armed);
  void refresh();
 private:
  struct Panel { int64_t ids[16]{}; unsigned count=0; int64_t primary=0; bool active=false; };
  Panel panels_[2];
  uint32_t generation_=0;
};
// May be invalidated by the UI on reset/project replacement. Audio only reads
// one lock-free packed snapshot per source; no device, mutex or allocation.
void touchModReset();
int16_t touchModOutput(unsigned panel, unsigned parameter, int amount);
