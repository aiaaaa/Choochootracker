#include "wave_bank.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace hybrid {
bool WaveBank::load(const std::string& path, std::string& error) {
    try {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) throw std::runtime_error("Cannot open wave bank: " + path);
        const auto length = file.tellg();
        if (length < 24 || length > 1024 * 1024) throw std::runtime_error("Invalid bank size");
        file.seekg(0);
        std::vector<uint8_t> data(static_cast<size_t>(length));
        if (!file.read(reinterpret_cast<char*>(data.data()), length)) throw std::runtime_error("Cannot read bank");
        size_t pos = 0;
        auto get = [&](unsigned bytes) {
            if (bytes > data.size() - pos) throw std::runtime_error("Truncated bank");
            uint32_t value = 0;
            for (unsigned i = 0; i < bytes; ++i) value |= uint32_t(data[pos++]) << (8 * i);
            return value;
        };
        const std::string magic(data.begin(), data.begin() + 8);
        if (magic != "HYBWAV01") throw std::runtime_error("Unknown wave bank format");
        pos = 8;
        const auto count = get(4), stock = get(4), romBytes = get(4), reserved = get(4);
        if (count != 256 || (stock != 75 && stock != 32) || reserved ||
            (romBytes != 262144 && romBytes != 65536) ||
            data.size() != 24 + count * (16 + 16 * 8) + romBytes)
            throw std::runtime_error("Invalid bank metadata");
        WaveBank next;
        next.stockCount_ = stock;
        next.waves_.resize(count);
        for (unsigned id = 0; id < count; ++id) {
            auto& info = next.waves_[id];
            for (int i = 0; i < 16; ++i) {
                const auto c = get(1);
                if (c >= 32 && c < 127) info.name.push_back(static_cast<char>(c));
                else if (c != 0) throw std::runtime_error("Invalid wave name");
            }
            info.experimental = id >= stock;
            for (auto& r : info.regions) {
                r.address = get(4);
                r.control = static_cast<uint8_t>(get(1));
                const auto semi = get(1);
                r.transpose = static_cast<int8_t>(semi < 128 ? int(semi) : int(semi) - 256);
                r.fine = static_cast<uint8_t>(get(1));
                const auto mode = get(1);
                r.oneShot = mode != 0;
                const auto window = 256u << ((r.control >> 3) & 7);
                if (mode > 1 || r.address >= romBytes || r.address % window != 0)
                    throw std::runtime_error("Invalid wave region");
            }
        }
        next.rom_.assign(data.begin() + static_cast<std::ptrdiff_t>(pos), data.end());
        *this = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) { error = e.what(); return false; }
}
const WaveInfo* WaveBank::wave(unsigned id) const {
    return id < waves_.size() ? &waves_[id] : nullptr;
}
const WaveRegion* WaveBank::region(unsigned id, int key) const {
    const auto* info = wave(id);
    return info ? &info->regions[static_cast<unsigned>(std::clamp(key, 0, 127)) / 8] : nullptr;
}
uint8_t WaveBank::byte(uint32_t address) const {
    return rom_.empty() ? 0 : rom_[address % rom_.size()];
}
} // namespace hybrid
