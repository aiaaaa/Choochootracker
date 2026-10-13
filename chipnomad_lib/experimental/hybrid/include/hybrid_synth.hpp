#pragma once
#include "wave_bank.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace hybrid {
// IDs follow the hardware modulation menu; OFF is 15.
enum class Source : uint8_t { lfo1, lfo2, lfo3, env1, env2, env3, env4,
    velocity, velocityExp, keyboard, keyboard2, wheel, pedal, external, pressure, off };
struct Mod { Source source = Source::off; float amount = 0; };
struct EnvelopeSpec {
    std::array<float, 3> levels{{63, 45, 40}}; // bipolar -63..63
    std::array<uint8_t, 4> times{{0, 24, 16, 24}}; // hardware time codes
    uint8_t velocity = 0, attackVelocity = 0, keyDecay = 0;
    bool exponentialVelocity = false, secondRelease = false;
};
enum class LfoShape : uint8_t { triangle, saw, square, noise };
struct LfoSpec {
    LfoShape shape = LfoShape::triangle;
    uint8_t speed = 24, startLevel = 0, endLevel = 0, delay = 0;
    Source depthSource = Source::off;
    bool reset = true, humanize = false;
};
struct Oscillator {
    uint16_t wave = 0; // stock IDs unchanged; 75..255 are additional choices
    bool enabled = false;
    float semitones = 0, level = 50; // DCA panel scale 0..63
    std::array<Mod, 2> pitchMods{}, levelMods{};
};
struct Patch {
    std::array<Oscillator, 3> oscillators{{{0, true, 0, 50}, {0, false, 0, 50}, {0, false, 0, 50}}};
    std::array<EnvelopeSpec, 4> envelopes{};
    std::array<LfoSpec, 3> lfos{};
    std::array<Mod, 2> filterMods{};
    Mod panMod{};
    float cutoff = 100, resonance = 0, filterKeyTrack = 0;
    float volume = 50, pan = 8, masterGain = .8f; // measured default output trim; volume 0..63, pan 0..15
    uint8_t glide = 0;
    bool sync = false, am = false, mono = false, voiceRestart = false;
    bool envelopeRestart = true, oscillatorRestart = true, fullCycle = false;
    bool filterEnabled = true, dcBlock = false, emulateAmBank = true;
    bool outputSaturation = true; // legacy output behavior; disable for a linear engineering path
};
// Functions use documented scales; calibration approximations are itemized in FIDELITY.md.
double envelopeSeconds(unsigned code);
double lfoHertz(unsigned code);
double filterHertz(double control);
double dcaGain(double control);
double outputTransfer(double input);
double outputDcaGain(double normalizedLevel);
double velocityResponse(double velocityCode);
double interpolateNative(const std::array<double, 4>& history, double phase);

// Polynomial reconstruction from native samples. Higher orders also constrain
// the initial slope to the preceding interval's endpoint derivative.
struct NativeInterpolator {
    std::array<double, 5> coefficients{};
    unsigned order = 1;
    void update(const std::array<double, 4>& history, unsigned nextOrder);
    double value(double phase) const;
};

class Contour {
public:
    void start(const EnvelopeSpec&, double rate, double velocity, int key,
               bool cycle, bool reset = true);
    void release();
    double tick();
    double value() const { return value_; }
    bool finished() const { return stage_ == 6; }
private:
    EnvelopeSpec spec_{};
    std::array<double, 4> seconds_{};
    std::array<double, 3> levels_{};
    double value_ = 0, origin_ = 0, target_ = 0, rate_ = 48000, clockPhase_ = 1;
    uint64_t length_ = 0, elapsed_ = 0;
    unsigned stage_ = 6;
    bool cycle_ = false, released_ = false;
    void enter(unsigned);
};

// Hardware accumulator/addressing unit, separately testable at the native clock.
class SampleOscillator {
public:
    static constexpr double clockRate = 38455.85546875;
    void configure(const WaveBank&, unsigned wave, int zoneKey, double transpose, bool reset);
    void setPitch(double midiKey, double modulation = 0);
    double tick(const WaveBank&, bool forceLoop = false, int bankOverride = -1);
    void restart() { accumulator_ = 0; halted_ = false; wrapped_ = false; }
    void stop() { halted_ = true; }
    void synchronize() { accumulator_ = 0; wrapped_ = false; } // sync preserves a stop-marker halt
    bool wrapped() const { return wrapped_; }
    bool halted() const { return halted_; }
    unsigned bank() const { return region_.address >> 16; }
    uint32_t frequencyRegister() const { return frequency_; }
    uint32_t phaseRegister() const { return accumulator_; }
    unsigned resolution() const { return resolution_; }
    unsigned interpolationOrder() const { return interpolationOrder_; }
private:
    WaveRegion region_{};
    uint32_t accumulator_ = 0, frequency_ = 0, period_ = 1u << 18;
    unsigned resolution_ = 1, shift_ = 9;
    unsigned interpolationOrder_ = 1;
    double transpose_ = 0;
    bool valid_ = false, halted_ = true, wrapped_ = false;
};

enum class EventKind { noteOn, noteOff, sustain, pitchBend, allSoundOff,
    wheel, pedal, external, channelPressure, polyPressure, channelVolume, latestPressure };
struct Event {
    uint32_t frame = 0;
    EventKind kind = EventKind::noteOn;
    uint8_t channel = 0, key = 60;
    float value = 1; // normalized controls; pitch bend in semitones
};
struct Modulator {
        double phase = 0, age = 0, noise = 0, humanOffset = 0;
        double depthLevel = -1, delayClock = 0;
        uint32_t random = 1;
        int humanBase = 0, humanHold = -1;
        void restart(bool resetPhase);
        double tick(const LfoSpec&, double modulation, double rate);
};

// Generic cascade of four bilinear one-poles with a delayed feedback loop.
struct ResonantFilter {
    std::array<double, 4> previousInput{}, previousOutput{};
    double pole = 0, feedback = 0, gain = 1;
    double sectionGain = .5;
    double targetPole = 0, targetFeedback = 0, targetGain = 1;
    double poleSmooth = 0, feedbackSmooth = 0, gainSmooth = 0;
    bool initialized = false;
    void configure(double frequencyHz, double resonance, double rate);
    double tick(double input);
};

// General bilinear Butterworth high-pass and smoothed amplitude modulation.
struct DcBlocker {
    double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    double b0 = 1, a1 = 0, a2 = 0;
    void configure(double sampleRate);
    double tick(double input);
};
struct AmplitudeModulator {
    double gain = 0;
    double tick(double modulation, double carrier);
};

class Synth {
    friend struct EngineeringProbe;
public:
    static constexpr size_t voiceCount = 8;
    explicit Synth(double sampleRate = 48000);
    bool setSampleRate(double sampleRate);
    // Non-owning bank: keep it alive and unchanged until detached. Resets voices.
    void setWaveBank(const WaveBank* bank);
    void reset();
    void setPatch(const Patch& patch); // all channels; applies to subsequent notes
    void setChannelPatch(unsigned channel, const Patch& patch);
    // Tracker extension: live overrides, -1 restores each voice's own patch.
    void setPerformance(int cutoff, int resonance, int lfo1Rate);
    size_t activeVoices() const;
    bool render(float* left, float* right, size_t frames, const Event* events = nullptr, size_t count = 0);
private:
    int performanceCutoff_=-1, performanceResonance_=-1, performanceLfo1_=-1;
    struct Voice {
        double notePressure = 0;
        bool active = false, held = false, used = false;
        uint8_t channel = 0, key = 0;
        uint64_t serial = 0, samples = 0;
        Patch patch{};
        double velocity = 0, pitch = 60, glideOrigin = 60;
        float glideOffset = 0, glideStep = 0;
        std::array<Contour, 4> envelopes{};
        std::array<Modulator, 3> lfos{};
        std::array<SampleOscillator, 3> oscillators{};
        std::array<double, 16> sources{};
        std::array<double, 3> levels{}, smoothedLevels{};
        std::array<double, 4> history{};
        NativeInterpolator interpolator{};
        unsigned interpolationOrder = 1;
        double clockPhase = 1, controlPhase = 1;
        DcBlocker dc{};
        AmplitudeModulator am{};
        double amplitude = 0, amplitudeTarget = 0, amplitudeStep = 0;
        unsigned amplitudeRemaining = 0;
        double fade = 1, fadeStep = 0;
        unsigned fadeRemaining = 0, retireSamples = 0;
        bool fadeOut = false;
        double leftGain = 0.7071067811865476, rightGain = 0.7071067811865476;
        ResonantFilter filter{};
    };
    struct Channel {
        double bend = 0, wheel = 0, pedal = 0, external = 0, pressure = 0, volume = 1;
        bool sustain = false;
        int lastKey = -1;
        std::array<double, 128> keyPressure{}, velocity{};
        std::array<uint64_t, 128> held{};
    };
    std::array<Voice, voiceCount> voices_{};
    std::array<Voice, voiceCount> tails_{}; // bounded, preallocated stolen-voice fades
    std::array<Channel, 16> channels_{};
    std::array<Patch, 16> patches_{};
    const WaveBank* bank_ = nullptr;
    double rate_ = 48000;
    uint64_t serial_ = 0;
    void event(const Event&);
    void release(Voice&);
    void retireForReplacement(Voice&);
    void start(Voice&, uint8_t channel, uint8_t key, double velocity, bool legato, bool returning = false);
    void inputSources(Voice&);
    void controls(Voice&);
    double sample(Voice&);
    double nativeSample(Voice&);
};
} // namespace hybrid
