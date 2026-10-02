#pragma once
#include "bank.h"
#include "network.h"
#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>
namespace modLucky {
enum class Phase { idle, loading, ready, importing, imported, failed };
struct Status {
  Phase phase=Phase::idle;
  uint64_t generation=0, revision=0;
  bool playing=false, bankReady=false;
  std::string message;
  std::shared_ptr<const Candidate> candidate;
  bool busy() const { return phase==Phase::loading || phase==Phase::importing; }
  bool canLoad() const { return phase==Phase::ready && candidate; }
};
class Service {
 public:
  explicit Service(Fetch fetch=httpsGet);
  ~Service();
  void enter();
  void leave();
  bool next(int rate);
  bool fixture(std::vector<uint8_t> bytes,std::string id,int rate); // developer/test only
  bool play();
  void stop();
  void stopFromAudio() { active_.store(false, std::memory_order_relaxed); }
  void shutdown();
  Status status();
  bool import(const Project& project,const std::string& root);
  // Called only on UI thread, with application audio paused.
  bool commit(Project& project);
  void projectChanged();
  bool audio(float* output,size_t frames,float masterGain); // try-lock only, no allocation
 private:
  enum class Job { none, next, fixture, bank, replay };
  Fetch fetch_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::thread worker_;
  std::atomic<bool> cancel_{false};
  bool exiting_=false, inside_=false, working_=false, ended_=false;
  std::atomic<bool> active_{false};
  uint64_t generation_=0, revision_=0;
  Phase phase_=Phase::idle;
  Job job_=Job::none;
  int rate_=48000;
  std::string previous_,message_,root_;
  std::chrono::steady_clock::time_point retryAt_{};
  std::shared_ptr<Candidate> candidate_,fixture_;
  std::vector<int> slots_;
  std::vector<uint8_t> projectSnapshot_;
  std::unique_ptr<Bank> pendingBank_;
  std::array<int16_t,previewFrames*2> pcm_{}, initialPcm_{};
  size_t read_=0,count_=0;
  void run();
  bool begin(Job job,int rate);
};
Service& service();
#ifdef CHOOCHOO_MOD_LUCKY_TEST
void testService(Service* replacement);
#endif
}
