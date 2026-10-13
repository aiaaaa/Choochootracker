#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace hybrid {
struct WaveRegion {
    uint32_t address = 0;
    uint8_t control = 0;
    int8_t transpose = 0;
    uint8_t fine = 0;
    bool oneShot = false;
};
struct WaveInfo {
    std::string name;
    bool experimental = false;
    std::array<WaveRegion, 16> regions{};
};
// Immutable during playback. Load on the control thread before binding to a synth.
// Contains user-supplied sample data, never compiled-in ROM data.
class WaveBank {
public:
    bool load(const std::string& path, std::string& error);
    size_t size() const { return waves_.size(); }
    size_t stockCount() const { return stockCount_; }
    size_t romSize() const { return rom_.size(); }
    const WaveInfo* wave(unsigned id) const;
    const WaveRegion* region(unsigned id, int key) const;
    uint8_t byte(uint32_t address) const;
private:
    uint32_t stockCount_ = 0;
    std::vector<WaveInfo> waves_;
    std::vector<uint8_t> rom_;
};
} // namespace hybrid
