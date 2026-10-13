#include "hybrid_synth.hpp"
#include <algorithm>
#include <cmath>

namespace hybrid {
namespace {
constexpr double pi = 3.14159265358979323846;
// Independently generated periodic colored noise. Fourier magnitudes follow a
// smooth low-pass spectrum fitted to aggregate statistics, never reference samples.
// Initialization happens before rendering and needs no heap allocation.
const std::array<double,256> noiseCycle = [] {
    std::array<double,256> values{};
    uint32_t seed = 42;
    for (int harmonic=1; harmonic<=128; ++harmonic) {
        seed ^= seed<<13; seed ^= seed>>17; seed ^= seed<<5;
        const double angle = 2*pi*(seed/4294967296.0);
        const double peak = 1 + 23.639822*std::exp(-std::pow((harmonic-2.234153)/.714946,2));
        const double lowCut = 1-std::exp(-std::pow(harmonic/2.632848,4));
        const double amplitude = std::sqrt(peak*lowCut/std::pow(harmonic*harmonic+8.300261*8.300261,2.945439/2));
        for (size_t i=0;i<values.size();++i)
            values[i] += amplitude*std::cos(2*pi*harmonic*i/256+angle);
    }
    double mean=0,variance=0;
    for(double v:values)mean+=v/256;
    for(double v:values)variance+=(v-mean)*(v-mean)/256;
    size_t zero=0;
    for(size_t i=0;i<values.size();++i){
        values[i]=std::clamp(std::round((values[i]-mean)*23.65/std::sqrt(variance)+1.1),-64.0,63.0);
        if(std::abs(values[i])<std::abs(values[zero]))zero=i;
    }
    std::rotate(values.begin(),values.begin()+zero,values.end());
    values[0]=0;
    return values;
}();
}
double envelopeSeconds(unsigned code) {
    // Public musician's manual p.64: time codes 16..63 double every six steps.
    // Low codes have a linear timing floor; these are rounded manual values.
    constexpr double low[] = {0,.01,.01,.02,.02,.03,.03,.03,.04,.04,.04,.05,.06,.06,.07,.08};
    code = std::min(code, 63u);
    return code < 16 ? low[code] : 20.48 * std::exp2((double(code) - 63) / 6);
}
double lfoHertz(unsigned code) {
    // Independently inferred from sparse frequency-response probes.
    code = std::min(code, 63u);
    constexpr double step = 83.59257507324219 / 256;
    return step * (code < 7 ? code / 7.0 : code - 6.0);
}
double filterHertz(double control) {
    // Four exponential regions independently fitted to sparse response probes.
    // Frequency is rate-independent; configure applies the observed sine mapping.
    const double c = std::clamp(control, 0.0, 127.5);
    double octave;
    if (c < 32) octave = 5.383981781 + .091071929*c;
    else if (c < 64) octave = 8.276477830 + .089689484*(c-32);
    else if (c < 96) octave = 11.124736697 + .082184665*(c-64);
    else octave = 13.732841565 + .054838124*(c-96);
    return std::exp2(octave);
}
double dcaGain(double control) {
    if (control <= 0) return 0;
    const double x = std::clamp(control, 0.0, 64.0) / 63;
    // Smooth approximation to sparse amplitude observations; modulation can
    // exceed panel level 63 and reaches the unity plateau at level 64.
    return std::min(1.0, 0.005 * (1 - std::exp(-control / 1.5)) + 0.966 * std::pow(x, 4.55));
}
double outputTransfer(double input) {
    // Cubic soft clipping fitted to measured input/output points. This general
    // smoothstep construction reaches a bounded plateau with zero slope.
    const double u = std::clamp(input / 1.93, -1.0, 1.0);
    return u * (1.5 - 0.5 * u * u);
}
double outputDcaGain(double normalizedLevel) {
    if (normalizedLevel <= 0) return 0;
    const double x = std::clamp(normalizedLevel,0.0,1.0);
    // Weighted fourth-degree fit to sparse measured DCA responses, including
    // the small positive gain at the lowest nonzero control level.
    return .004439249+x*(.063760581+x*(.138362299+x*(3.861771588-1.584366228*x)));
}
double velocityResponse(double velocityCode) {
    const double upper = std::max(0.0, velocityCode - 100);
    return std::round(velocityCode <= 64 ? velocityCode :
        64 + .8 * (velocityCode - 64) + .017284 * upper * upper);
}
double interpolateNative(const std::array<double, 4>& history, double phase) {
    // Standard Lagrange construction at nodes 0, 1, 2. Observed input/history
    // pairs identify this default interpolation order and history alignment.
    double output = 0;
    for (int i = 0; i < 3; ++i) {
        double basis = 1;
        for (int j = 0; j < 3; ++j) if (i != j) basis *= (phase - j) / (i - j);
        output += history[static_cast<size_t>(i)] * basis;
    }
    return output;
}
void NativeInterpolator::update(const std::array<double, 4>& history, unsigned nextOrder) {
    order = std::clamp(nextOrder, 1u, 4u);
    double slope = 0;
    for (unsigned i = 1; i <= order; ++i) slope += i * coefficients[i];
    const unsigned degree = order >= 3 ? order - 1 : order;
    std::array<double, 5> polynomial{};
    auto multiply = [](std::array<double, 5>& p, double root) {
        for (int i = 4; i > 0; --i) p[i] = p[i-1] - root*p[i];
        p[0] *= -root;
    };
    // Construct Lagrange bases directly from their nodes, not stored kernels.
    for (unsigned i = 0; i <= degree; ++i) {
        std::array<double, 5> basis{}; basis[0] = 1;
        double denominator = 1;
        for (unsigned j = 0; j <= degree; ++j) if (i != j) {
            multiply(basis, j); denominator *= double(i)-j;
        }
        for (unsigned k = 0; k <= degree; ++k)
            polynomial[k] += history[i]*basis[k]/denominator;
    }
    if (order >= 3) {
        // A polynomial vanishing at every sample node adds the independently
        // observed derivative constraint without changing those sample values.
        std::array<double, 5> correction{}; correction[0] = 1;
        for (unsigned j = 0; j <= degree; ++j) multiply(correction, j);
        const double scale = (slope-polynomial[1])/correction[1];
        for (unsigned k = 0; k <= order; ++k) polynomial[k] += scale*correction[k];
    }
    for (unsigned k = 0; k <= order; ++k) coefficients[k] = polynomial[k];
}
double NativeInterpolator::value(double phase) const {
    double result = coefficients[order];
    for (unsigned i = order; i > 0; --i) result = result*phase + coefficients[i-1];
    return result;
}
void DcBlocker::configure(double rate) {
    const double k = std::tan(pi * 15 / rate);
    b0 = 1 / (1 + std::sqrt(2.0) * k + k*k);
    a1 = 2 * (k*k - 1) * b0;
    a2 = (1 - std::sqrt(2.0) * k + k*k) * b0;
}
double DcBlocker::tick(double input) {
    const double output = b0 * (input - 2*x1 + x2) - a1*y1 - a2*y2;
    x2 = x1; x1 = input; y2 = y1; y1 = output;
    return output;
}
double AmplitudeModulator::tick(double modulation, double carrier) {
    const double output = carrier * gain;
    // Constant-input original-DLL probes: DC offset .4, slope .6 and a
    // short native-clock low-pass. Preserve negative amplitudes (ring action).
    const double target = .4 + .6 * modulation;
    const double pole = std::exp(-1 / (SampleOscillator::clockRate * .000049076));
    gain = target + (gain - target) * pole;
    return output;
}
void Contour::start(const EnvelopeSpec& s, double rate, double velocity, int key, bool cycle, bool reset) {
    spec_ = s; rate_ = rate; cycle_ = cycle; released_ = false;
    if (reset || finished()) value_ = 0;
    const int velocityCode = static_cast<int>(std::lround(std::clamp(velocity, 0.0, 1.0) * 127));
    // Compact piecewise fit to generated-program velocity probes, not an
    // imported velocity table. The upper half is softer than raw MIDI velocity.
    const double linear = velocityResponse(velocityCode);
    const double response = s.exponentialVelocity ? velocityCode * velocityCode / 127.0 : linear;
    // Measured level scaling is multiplicative, including a small unity offset.
    const double scale = std::max(0.0, 252 + std::floor(s.velocity * (response - 128) / 32));
    for (size_t i = 0; i < 3; ++i) {
        const double level = 2 * s.levels[i] - (s.levels[i] < 0 ? 2 : 0);
        levels_[i] = std::trunc(level * scale / 256) / 126;
    }
    clockPhase_ = 1;
    for (size_t i = 0; i < 4; ++i) seconds_[i] = envelopeSeconds(s.times[i]);
    const int attackReduction = s.attackVelocity * velocityCode / 128;
    seconds_[0] = envelopeSeconds(static_cast<unsigned>(std::max(0, int(s.times[0]) - attackReduction)));
    // Keyboard scaling changes the time code rather than scaling seconds. This
    // relation matches the 21 generated-program observations in the research log.
    const int keyReduction = std::max(key - 36, 0) * s.keyDecay / 128;
    for (size_t i : {size_t(1), size_t(2)})
        seconds_[i] = envelopeSeconds(static_cast<unsigned>(std::max(0, int(s.times[i]) - keyReduction)));
    enter(0);
}
void Contour::enter(unsigned stage) {
    stage_ = stage; origin_ = value_; elapsed_ = 0;
    if (stage == 6) { value_ = 0; return; }
    if (stage == 5) return;
    target_ = stage < 3 ? levels_[stage] : (stage == 3 && spec_.secondRelease ? (std::floor(origin_ * 63) - 12) / 126 : 0);
    const double seconds = stage < 4 ? seconds_[stage] : envelopeSeconds(39);
    // Hardware control updates give even a zero-time segment one control tick.
    length_ = static_cast<uint64_t>(std::max(1.0, std::round(seconds * 100)));
}
void Contour::release() {
    if (!cycle_ && !released_ && !finished()) { released_ = true; enter(3); }
}
double Contour::tick() {
    constexpr double controlRate = 83.59257507324219;
    const bool update = clockPhase_ >= 1 - 1e-12;
    if (update) clockPhase_ -= 1;
    clockPhase_ += controlRate / rate_;
    if (update && stage_ < 5) {
        ++elapsed_;
        // Measured ramps retain a fractional origin but calculate their slope
        // from whole level units, with 1/256-unit increment precision.
        const double delta = target_*126 - std::trunc(origin_*126 + std::copysign(1e-10,origin_));
        const double step = std::trunc(delta*256/double(length_)) / (256*126);
        value_ = origin_ + step*std::min(elapsed_,length_);
        if (elapsed_ >= length_) {
            if (stage_ < 2) enter(stage_ + 1);
            else if (stage_ == 2) enter(cycle_ ? 3 : 5);
            else if (stage_ == 3 && spec_.secondRelease) enter(4);
            else enter(6);
        }
    }
    // Control outputs are whole signed level units. Keeping the trajectory in
    // double avoids importing the original fixed-point ramp implementation.
    return std::trunc(value_ * 126 + std::copysign(1e-10, value_)) / 126;
}
void SampleOscillator::configure(const WaveBank& bank, unsigned wave, int zoneKey, double transpose, bool reset) {
    const auto* region = bank.region(wave, zoneKey);
    valid_ = region != nullptr;
    if (!valid_) { halted_ = true; return; }
    region_ = *region;
    resolution_ = region_.control & 7;
    transpose_ = 12 + transpose + region_.transpose + region_.fine / 256.0;
    // Shift octaves between the frequency register and address resolution. This
    // preserves playback speed while using more of the 16-bit tuning range.
    while (transpose_ < 12 && resolution_ < 7) { transpose_ += 12; ++resolution_; }
    while (transpose_ >= 36 && resolution_ > 0) { transpose_ -= 12; --resolution_; }
    const unsigned size = 8 + ((region_.control >> 3) & 7);
    shift_ = 17 + resolution_ - size;
    period_ = 1u << (17 + resolution_);
    accumulator_ %= period_;
    if (reset || region_.oneShot) restart();
    else halted_ = false;
}
void SampleOscillator::setPitch(double key, double modulation) {
    double semitone = std::clamp(key + transpose_ + modulation, 0.0, 160.0);
    // Original-DLL high-note probes show octave folding above this range,
    // with a short plateau at the upper endpoint before the fold.
    while (semitone >= 128.0625) semitone -= 12;
    // Runtime pitch sweeps locate these traversal-pitch boundaries. The 1/16
    // semitone decision grid differs from the tuning register's 1/32 grid.
    const double traversal = std::floor(semitone*16) - 192*double(shift_);
    interpolationOrder_ = 1u + (traversal > -320) + (traversal > -32) + (traversal > 368);
    // Measured pitch steps are 1/32 semitone. Use an analytic equal-temperament
    // response with two fitted scalars, rather than an extracted tuning table.
    // The half-step rounding bias approximates the observed integer conversion;
    // it is an approximation calibrated from measured oscillator responses.
    const unsigned step = std::min(4096u, static_cast<unsigned>(std::floor(semitone * 32)));
    const double increment = 440 * 0.999852 * std::exp2((step / 32.0 - 69) / 12) * 131072 / clockRate;
    const double rounded = std::floor(increment + 0.5 - ((step & 1) ? 0.3 : 0.0));
    frequency_ = static_cast<uint32_t>(std::clamp(rounded, 0.0, 65535.0));
}
double SampleOscillator::tick(const WaveBank& bank, bool forceLoop, int bankOverride) {
    const bool stopAfterSample = region_.oneShot && wrapped_ && !forceLoop;
    wrapped_ = false;
    if (!valid_ || halted_) return 0;
    uint32_t address = region_.address;
    if (bankOverride >= 0) address = (address & ~uint32_t(65536)) | (uint32_t(bankOverride & 1) << 16);
    const uint8_t value = bank.byte(address + (accumulator_ >> shift_));
    if (value == 0) { halted_ = true; return 0; }
    // In the measured one-shot path, the wrapped address contributes one final
    // sample before the halt takes effect. Zero-byte stop markers stay immediate.
    if (stopAfterSample) { halted_ = true; return (int(value) - 128) / 127.0; }
    accumulator_ += frequency_;
    if (accumulator_ >= period_) {
        accumulator_ %= period_;
        wrapped_ = true;
    }
    return (int(value) - 128) / 127.0;
}
void ResonantFilter::configure(double hz, double res, double rate) {
    const double g = std::sin(pi * std::clamp(hz/rate, 1e-8, .49));
    targetPole = 2 * g - 1;
    const double r = std::clamp(res / 31, 0.0, 1.0);
    const double maximum = ((((((-3.898761693*g+7.607623024)*g-5.501183092)*g-.087408789)*g+4.206274249)*g-5.725536702)*g+3.573070386);
    // Sparse independent probes identify a clipped quadratic resonance response.
    targetFeedback = maximum * std::max(0.0, -.04600997 + r*(1.61922036-.57321041*r));
    const double q = std::sqrt(r);
    targetGain = 1 / (1 + q*(1.104318586+q*(.642337406+q*(.165267362+.118373394*q))));
    poleSmooth = std::exp(-1 / (rate * .00195318));
    feedbackSmooth = poleSmooth; gainSmooth = poleSmooth;
    if (!initialized) { pole = targetPole; sectionGain = g; feedback = targetFeedback; gain = targetGain; initialized = true; }
}
double ResonantFilter::tick(double input) {
    // Each section implements H(z)=g(1+z^-1)/(1+p*z^-1).
    // The four-section output is fed back with one sample of delay. This generic
    // signal-flow implementation is independent of the original instruction order.
    const double g = sectionGain;
    double x = input * (1 + 1.075 * feedback) - feedback * previousOutput[3];
    for (size_t i = 0; i < previousInput.size(); ++i) {
        const double y = g * (x + previousInput[i]) - pole * previousOutput[i];
        previousInput[i] = x; previousOutput[i] = y; x = y;
    }
    const double output = x * gain;
    // Step probes show that feed-forward gain follows the smoothed pole one
    // sample later than the recurrence coefficient during parameter changes.
    sectionGain = (1 + pole) * .5;
    pole = targetPole + (pole - targetPole) * poleSmooth;
    feedback = targetFeedback + (feedback - targetFeedback) * feedbackSmooth;
    gain = targetGain + (gain - targetGain) * gainSmooth;
    return output;
}
void Modulator::restart(bool resetPhase) {
    if (resetPhase) phase = 0;
    age = 0; depthLevel = -1; delayClock = 0; humanOffset = 0;
    humanHold = -1;
}
double Modulator::tick(const LfoSpec& s, double mod, double rate) {
    auto randomUnit = [&]() {
        random ^= random << 13; random ^= random >> 17; random ^= random << 5;
        return (random / 4294967295.0) * 2 - 1;
    };
    const unsigned index = static_cast<unsigned>(phase * 256) & 255;
    double wave = 0;
    switch (s.shape) {
    case LfoShape::triangle: wave = index < 128 ? int(index)-64 : 191-int(index); break;
    case LfoShape::saw: wave = int(index / 2)-64; break;
    case LfoShape::square: wave = index < 128 ? 63 : 0; break;
    case LfoShape::noise: wave = noiseCycle[index]; break;
    }
    if (depthLevel < 0) depthLevel = s.startLevel * 2;
    const double depth = std::clamp(depthLevel + std::trunc(mod * 128), 0.0, 255.0);
    const double result = std::trunc(wave * depth / 128) / 63;
    // Measured legacy delay advances in discrete half-level steps, after output.
    const bool emuStep = delayClock <= 1e-10;
    if (emuStep) {
        depthLevel += std::clamp(s.endLevel * 2.0 - depthLevel, -double(s.delay), double(s.delay));
        delayClock += 8.125 * rate / 83.59257507324219;
    }
    delayClock -= 1;
    age += 1 / rate;
    const double offset = s.humanize && s.speed >= 7 ? humanOffset : 0;
    phase += (lfoHertz(s.speed) + offset*(83.59257507324219/256)) / rate;
    // Independent semi-Markov model fitted to aggregate offset occupancy and
    // dwell statistics. Each restart chooses a biased pair of adjacent levels;
    // rare excursions and durations are generated from our own PRNG.
    if (emuStep && s.humanize) {
        if (humanHold < 0) humanBase = randomUnit()<0 ? -1 : 0;
        else if (--humanHold <= 0) {
            if (humanOffset == humanBase) humanOffset = humanBase+1;
            else if (humanOffset == humanBase+2) humanOffset = humanBase+1;
            else humanOffset = humanBase + (randomUnit()>.92 ? 2 : 0);
        }
        if (humanHold <= 0) {
            const double u = std::max(1e-12,(randomUnit()+1)*.5);
            const double z = std::sqrt(-2*std::log(u))*std::cos(pi*(randomUnit()+1));
            humanHold = static_cast<int>(std::clamp(std::round(5.5+2.3*z),2.0,11.0));
        }
    }
    if (phase >= 1) {
        phase -= std::floor(phase);
    }
    return result;
}

} // namespace hybrid
