#include "insert_fx.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif
#include "external/insert_fx/Distortion.h"
#include "external/insert_fx/StereoDoubler.h"
#include "external/insert_fx/ott_dsp.c"
#include "external/insert_fx/tapescam.h"
using M = InsertMapping;
static const InsertDescriptor descriptors[] = {
    {"OFF", 0, {}},
    {"Compressor",
     8,
     {{"Thresh", "Threshold", "dB", -60, 0, 153, M::linear},
      {"Attack", "Attack", "Hz", 800, 3, 96, M::exponential},
      {"Release", "Release", "Hz", 60, 0.4f, 128, M::exponential},
      {"Makeup", "Makeup", "dB", 0, 24, 0, M::linear},
      {"Ratio", "Ratio", "", 0, 7, 3, M::discrete},
      {"Detect", "Detector source", "", 0, 2, 0, M::discrete},
      {"DetFilt", "Detector filter", "", -1, 1, 128, M::bipolar},
      {"Mix", "Mix", "%", 0, 1, 255, M::linear}}},
    {"Distortion",
     4,
     {{"Input", "Input", "dB", -12, 12, 128, M::bipolar},
      {"Mode", "Mode", "", 0, 4, 0, M::discrete},
      {"Output", "Output", "dB", -12, 12, 128, M::bipolar},
      {"Mix", "Mix", "%", 0, 1, 255, M::linear}}},
    {"Doubler",
     2,
     {{"Detune", "Detune", "%", 0, 1, 153, M::linear}, {"Mix", "Mix", "%", 0, 1, 102, M::linear}}},
    {"TAPESCAM",
     6,
     {{"Input", "Input", "%", 0, 1, 179, M::linear},
      {"Drive", "Drive", "%", 0, 1, 0, M::linear},
      {"Color", "Color", "%", 0, 1, 0, M::linear},
      {"Wobble", "Wobble", "%", 0, 1, 0, M::linear},
      {"Tone", "Tone", "", -1, 1, 128, M::bipolar},
      {"Output", "Output", "%", 0, 1, 255, M::linear}}},
    {"OTT",
     6,
     {{"Depth", "Depth", "%", 0, 1, 64, M::linear},
      {"Time", "Time", "x", 0.1f, 2, 128, M::exponential},
      {"Upward", "Upward", "%", 0, 1, 64, M::linear},
      {"Down", "Downward", "%", 0, 1, 128, M::linear},
      {"Input", "Input", "dB", -24, 24, 128, M::bipolar},
      {"Output", "Output", "dB", -24, 24, 112, M::bipolar}}}};
static_assert(sizeof(descriptors) / sizeof(descriptors[0]) == insertModuleCount,
              "Stable module catalogue");
const InsertDescriptor& insertDescriptor(int m) {
  return descriptors[m >= 0 && m < insertModuleCount ? m : 0];
}
uint8_t insertClamp(int m, int p, int v) {
  const auto& d = insertDescriptor(m);
  if (p < 0 || p >= d.count) return 0;
  return std::max(
      0, std::min(v, d.parameters[p].mapping == M::discrete ? (int)d.parameters[p].maximum : 255));
}
float insertMap(int m, int p, int v) {
  const auto& d = insertDescriptor(m);
  if (p < 0 || p >= d.count) return 0;
  const auto& a = d.parameters[p];
  v = insertClamp(m, p, v);
  float x = v / 255.0f;
  if (a.mapping == M::discrete) return v;
  if (a.mapping == M::exponential) return a.minimum * powf(a.maximum / a.minimum, x);
  if (a.mapping == M::bipolar)
    return v >= 128 ? a.maximum * (v - 128) / 127.0f : -a.minimum * (v - 128) / 128.0f;
  return a.minimum + (a.maximum - a.minimum) * x;
}
void insertSelect(InsertConfig* c, int m) {
  c->module = m >= 0 && m < insertModuleCount ? m : 0;
  c->bypass = 0;
  ++c->selection;
  for (int i = 0; i < 8; ++i) {
    c->values[i] = insertDescriptor(c->module).parameters[i].initial;
    ++c->edits[i];
  }
}
void insertEdit(InsertConfig* c, int p, int v) {
  if (p < 0 || p >= insertDescriptor(c->module).count) return;
  c->values[p] = insertClamp(c->module, p, v);
  ++c->edits[p];
}
void insertDescribe(char* text, size_t size, int m, int p, int value) {
  const auto& d = insertDescriptor(m);
  if (p < 0 || p >= d.count) {
    snprintf(text, size, "Inactive");
    return;
  }
  float v = insertMap(m, p, value);
  if (m == insertDistortion && p == 1) {
    static const char* n[] = {"Density", "Drive", "Spiral", "Mojo", "Dyno"};
    snprintf(text, size, "%s", n[(int)v]);
  } else if (m == insertCompressor && p == 4) {
    static const float n[] = {1.5f, 2, 3, 4, 6, 8, 16, 20};
    snprintf(text, size, "%.1f:1", n[(int)v]);
  } else if (m == insertCompressor && p == 5) {
    static const char* n[] = {"Stereo", "Left", "Right"};
    snprintf(text, size, "%s", n[(int)v]);
  } else if (m == insertCompressor && (p == 1 || p == 2))
    snprintf(text, size, "%.2f ms", 1000.0f / (6.28318530718f * v));
  else
    snprintf(text, size, "%.2f %s", d.parameters[p].units[0] == '%' ? v * 100 : v,
             d.parameters[p].units);
}
#include "external/insert_fx/work_compressor.h"
struct InsertChain::Impl {
  static constexpr int block = 128;
  struct Slot {
    InsertConfig config{};
    uint8_t previous[8]{};
    bool dirty = true;
    int module = 0, pending = 0;
    float wet = 0;
    bool switching = false;
    WorkCompressor work;
    insert_dsp::Distortion distortion;
    insert_dsp::StereoDoubler doubler;
    insert_tape::tapescam_instance_t tape;
    ott_dsp_t* ott[2]{};
  } slots[2];
  float rate;
  explicit Impl(float sr) : rate(sr) {
    for (auto& s : slots) {
      s.ott[0] = ott_dsp_new(sr);
      s.ott[1] = ott_dsp_new(sr);
    }
  }
  ~Impl() {
    for (auto& s : slots)
      for (auto* o : s.ott) ott_dsp_free(o);
  }
  void resetDSP(Slot& s, int m) {
    s.module = m;
    s.pending = m;
    s.dirty = true;
    switch (m) {
      case insertCompressor:
        s.work = WorkCompressor{};
        s.work.rate = rate;
        break;
      case insertDistortion:
        s.distortion.reset(rate);
        break;
      case insertDoubler:
        s.doubler.reset(rate);
        break;
      case insertTape:
        insert_tape::reset(&s.tape, rate);
        break;
      case insertOTT:
        for (auto* o : s.ott) ott_dsp_reset(o);
        break;
    }
  }
  void parameters(Slot& s, const uint8_t* v) {
    if (!s.dirty && !memcmp(v, s.previous, 8)) return;
    float p[8];
    for (int i = 0; i < 8; ++i) p[i] = insertMap(s.module, i, v[i]);
    switch (s.module) {
      case insertCompressor:
        s.work.set(p);
        break;
      case insertDistortion:
        s.distortion.A = p[0] / 24 + 0.5f;
        s.distortion.B = p[1] / 4;
        s.distortion.C = p[2] / 24 + 0.5f;
        s.distortion.D = p[3];
        break;
      case insertDoubler:
        s.doubler.A = p[0];
        s.doubler.B = p[1];
        break;
      case insertTape:
        p[4] = 0.5f + 0.5f * p[4];
        insert_tape::set(&s.tape, p);
        break;
      case insertOTT: {
        auto q = ott_dsp_default_params();
        q.depth = p[0];
        q.time_scale = p[1];
        q.upward = p[2];
        q.downward = p[3];
        q.input_gain = p[4];
        q.output_gain = p[5];
        for (auto* o : s.ott) ott_dsp_set_params(o, &q);
        break;
      }
    }
    memcpy(s.previous, v, 8);
    s.dirty = false;
  }
  void process(Slot& s, float* data, int frames, const uint8_t* values) {
    float increment = 1.0f / std::max(1.0f, rate * 0.005f);
    for (int offset = 0; offset < frames;) {
      if (s.switching && s.wet <= 0) {
        resetDSP(s, s.pending);
        s.switching = false;
      }
      float target = s.switching || s.config.bypass || s.module == insertOff ? 0 : 1;
      if (s.module == insertOff || (target == 0 && s.wet == 0)) {
        offset = frames;
        continue;
      }
      int n = std::min(block, frames - offset);
      if (s.switching) n = std::min(n, std::max(1, (int)ceilf(s.wet / increment)));
      float l[block], r[block];
      for (int i = 0; i < n; ++i) {
        l[i] = data[2 * (offset + i)];
        r[i] = data[2 * (offset + i) + 1];
        if (!std::isfinite(l[i])) l[i] = data[2 * (offset + i)] = 0;
        if (!std::isfinite(r[i])) r[i] = data[2 * (offset + i) + 1] = 0;
        l[i] = std::max(-32.0f, std::min(32.0f, l[i]));
        r[i] = std::max(-32.0f, std::min(32.0f, r[i]));
      }
      parameters(s, s.switching ? s.previous : values);
      float* channels[] = {l, r};
      switch (s.module) {
        case insertCompressor:
          s.work.process(l, r, n);
          break;
        case insertDistortion:
          s.distortion.processReplacing(channels, channels, n);
          break;
        case insertDoubler:
          s.doubler.processReplacing(channels, channels, n);
          break;
        case insertTape:
          insert_tape::process(&s.tape, l, r, n);
          break;
        case insertOTT:
          ott_dsp_process_stereo(s.ott[0], s.ott[1], l, r, l, r, n);
          break;
      }
      for (int i = 0; i < n; ++i) {
        s.wet = target > s.wet ? std::min(target, s.wet + increment)
                               : std::max(target, s.wet - increment);
        for (int c = 0; c < 2; ++c) {
          float v = c ? r[i] : l[i];
          if (!std::isfinite(v)) v = 0;
          v = std::max(-32.0f, std::min(32.0f, v));
          float& dry = data[2 * (offset + i) + c];
          dry += s.wet * (v - dry);
        }
      }
      offset += n;
    }
  }
};
InsertChain::InsertChain(float rate) : impl_(new (std::nothrow) Impl(rate)) {}
InsertChain::~InsertChain() { delete impl_; }
bool InsertChain::ready() const {
  return impl_ && impl_->slots[0].ott[0] && impl_->slots[0].ott[1] && impl_->slots[1].ott[0] &&
         impl_->slots[1].ott[1];
}
void InsertChain::reset() {
  if (!ready()) return;
  for (auto& s : impl_->slots) {
    impl_->resetDSP(s, s.config.module);
    s.wet = 0;
    s.switching = false;
  }
}
uint16_t InsertChain::sync(const InsertConfig c[2], InsertAutomation* a) {
  if (!ready()) return 0;
  uint16_t cleared = 0;
  for (int slot = 0; slot < 2; ++slot) {
    auto& s = impl_->slots[slot];
    if (c[slot].module != s.config.module || c[slot].selection != s.config.selection) {
      a->valid[slot] = 0;
      cleared |= 0xffu << (slot * 8);
      s.pending = c[slot].module;
      s.switching = true;
    }
    for (int i = 0; i < 8; ++i)
      if (c[slot].values[i] != s.config.values[i] || c[slot].edits[i] != s.config.edits[i]) {
        a->valid[slot] &= ~(1 << i);
        cleared |= 1u << (slot * 8 + i);
      }
    s.config = c[slot];
  }
  return cleared;
}
bool InsertChain::active() const {
  if (!ready()) return false;
  for (auto& s : impl_->slots)
    if (s.config.module || s.module || s.switching) return true;
  return false;
}
void InsertChain::process(float* data, int frames, const uint8_t v[2][8]) {
  if (!ready()) return;
  for (int slot = 0; slot < 2; ++slot) impl_->process(impl_->slots[slot], data, frames, v[slot]);
}
