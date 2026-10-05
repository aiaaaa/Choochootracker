#pragma once
#include "project_instruments.h"
#include <algorithm>

// Runtime offsets always derive from the saved patch, never from the previous
// frame. 80 is exact identity; the endpoints span the native parameter range.
enum FMMacro { fmTime, fmDecay, fmDetune, fmRatio, fmLFORate, fmLFODepth };
inline int fmMacroDelta(int offset, int span) {
  return offset < 0 ? -((-offset * span + 64) / 128) : (offset * span + 63) / 127;
}
inline int fmMacroValue(int base, int offset, int maximum) {
  return std::clamp(base + fmMacroDelta(offset, maximum), 0, maximum);
}
inline int fmMacroRate(int rate, int maximum, const InstrumentFMTone& tone, bool decay) {
  // Yamaha rate zero means hold; do not turn a held envelope into a decay.
  if (!rate) return 0;
  return std::clamp(rate - fmMacroDelta(tone.macro[fmTime], maximum)
    - (decay ? fmMacroDelta(tone.macro[fmDecay], maximum) : 0), 1, maximum);
}
inline unsigned fmFourOpCarriers(unsigned algorithm) {
  constexpr unsigned masks[] = {8,8,8,8,10,14,14,15};
  return masks[algorithm & 7];
}
inline unsigned fmOPLCarriers(const InstrumentOPL& p) {
  constexpr unsigned masks[] = {8,9,10,13};
  if (p.topology == OPLTopology::fourOperator) return masks[p.connection[0] | (p.connection[1]<<1)];
  return (p.connection[0]?3:2) | (p.topology==OPLTopology::dualVoice?(p.connection[1]?12:8):0);
}
inline int fmNativeDetune(int base, int offset, int op) {
  int signedBase = (base & 4) ? -(base & 3) : base & 3;
  int value = std::clamp(signedBase + (op & 1 ? -1 : 1)*fmMacroDelta(offset,3),-3,3);
  return value < 0 ? 4-value : value;
}
