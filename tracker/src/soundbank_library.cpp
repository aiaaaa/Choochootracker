// clang-format off
// app.h includes common.h inside a legacy C linkage block. Load the C++ API first.
#include "chipnomad_lib.h"
// clang-format on

#include "soundbank_library.h"
#include "soundbank_browser.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <future>
#include <map>
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
bool scanned = false;
int pendingInstrument = -1;
std::string pendingBank, pendingPreset, pendingExpectedBank, pendingExpectedPreset;
uint32_t pendingExpectedHandle=0;
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
std::map<int, std::string> instrumentErrors;
std::string scanError;
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
bool soundbankScanned() { return scanned; }
bool soundbankScanning() { return job.valid() && target < 0; }
const std::string& soundbankStatus() { return status; }
std::string soundbankInstrumentStatus(int instrument) {
  if (!chipnomadState || instrument < 0 || instrument >= PROJECT_MAX_INSTRUMENTS) return {};
  if (job.valid() && target == instrument) return "Preparing preset...";
  auto error = instrumentErrors.find(instrument);
  if (error != instrumentErrors.end()) return error->second;
  const auto& s = chipnomadState->project.instruments[instrument].chip.soundfont;
  if (soundbank::prepared(s.prepared)) return "Preset ready";
  return s.bank[0] ? "Preset not ready" : "Select a preset";
}
std::string soundbankInstrumentWarning(int instrument) {
  if (!chipnomadState || instrument < 0 || instrument >= PROJECT_MAX_INSTRUMENTS) return {};
  if (!scanError.empty()) return "Scan failed: " + scanError;
  const auto* p = soundbank::prepared(chipnomadState->project.instruments[instrument].chip.soundfont.prepared);
  if (!p || p->warnings.empty()) return {};
  for (const auto& warning : p->warnings)
    if (warning.find("custom modulators unsupported") != std::string::npos)
      return "Custom bank modulation ignored";
  return p->warnings.front();
}
void soundbankRescan(bool force) {
  if (soundbankBusy()) return;
  target = -1;
  status = "Indexing SoundBanks...";
  scanError.clear();
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
  if (!chipnomadState || instrument < 0 || instrument >= PROJECT_MAX_INSTRUMENTS) return;
  auto& i = chipnomadState->project.instruments[instrument];
  if (i.type != InstrumentType::SoundFont) return;
  if (soundbankBusy()) {
    if (!restoring) {
      pendingInstrument=instrument; pendingBank=bank; pendingPreset=preset;
      pendingExpectedBank=i.chip.soundfont.bank; pendingExpectedPreset=i.chip.soundfont.preset;
      pendingExpectedHandle=i.chip.soundfont.prepared;
    }
    return;
  }
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
  instrumentErrors.erase(instrument);
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
void soundbankCycle(int instrument, int direction) {
  if (!chipnomadState || instrument < 0 || instrument >= PROJECT_MAX_INSTRUMENTS) return;
  const auto& i = chipnomadState->project.instruments[instrument];
  if (i.type != InstrumentType::SoundFont) return;
  if (!scanned) { if (!soundbankBusy()) soundbankRescan(); return; }
  auto selected = soundbank::cyclePreset(entries,
    pendingInstrument == instrument ? pendingBank : (soundbankBusy() && target == instrument ? bankId : i.chip.soundfont.bank),
    pendingInstrument == instrument ? pendingPreset : (soundbankBusy() && target == instrument ? presetId : i.chip.soundfont.preset), direction);
  if (selected.first >= 0)
    soundbankLoad(instrument, entries[selected.first].id, entries[selected.first].presets[selected.second].first);
}
static void loadPending() {
  int next=pendingInstrument; pendingInstrument=-1;
  if (!chipnomadState || next<0) return;
  const auto& i=chipnomadState->project.instruments[next];
  if (i.type==InstrumentType::SoundFont && pendingExpectedBank==i.chip.soundfont.bank &&
      pendingExpectedPreset==i.chip.soundfont.preset && pendingExpectedHandle==i.chip.soundfont.prepared)
    soundbankLoad(next,pendingBank,pendingPreset);
}
void soundbankPoll() {
  if (refreshPending && chipnomadState && chipnomadQueueProjectRefresh(chipnomadState))
    refreshPending = false;
  if (job.valid()) {
    auto ready = job.wait_for(std::chrono::seconds(0));
    if (ready == std::future_status::deferred && preparationFrames-- > 0) return;
    if (ready != std::future_status::ready && ready != std::future_status::deferred) return;
    auto r = job.get();
    // Only the latest requested preset is published when cycling outruns decoding.
    if (pendingInstrument == target && target >= 0) {
      loadPending();
      return;
    }
    if (!r.error.empty()) {
      status = r.error;
      if (target >= 0) failed.insert(bankId + "|" + presetId);
      if (target >= 0) instrumentErrors[target] = status;
      else scanError = status;
      return;
    }
    if (target < 0) {
      entries = std::move(r.banks);
      scanned = true;
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
      status = "Preset ready";
    } catch (const std::exception& e) {
      audioManager.resume();
      status = e.what();
      instrumentErrors[target] = status;
    }
    return;
  }
  if (!chipnomadState) return;
  if (pendingInstrument >= 0) {
    loadPending(); return;
  }
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
  pendingInstrument=-1; entries.clear(); scanned=false; status.clear(); instrumentErrors.clear(); scanError.clear();
  soundbank::collect([](uint32_t) { return false; });
  cache.trim();
}
