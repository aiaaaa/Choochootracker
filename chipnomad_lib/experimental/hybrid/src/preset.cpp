#include "preset.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace hybrid {
namespace {
int signed8(unsigned x) { return x < 128 ? int(x) : int(x)-256; }
int signed7(unsigned x) { x &= 127; return x < 64 ? int(x) : int(x)-128; }
Source modSource(unsigned x) { return static_cast<Source>(x & 15); }
}
Preset decodePreset(const std::array<uint8_t, 102>& b, PresetProfile profile) {
    Preset result; result.raw = b;
    for (size_t i = 0; i < 6; ++i) result.name += b[i] >= 32 && b[i] < 127 ? char(b[i]) : '?';
    auto& p = result.patch;
    for (size_t i = 0; i < 4; ++i) {
        const size_t n = 6 + 10*i;
        auto& e = p.envelopes[i];
        for (size_t j = 0; j < 3; ++j) e.levels[j] = signed8(b[n+j]) / 2.0f;
        for (size_t j = 0; j < 4; ++j) e.times[j] = b[n+3+j] & 63;
        e.secondRelease = (b[n+6] & 128) != 0;
        e.velocity = b[n+7] >> 2;
        // The measured legacy importer drops the hardware exponential flag.
        e.exponentialVelocity = profile == PresetProfile::hardware && (b[n+7] & 1) != 0;
        e.attackVelocity = b[n+8] & 63; e.keyDecay = b[n+9] & 63;
    }
    for (size_t i = 0; i < 3; ++i) {
        const size_t n = 46 + 4*i;
        auto& l = p.lfos[i];
        l.speed = b[n] & 63; l.shape = static_cast<LfoShape>(b[n] >> 6);
        l.startLevel = b[n+1] & 63; l.endLevel = b[n+2] & 63;
        // The legacy importer reverses these pairs relative to the hardware manual.
        l.depthSource = profile == PresetProfile::legacyPlugin
            ? modSource(((b[n+1] >> 6) << 2) | (b[n+2] >> 6))
            : modSource((b[n+1] >> 6) | ((b[n+2] >> 6) << 2));
        l.delay = b[n+3] & 63; l.reset = (b[n+3] & 128) != 0; l.humanize = (b[n+3] & 64) != 0;
    }
    for (size_t i = 0; i < 3; ++i) {
        const size_t n = 58 + 10*i;
        auto& o = p.oscillators[i];
        o.semitones = float(b[n] & 127) - 36 + (b[n+1] >> 3) / 32.0f;
        o.wave = b[n+5]; // never clamp to the stock bank or renumber extensions
        o.enabled = (b[n+6] & 128) != 0; o.level = (b[n+6] & 127) / 2.0f;
        for (size_t j = 0; j < 2; ++j) {
            o.pitchMods[j] = {modSource(b[n+2] >> (4*j)), signed8(b[n+3+j]) / 2.0f};
            o.levelMods[j] = {modSource(b[n+7] >> (4*j)), signed8(b[n+8+j]) / 2.0f};
        }
    }
    p.volume = (b[88] & 127) / 2.0f; p.am = (b[88] & 128) != 0;
    p.cutoff = b[89] & 127; p.sync = (b[89] & 128) != 0; p.resonance = b[90] & 31;
    p.filterMods[0] = {modSource(b[91]), float(signed7(b[92]))};
    p.filterMods[1] = {modSource(b[91] >> 4), float(signed7(b[93]))};
    p.voiceRestart = (b[92] & 128) != 0; p.mono = (b[93] & 128) != 0;
    p.filterKeyTrack = (b[94] & 127) / 2.0f; p.envelopeRestart = (b[94] & 128) != 0;
    p.glide = b[95] & 63; p.oscillatorRestart = (b[95] & 128) != 0;
    p.pan = b[100] >> 4; p.panMod = {modSource(b[100]), float(signed7(b[101]))};
    p.fullCycle = (b[101] & 128) != 0;
    result.splitOrLayer = ((b[97] | b[98] | b[99]) & 128) != 0;
    return result;
}
bool readPresets(const std::string& path, std::vector<Preset>& out, std::string& error, PresetProfile profile) {
    try {
        std::ifstream f(path, std::ios::binary | std::ios::ate);
        if (!f) throw std::runtime_error("Cannot open preset file: " + path);
        const auto length = f.tellg();
        if (length <= 0 || length > 4*1024*1024) throw std::runtime_error("Invalid preset file size");
        f.seekg(0); std::vector<uint8_t> bytes(static_cast<size_t>(length));
        if (!f.read(reinterpret_cast<char*>(bytes.data()), length)) throw std::runtime_error("Cannot read preset file");
        std::vector<Preset> result;
        if (bytes.size() == 102) {
            std::array<uint8_t,102> raw{}; std::copy(bytes.begin(), bytes.end(), raw.begin());
            result.push_back(decodePreset(raw, profile));
        } else {
            size_t pos = 0;
            while (pos < bytes.size()) {
                if (bytes.size()-pos < 6 || bytes[pos] != 0xf0 || bytes[pos+1] != 0x0f || bytes[pos+2] != 0x02 || bytes[pos+3] > 15)
                    throw std::runtime_error("Not a supported hardware program SysEx message");
                const unsigned command = bytes[pos+4];
                const size_t programs = command == 1 ? 1 : command == 2 ? 40 : 0;
                if (!programs) throw std::runtime_error("Unsupported SysEx command (expected program/bank)");
                const size_t size = 6 + programs*204;
                if (size > bytes.size()-pos || bytes[pos+size-1] != 0xf7) throw std::runtime_error("Truncated program SysEx");
                size_t at = pos+5;
                for (size_t p = 0; p < programs; ++p) {
                    std::array<uint8_t,102> raw{};
                    for (auto& x : raw) {
                        const auto lo = bytes[at++], hi = bytes[at++];
                        if (lo > 15 || hi > 15) throw std::runtime_error("Invalid SysEx nibble");
                        x = static_cast<uint8_t>(lo | (hi << 4));
                    }
                    result.push_back(decodePreset(raw, profile));
                }
                pos += size;
            }
        }
        out = std::move(result); error.clear(); return true;
    } catch (const std::exception& e) { error = e.what(); return false; }
}
bool writePreset(const std::string& path, const Preset& p, std::string& error) {
    std::ofstream out(path, std::ios::binary);
    const uint8_t header[] = {0xf0,0x0f,0x02,0x00,0x01};
    out.write(reinterpret_cast<const char*>(header), 5);
    for (auto x : p.raw) { out.put(char(x & 15)); out.put(char(x >> 4)); }
    out.put(char(0xf7)); out.close();
    if (!out) { error = "Cannot write preset"; return false; }
    error.clear(); return true;
}
} // namespace hybrid
