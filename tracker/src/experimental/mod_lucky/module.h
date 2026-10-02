#pragma once
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <xmp.h>

namespace modLucky {
constexpr size_t maxHtmlBytes = 256 * 1024;
constexpr size_t maxModuleBytes = 8 * 1024 * 1024;
constexpr size_t maxDecodedBytes = 16 * 1024 * 1024;
constexpr size_t maxPatternEvents = 512 * 1024; // bound decoder pattern expansion too
constexpr size_t previewFrames = 32768; // 128 KiB stereo PCM16 ring
constexpr size_t maxPreviewPcmBytes = previewFrames * 4 * 3 + 2048 * 2; // ring, replay cache, worker staging + chunk
constexpr unsigned maxCandidates = 3;
constexpr unsigned maxSamples = 256;
struct Error : std::runtime_error { using std::runtime_error::runtime_error; };
struct Sample {
  unsigned index = 0, frames = 0, channels = 1, bits = 8;
  unsigned loopStart = 0, loopEnd = 0, loopType = 0;
  unsigned sustainStart = 0, sustainEnd = 0, sustainType = 0;
  double referenceRate = 8363;
  int transpose = 0, finetune = 0;
  std::string name, limitation;
  std::vector<int16_t> pcm;
  uint8_t playableLoop = 0;
};
struct Candidate {
  uint64_t generation = 0;
  std::string id, title, format, originalFilename, credits, downloadedAt, hash, sourcePage;
  std::vector<uint8_t> bytes;
  std::vector<Sample> samples;
  unsigned loopFallbacks = 0;
};
struct Decoder {
  xmp_context context = nullptr;
  bool started = false;
  Decoder();
  ~Decoder();
  Decoder(const Decoder&) = delete;
  Decoder& operator=(const Decoder&) = delete;
};
std::string contentHash(const std::vector<uint8_t>& bytes);
std::vector<Sample> inspectModule(const std::vector<uint8_t>& bytes, std::string& format);
std::unique_ptr<Decoder> prepare(Candidate& candidate, int sampleRate, const std::atomic<bool>& cancel);
bool render(Decoder& decoder, int16_t* stereo, int frames);
void writeWav(const std::string& path, const Sample& sample);
}
