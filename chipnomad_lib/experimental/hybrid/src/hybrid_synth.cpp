#include "hybrid_synth.hpp"
#include <algorithm>
#include <cmath>

namespace hybrid {
namespace {
double finite(double x, double lo, double hi, double fallback = 0) {
    return std::isfinite(x) ? std::clamp(x, lo, hi) : fallback;
}
Source source(Source s) { return static_cast<unsigned>(s) <= 15 ? s : Source::off; }
Patch sanitized(Patch p) {
    auto mod = [](Mod& m) { m.source = source(m.source); m.amount = static_cast<float>(finite(m.amount, -127, 127)); };
    for (auto& o : p.oscillators) {
        o.wave = std::min<uint16_t>(o.wave, 255);
        o.semitones = static_cast<float>(finite(o.semitones, -96, 96));
        o.level = static_cast<float>(finite(o.level, 0, 63));
        for (auto& m : o.pitchMods) mod(m);
        for (auto& m : o.levelMods) mod(m);
    }
    for (auto& e : p.envelopes) {
        for (auto& l : e.levels) l = static_cast<float>(finite(l, -63, 63));
        for (auto& t : e.times) t = std::min<uint8_t>(t, 63);
        e.velocity = std::min<uint8_t>(e.velocity, 63);
        e.attackVelocity = std::min<uint8_t>(e.attackVelocity, 63);
        e.keyDecay = std::min<uint8_t>(e.keyDecay, 63);
    }
    for (auto& l : p.lfos) {
        if (static_cast<unsigned>(l.shape) > 3) l.shape = LfoShape::triangle;
        l.speed = std::min<uint8_t>(l.speed, 63); l.delay = std::min<uint8_t>(l.delay, 63);
        l.startLevel = std::min<uint8_t>(l.startLevel, 63); l.endLevel = std::min<uint8_t>(l.endLevel, 63);
        l.depthSource = source(l.depthSource);
    }
    for (auto& m : p.filterMods) mod(m);
    mod(p.panMod);
    p.cutoff = static_cast<float>(finite(p.cutoff, 0, 127, 100));
    p.resonance = static_cast<float>(finite(p.resonance, 0, 31));
    p.filterKeyTrack = static_cast<float>(finite(p.filterKeyTrack, 0, 63));
    p.volume = static_cast<float>(finite(p.volume, 0, 63));
    p.pan = static_cast<float>(finite(p.pan, 0, 15, 8));
    p.masterGain = static_cast<float>(finite(p.masterGain, 0, 1, 1.0));
    p.glide = std::min<uint8_t>(p.glide, 63);
    return p;
}
}
Synth::Synth(double rate) { setSampleRate(rate); setPatch(Patch{}); }
bool Synth::setSampleRate(double rate) {
    if (!std::isfinite(rate) || rate < 8000 || rate > 192000) return false;
    rate_ = rate; reset(); return true;
}
void Synth::setWaveBank(const WaveBank* bank) { bank_ = bank; reset(); }
void Synth::reset() { voices_ = {}; tails_ = {}; channels_ = {}; serial_ = 0; performanceCutoff_=performanceResonance_=performanceLfo1_=-1; }
void Synth::setPerformance(int cutoff,int resonance,int lfo1Rate) {
 performanceCutoff_=std::clamp(cutoff,-1,127);performanceResonance_=std::clamp(resonance,-1,31);performanceLfo1_=std::clamp(lfo1Rate,-1,63);
}
void Synth::setPatch(const Patch& p) { patches_.fill(sanitized(p)); }
void Synth::setChannelPatch(unsigned c, const Patch& p) { if (c < 16) patches_[c] = sanitized(p); }
size_t Synth::activeVoices() const {
    size_t n = 0; for (const auto& v : voices_) n += v.active; return n;
}
void Synth::release(Voice& v) { for (auto& e : v.envelopes) e.release(); }
void Synth::retireForReplacement(Voice& v) {
    if (!v.active) return;
    auto* tail = &tails_[0];
    for (auto& t : tails_) {
        if (!t.active) { tail = &t; break; }
        if (t.serial < tail->serial) tail = &t;
    }
    *tail = v;
    tail->fadeOut = true;
    tail->fadeRemaining = std::max(1u, static_cast<unsigned>(rate_*.005));
    tail->fadeStep = -tail->fade/tail->fadeRemaining;
}
void Synth::start(Voice& v, uint8_t channel, uint8_t key, double velocity, bool legato, bool returning) {
    const bool sameKey = v.used && v.channel == channel && v.key == key;
    const bool wasUsed = v.used;
    const auto savedOscillators = v.oscillators;
    const auto savedEnvelopes = v.envelopes;
    const auto savedLfos = v.lfos;
    const auto newPatch = patches_[channel];
    const bool continuing = !legato && v.active && sameKey && newPatch.voiceRestart;
    const double previousAmplitude = v.amplitude;
    if (!legato) {
        retireForReplacement(v);
        v = Voice{};
        v.oscillators = savedOscillators;
        if (sameKey && !newPatch.envelopeRestart) v.envelopes = savedEnvelopes;
        for (size_t i = 0; i < 3; ++i) {
            v.lfos[i] = savedLfos[i]; v.lfos[i].restart(newPatch.lfos[i].reset);
            if (!wasUsed) v.lfos[i].random = static_cast<uint32_t>((serial_ + 1) * 2654435761u + i * 65537u) | 1u;
        }
        if (continuing) {
            v.amplitude = v.amplitudeTarget = previousAmplitude;
            const unsigned attack = newPatch.envelopes[3].times[0];
            const double seconds = attack==0 ? .0009 : attack==1 ? .0025 : attack==2 ? .0032 : .005;
            v.fade = 0; v.fadeRemaining = std::max(1u,static_cast<unsigned>(rate_*seconds));
            v.fadeStep = 1.0/v.fadeRemaining;
        }
    }
    v.patch = newPatch; v.active = v.held = v.used = true;
    v.channel = channel; v.key = key; v.velocity = velocity; v.serial = ++serial_;
    auto& channelState = channels_[channel];
    const bool glide = v.patch.glide && channelState.lastKey >= 0;
    if (!(returning && legato && glide)) {
        v.glideOrigin = glide ? channelState.lastKey : key;
        v.glideOffset = 0;
        v.pitch = v.glideOrigin;
        const double glideTicks = std::max(1.0, std::round(envelopeSeconds(v.patch.glide) * 100));
        v.glideStep = glide ? static_cast<float>((key-v.glideOrigin)/glideTicks) : 0;
    }
    v.notePressure=0;
    channelState.lastKey = key;
    if (!returning) v.controlPhase = 1;
    v.dc.configure(rate_);
    if (!legato) for (size_t i = 0; i < 4; ++i)
        v.envelopes[i].start(v.patch.envelopes[i], rate_, velocity, key, v.patch.fullCycle,
                             !(sameKey && !v.patch.envelopeRestart));
    for (size_t i = 0; i < 3; ++i) {
        const auto& o = v.patch.oscillators[i];
        if (bank_) v.oscillators[i].configure(*bank_, o.wave, key + int(std::floor(o.semitones)), o.semitones,
                                           !legato && (v.patch.oscillatorRestart || !wasUsed || v.patch.glide));
    }
    inputSources(v);
    controls(v);
    // Note-start parameters must be initialized after the first envelope/LFO
    // update. The provisional zero-envelope setup is not an audible filter state.
    if (!legato) v.filter.initialized = false;
}
void Synth::event(const Event& m) {
    auto& c = channels_[m.channel];
    const double value = std::clamp<double>(m.value, 0, 1);
    switch (m.kind) {
    case EventKind::pitchBend: c.bend = std::clamp<double>(m.value, -24, 24); return;
    case EventKind::wheel: c.wheel = value; return;
    case EventKind::pedal: c.pedal = value; return;
    case EventKind::external: c.external = value; return;
    case EventKind::channelPressure: c.pressure = value; return;
    case EventKind::latestPressure: {
        Voice* latest=nullptr;for(auto& v:voices_)if(v.active&&v.channel==m.channel&&(!latest||v.serial>latest->serial))latest=&v;
        if(latest)latest->notePressure=value;return;
    }
    case EventKind::polyPressure: c.keyPressure[m.key] = value; return;
    case EventKind::channelVolume: c.volume = value; return;
    case EventKind::sustain:
        c.sustain = value >= 0.5;
        if (!c.sustain) for (auto& v : voices_) if (v.active && v.channel == m.channel && !v.held) release(v);
        return;
    case EventKind::allSoundOff:
        for (auto& v : voices_) if (v.channel == m.channel) v.active = false;
        for (auto& v : tails_) if (v.channel == m.channel) v.active = false;
        c.held.fill(0); return;
    default: break;
    }
    const bool on = m.kind == EventKind::noteOn && m.value > 0;
    if (!on) {
        c.held[m.key] = 0;
        Voice* chosen = nullptr;
        for (auto& v : voices_) if (v.active && v.held && v.channel == m.channel && v.key == m.key)
            if (!chosen || v.serial < chosen->serial) chosen = &v;
        if (!chosen) return;
        if (chosen->patch.mono) {
            uint64_t newest = 0; int key = -1;
            for (int i = 0; i < 128; ++i) if (c.held[i] > newest) { newest = c.held[i]; key = i; }
            if (key >= 0) { start(*chosen, m.channel, static_cast<uint8_t>(key), c.velocity[key], true, true); return; }
        }
        chosen->held = false;
        if (!c.sustain) release(*chosen);
        return;
    }
    c.held[m.key] = serial_ + 1; c.velocity[m.key] = value;
    Voice* chosen = nullptr;
    const auto& p = patches_[m.channel];
    if (p.mono) {
        for (auto& v : voices_) if (v.active && v.channel == m.channel) { chosen = &v; break; }
        if (chosen && chosen->held) { start(*chosen, m.channel, m.key, value, true); return; }
    }
    if (!chosen) {
        size_t same = 0; Voice* oldest = nullptr;
        for (auto& v : voices_) if (v.active && v.channel == m.channel && v.key == m.key) {
            ++same; if (!oldest || v.serial < oldest->serial) oldest = &v;
        }
        if (p.voiceRestart || same >= 2) chosen = oldest;
    }
    // A new strike releases the preceding strike of the same key even when
    // voice restart is disabled. Sustain still defers the envelope release.
    for (auto& v : voices_) if(v.active && v.held && v.channel==m.channel && v.key==m.key) {
        v.held=false;
        if(!c.sustain) release(v);
    }
    if (!chosen) for (size_t i = 0; i < voiceCount; ++i) {
        // Reuse the first free logical voice. With oscillator restart disabled,
        // its retained phase remains audible even when the next key differs.
        if (!voices_[i].active) { chosen = &voices_[i]; break; }
    }
    if (!chosen) {
        chosen = &voices_[0];
        for (auto& v : voices_)
            if ((chosen->held && !v.held) || (chosen->held == v.held &&
                (v.samples > chosen->samples || (v.samples == chosen->samples && v.serial > chosen->serial)))) chosen = &v;
    }
    start(*chosen, m.channel, m.key, value, false);
}
void Synth::inputSources(Voice& v) {
    const auto& c = channels_[v.channel];
    auto& s = v.sources;
    const double velocityCode = std::round(v.velocity * 127);
    s[7] = velocityResponse(velocityCode) / 128; s[8] = std::round(127 * (0.12 * (velocityCode / 127) + 0.88 * std::pow(velocityCode / 127, 4.5))) / 128;
    s[9] = v.key / 128.0;
    s[10] = std::clamp((v.key - 64.0) / 32, -1.0, 127.0/128);
    s[11] = std::round(c.wheel*127)/128; s[12] = std::round(c.pedal*127)/128; s[13] = std::round(c.external*127)/128;
    s[14] = std::round(std::max({c.pressure, c.keyPressure[v.key], v.notePressure})*127)/128; s[15] = 0;
}
void Synth::controls(Voice& v) {
    const auto& c = channels_[v.channel];
    auto& s = v.sources;
    auto mod = [&](const auto& mods) { double x = 0; for (const auto& m : mods) x += s[static_cast<unsigned>(m.source)] * m.amount; return x; };
    v.interpolationOrder = 1;
    for (size_t i = 0; i < 3; ++i) {
        const auto& o = v.patch.oscillators[i];
        v.oscillators[i].setPitch(v.pitch, c.bend + 0.5 * mod(o.pitchMods));
        v.levels[i] = dcaGain(o.level + mod(o.levelMods));
        if (o.enabled || (v.patch.am && i < 2))
            v.interpolationOrder = std::max(v.interpolationOrder, v.oscillators[i].interpolationOrder());
    }
    v.interpolator.order = v.interpolationOrder;
    const double cutoff = (performanceCutoff_<0?v.patch.cutoff:performanceCutoff_) + .5*std::trunc(2*(mod(v.patch.filterMods) + 2*v.patch.filterKeyTrack*s[9]));
    v.filter.configure(filterHertz(cutoff), performanceResonance_<0?v.patch.resonance:performanceResonance_, rate_);
    // Measured pan uses a signed integer balance followed by unit-energy
    // normalization of two linear gains, rather than a constant angular sweep.
    const double basePan = v.patch.pan >= 15 ? 63 : 8*(v.patch.pan-8);
    const double pan = std::clamp(basePan + std::trunc(s[static_cast<unsigned>(v.patch.panMod.source)]*v.patch.panMod.amount),-63.0,63.0);
    const double left = 63-pan, right = 63+pan;
    const double norm = std::hypot(left,right);
    v.leftGain = left/norm; v.rightGain = right/norm;
}
double Synth::nativeSample(Voice& v) {
    if (!bank_) return 0;
    std::array<double, 3> x{};
    // Sync responds to the preceding native sample's wrap indication.
    const bool syncTrigger = v.oscillators[0].wrapped();
    for (size_t i = 0; i < 3; ++i) {
        const double dcaPole = std::exp(-1 / (SampleOscillator::clockRate * .00043994));
        v.smoothedLevels[i] = v.levels[i] + (v.smoothedLevels[i] - v.levels[i]) * dcaPole;
        const bool am = v.patch.am && i < 2;
        const bool sourceOsc = i == 0 && v.patch.sync && v.patch.oscillators[1].enabled;
        if (!v.patch.oscillators[i].enabled && !am && !sourceOsc) continue;
        if (i == 1 && v.patch.sync && syncTrigger) v.oscillators[1].synchronize();
        const int bankOverride = i == 1 && v.patch.am && v.patch.emulateAmBank ? int(v.oscillators[0].bank()) : -1;
        x[i] = v.oscillators[i].tick(*bank_, i == 1 && (v.patch.am || v.patch.sync), bankOverride);
    }
    if (v.patch.am) return v.am.tick(x[0], x[1]) + x[2] * v.smoothedLevels[2];
    double result = 0;
    for (size_t i = 0; i < 3; ++i) if (v.patch.oscillators[i].enabled) result += x[i] * v.smoothedLevels[i];
    return result;
}
double Synth::sample(Voice& v) {
    for (size_t i = 0; i < 4; ++i) v.sources[3 + i] = v.envelopes[i].tick() * (126.0/128);
    const bool controlTick = v.controlPhase >= 1 - 1e-12;
    if (controlTick) {
        v.controlPhase -= 1;
        inputSources(v);
        const auto previous = v.sources;
        for (size_t i = 0; i < 3; ++i) {
            auto spec=v.patch.lfos[i];if(i==0&&performanceLfo1_>=0)spec.speed=performanceLfo1_;
            v.sources[i] = (63.0/128) * v.lfos[i].tick(spec, previous[static_cast<unsigned>(spec.depthSource)], 83.59257507324219);
        }
    }
    v.controlPhase += 83.59257507324219 / rate_;
    if (controlTick && v.glideStep) {
        const float target = static_cast<float>(v.key-v.glideOrigin);
        v.glideOffset += std::clamp(target-v.glideOffset, -std::abs(v.glideStep), std::abs(v.glideStep));
        v.pitch = v.glideOrigin + std::trunc(v.glideOffset*256)/256;
    }
    if (controlTick) controls(v);
    ++v.samples;
    if (v.envelopes[3].finished()) for (auto& oscillator : v.oscillators) oscillator.stop();
    while (v.clockPhase >= 1) {
        v.history[0] = v.history[1]; v.history[1] = v.history[2]; v.history[2] = v.history[3];
        v.history[3] = nativeSample(v); v.clockPhase -= 1;
        v.interpolator.update(v.history, v.interpolationOrder);
    }
    double x = v.interpolator.value(v.clockPhase);
    v.clockPhase += SampleOscillator::clockRate / rate_;
    if (v.patch.dcBlock) x = v.dc.tick(x);
    if (v.patch.filterEnabled) x = v.filter.tick(x);
    if (v.patch.outputSaturation) x = outputTransfer(x);
    // Generated low-level probes identify a quantized volume expansion and
    // a one-code offset before the output gain curve.
    const double volumeCode = std::floor(v.patch.volume * 65/63);
    const double finalControl = std::clamp(std::floor(std::max(0.0,v.sources[6])*128*volumeCode/32)-1, 0.0, 255.0)/255;
    // Smooth fitted output gain, independent of the oscillator DAC level law.
    const double gain = outputDcaGain(finalControl);
    if (gain != v.amplitudeTarget) {
        v.amplitudeTarget = gain;
        const unsigned attack = v.patch.envelopes[3].times[0];
        const double rampSeconds = v.samples > 1 ? .01 : attack == 0 ? .0025 : attack == 1 ? .004 : attack == 2 ? .005 : .01;
        v.amplitudeRemaining = std::max(1u, static_cast<unsigned>(rate_ * rampSeconds));
        v.amplitudeStep = (gain - v.amplitude) / v.amplitudeRemaining;
    }
    if (v.amplitudeRemaining) {
        v.amplitude += v.amplitudeStep;
        if (--v.amplitudeRemaining == 0) v.amplitude = v.amplitudeTarget;
    }
    const double output = x * v.amplitude * v.patch.masterGain * channels_[v.channel].volume * v.fade;
    if (v.fadeRemaining) {
        v.fade += v.fadeStep;
        if (--v.fadeRemaining == 0) {
            v.fade = v.fadeOut ? 0 : 1;
            if (v.fadeOut) v.active = false;
        }
    }
    // Keep the state alive through the measured output ramp and quiet retirement
    // interval. Cutting the voice at the last envelope tick caused discontinuities.
    if (v.envelopes[3].finished() && !v.amplitudeRemaining && ++v.retireSamples >= rate_*.07)
        v.active = false;
    return output;
}
bool Synth::render(float* left, float* right, size_t frames, const Event* events, size_t count) {
    if ((frames && (!left || !right || left == right)) || (count && !events)) return false;
    for (size_t i = 0; i < count; ++i) {
        const auto& e = events[i];
        if (e.frame >= frames || e.channel >= 16 || e.key >= 128 || !std::isfinite(e.value) ||
            static_cast<unsigned>(e.kind) > static_cast<unsigned>(EventKind::latestPressure) ||
            (i && e.frame < events[i-1].frame)) return false;
    }
    size_t next = 0;
    for (size_t i = 0; i < frames; ++i) {
        while (next < count && events[next].frame == i) event(events[next++]);
        double l = 0, r = 0;
        for (auto& v : voices_) if (v.active) { const double x = sample(v); l += x*v.leftGain; r += x*v.rightGain; }
        for (auto& v : tails_) if (v.active) { const double x = sample(v); l += x*v.leftGain; r += x*v.rightGain; }
        left[i] = static_cast<float>(l); right[i] = static_cast<float>(r);
    }
    return true;
}
} // namespace hybrid
