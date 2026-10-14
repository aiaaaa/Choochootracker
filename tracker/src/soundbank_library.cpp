// clang-format off
// app.h includes common.h inside a legacy C linkage block. Load the C++ API first.
#include "chipnomad_lib.h"
// clang-format on

#include "soundbank_library.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <future>
#include <set>

#include "app.h"
#include "audio_manager.h"
#include "corelib_file.h"
#include "screens/screens.h"
#include "soundbank_runtime.h"
namespace {
struct Result {
  std::vector<soundbank::BankEntry> banks;
  std::shared_ptr<const soundbank::PreparedPreset> preset;
  std::string error;
};
std::vector<soundbank::BankEntry> entries;
std::future<Result> job;
std::string status, bankId, presetId, expectedBank, expectedPreset;
int target = -1;
int preparationFrames = 0;
bool restoringProject = false;
bool refreshPending = false;
uint32_t expectedHandle = 0;
size_t cacheLimit() {
  const char* value = getenv("CHOOCHOO_SOUNDBANK_CACHE_MB");
  if (!value) return 64 * 1024 * 1024;
  char* end = nullptr;
  long n = strtol(value, &end, 10);
  return n >= 8 && n <= 1024 && end && !*end ? size_t(n) * 1024 * 1024 : 64 * 1024 * 1024;
}
soundbank::SampleCache cache(cacheLimit());
std::set<std::string> failed;
#ifdef __EMSCRIPTEN__
constexpr auto launchPolicy = std::launch::deferred;
#else
constexpr auto launchPolicy = std::launch::async;
#endif
}  // namespace
std::string soundbankRoot() {
  if (const char* root = getenv("CHOOCHOO_SOUNDBANKS"))
    if (*root) return root;
#ifdef __EMSCRIPTEN__
  return "/user/instruments/USER/soundfont";
#endif
  bool external = fileIsRunningFromAppImage();
#if defined(ANDROID_BUILD) || defined(_WIN32)
  external = true;
#endif
  if (external) {
    char directory[1024];
    if (!fileGetDefaultDirectory(directory, sizeof(directory)))
      return std::string(directory) + "/instruments/USER/soundfont";
  }
  return "instruments/USER/soundfont";
}
const std::vector<soundbank::BankEntry>& soundbankEntries() { return entries; }
bool soundbankBusy() { return job.valid(); }
const std::string& soundbankStatus() { return status; }
void soundbankRescan(bool force) {
  if (soundbankBusy()) return;
  target = -1;
  status = "Indexing SoundBanks...";
  failed.clear();
  auto root = soundbankRoot();
  preparationFrames = 2;
  job = std::async(launchPolicy, [root, force]() {
    Result r;
    try {
      r.banks = soundbank::BankIndex(root).scan(force);
    } catch (const std::exception& e) {
      r.error = e.what();
    }
    return r;
  });
}
void soundbankLoad(int instrument, const std::string& bank, const std::string& preset,
                   bool restoring) {
  if (soundbankBusy() || !chipnomadState || instrument < 0 || instrument >= PROJECT_MAX_INSTRUMENTS)
    return;
  auto& i = chipnomadState->project.instruments[instrument];
  if (i.type != InstrumentType::SoundFont) return;
  if (bank.size() >= sizeof(i.chip.soundfont.bank) ||
      preset.size() >= sizeof(i.chip.soundfont.preset)) {
    status = "Bank/preset path too long";
    return;
  }
  audioManager.pause();
  chipnomadCollectSoundbanks(chipnomadState);
  audioManager.resume();
  cache.trim();
  target = instrument;
  restoringProject = restoring;
  bankId = bank;
  presetId = preset;
  expectedBank = i.chip.soundfont.bank;
  expectedPreset = i.chip.soundfont.preset;
  expectedHandle = i.chip.soundfont.prepared;
  status = "Preparing preset...";
  auto root = soundbankRoot();
  preparationFrames = 2;
  job = std::async(launchPolicy, [root, bank, preset]() {
    Result r;
    try {
      auto b = soundbank::parseBank(soundbank::BankIndex(root).resolve(bank));
      r.preset = cache.prepare(b, preset);
    } catch (const std::exception& e) {
      r.error = e.what();
    }
    return r;
  });
}
void soundbankPoll() {
  if (refreshPending && chipnomadState && chipnomadQueueProjectRefresh(chipnomadState))
    refreshPending = false;
  if (job.valid()) {
    auto ready = job.wait_for(std::chrono::seconds(0));
    if (ready == std::future_status::deferred && preparationFrames-- > 0) return;
    if (ready != std::future_status::ready && ready != std::future_status::deferred) return;
    auto r = job.get();
    if (!r.error.empty()) {
      status = r.error;
      if (target >= 0) failed.insert(bankId + "|" + presetId);
      screenMessage(MESSAGE_TIME_ERROR, "%.100s", status.c_str());
      return;
    }
    if (target < 0) {
      entries = std::move(r.banks);
      status = std::to_string(entries.size()) + " USER banks";
      return;
    }
    auto& i = chipnomadState->project.instruments[target];
    if (i.type != InstrumentType::SoundFont || expectedBank != i.chip.soundfont.bank ||
        expectedPreset != i.chip.soundfont.preset || expectedHandle != i.chip.soundfont.prepared) {
      status = "Preset request superseded";
      return;
    }
    try {
      audioManager.pause();
      uint32_t handle = soundbank::publish(r.preset);
      strcpy(i.chip.soundfont.bank, bankId.c_str());
      strcpy(i.chip.soundfont.preset, presetId.c_str());
      i.chip.soundfont.prepared = handle;
      if (!restoringProject || !i.name[0])
        snprintf(i.name, sizeof(i.name), "%s", r.preset->name.c_str());
      refreshPending = !chipnomadQueueProjectRefresh(chipnomadState);
      audioManager.resume();
      if (!restoringProject) projectModified = 1;
      status = r.preset->warnings.empty() ? "Preset ready" : r.preset->warnings.front();
      screenMessage(MESSAGE_TIME_ERROR, "%.100s", status.c_str());
    } catch (const std::exception& e) {
      audioManager.resume();
      status = e.what();
      screenMessage(MESSAGE_TIME_ERROR, "%.100s", status.c_str());
    }
    return;
  }
  if (!chipnomadState) return;
  for (int n = 0; n < PROJECT_MAX_INSTRUMENTS; ++n) {
    auto& i = chipnomadState->project.instruments[n];
    if (i.type != InstrumentType::SoundFont) continue;
    auto& s = i.chip.soundfont;
    if (s.bank[0] && s.preset[0] && !soundbank::prepared(s.prepared) &&
        !failed.count(std::string(s.bank) + "|" + s.preset)) {
      soundbankLoad(n, s.bank, s.preset, true);
      break;
    }
  }
}

void soundbankShutdown() {
  if (job.valid()) job.get();
  soundbank::collect([](uint32_t) { return false; });
  cache.trim();
}
