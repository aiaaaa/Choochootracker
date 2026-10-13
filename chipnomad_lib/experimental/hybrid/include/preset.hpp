#pragma once
#include "hybrid_synth.hpp"
#include <string>
#include <vector>
namespace hybrid {
enum class PresetProfile { legacyPlugin, hardware };
struct Preset {
    std::string name;
    Patch patch{};
    std::array<uint8_t, 102> raw{}; // preserved for lossless interchange
    bool splitOrLayer = false;
};
Preset decodePreset(const std::array<uint8_t, 102>& bytes, PresetProfile profile = PresetProfile::legacyPlugin);
bool readPresets(const std::string& path, std::vector<Preset>& presets, std::string& error,
                 PresetProfile profile = PresetProfile::legacyPlugin);
bool writePreset(const std::string& path, const Preset& preset, std::string& error);
} // namespace hybrid
