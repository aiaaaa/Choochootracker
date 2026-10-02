#include "service.h"
#include <algorithm>
#include <cstring>
#include <cstdio>
namespace modLucky {
Service::Service(Fetch fetch):fetch_(std::move(fetch)) { worker_=std::thread(&Service::run,this); }
Service::~Service() { shutdown(); }
void Service::enter() { std::lock_guard<std::mutex> lock(mutex_); inside_=true; }
void Service::leave() {
  std::lock_guard<std::mutex> lock(mutex_); inside_=false; active_=false;
  if(pendingBank_) { pendingBank_.reset(); working_=false; phase_=Phase::ready; }
  if(working_) { cancel_=true; ++generation_; candidate_.reset(); phase_=Phase::idle; message_.clear(); }
  ++revision_; cv_.notify_all();
}
void Service::projectChanged() {
  std::lock_guard<std::mutex> lock(mutex_); active_=false;
  if(pendingBank_) { pendingBank_.reset(); working_=false; phase_=Phase::ready; message_="Project changed; import cancelled"; ++revision_; }
  if(working_) { cancel_=true; ++generation_; phase_=Phase::idle; candidate_.reset(); message_="Project changed; cancelled"; ++revision_; }
  cv_.notify_all();
}
bool Service::begin(Job job,int rate) {
  if(!inside_ || exiting_ || working_ || std::chrono::steady_clock::now()<retryAt_) return false;
  active_=false; candidate_.reset(); count_=read_=0; ended_=false;
  cancel_=false; ++generation_; ++revision_; phase_=Phase::loading; message_="Loading...";
  rate_=rate; job_=job; working_=true; cv_.notify_all(); return true;
}
bool Service::next(int rate) { std::lock_guard<std::mutex> lock(mutex_); return begin(Job::next,rate); }
bool Service::fixture(std::vector<uint8_t> bytes,std::string id,int rate) {
  auto fixture=std::make_shared<Candidate>(); fixture->bytes=std::move(bytes); fixture->id=std::move(id);
  std::lock_guard<std::mutex> lock(mutex_);
  if(!begin(Job::fixture,rate)) return false;
  fixture->generation=generation_; fixture_=std::move(fixture); return true;
}
bool Service::play() {
  std::lock_guard<std::mutex> lock(mutex_);
  if(!inside_ || working_ || !candidate_ || exiting_) return false;
  if(ended_ && !count_) {
    pcm_=initialPcm_; read_=0; count_=previewFrames; ended_=false;
    job_=Job::replay; working_=true;
  }
  active_=true; message_="Playing: "+candidate_->title; ++revision_; cv_.notify_all(); return true;
}
void Service::stop() { std::lock_guard<std::mutex> lock(mutex_); active_=false; }
void Service::shutdown() {
  { std::lock_guard<std::mutex> lock(mutex_); exiting_=true; active_=false; cancel_=true; ++generation_; cv_.notify_all(); }
  if(worker_.joinable()) worker_.join();
}
Status Service::status() {
  std::lock_guard<std::mutex> lock(mutex_);
  return {phase_,generation_,revision_,active_,bool(pendingBank_),message_,candidate_};
}
bool Service::import(const Project& project,const std::string& root) {
  std::lock_guard<std::mutex> lock(mutex_);
  if(!inside_ || working_ || phase_!=Phase::ready || !candidate_) return false;
  auto slots=availableSlots(project);
  if(slots.size()<candidate_->samples.size()) {
    message_="Need "+std::to_string(candidate_->samples.size())+" slots; "+std::to_string(slots.size())+" free"; ++revision_; return false;
  }
  slots.resize(candidate_->samples.size()); slots_=std::move(slots);
  projectSnapshot_=snapshotProject(project); root_=root;
  active_=false; phase_=Phase::importing; message_="Loading samples..."; ++revision_;
  cancel_=false; job_=Job::bank; working_=true; cv_.notify_all(); return true;
}
bool Service::commit(Project& project) {
  std::unique_ptr<Bank> bank;
  bool committed=false;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if(!pendingBank_) return false;
    bank=std::move(pendingBank_);
    if(inside_ && !cancel_ && bank->candidate==candidate_ && bank->candidate->generation==generation_)
      committed=commitBank(*bank,project,projectSnapshot_);
    phase_=committed?Phase::imported:(candidate_?Phase::ready:Phase::idle);
    message_=committed?"Loaded "+std::to_string(bank->slots.size())+" samples":"Project changed; import cancelled";
    if(committed && candidate_->loopFallbacks) message_+="; "+std::to_string(candidate_->loopFallbacks)+" loop fallbacks";
    working_=false; ++revision_;
  }
  if(committed) std::fprintf(stderr,"ModLucky: %s; bank %s\n",message_.c_str(),bank->directory.c_str());
  return committed;
}
bool Service::audio(float* out,size_t frames,float gain) {
  if(!active_.load(std::memory_order_relaxed)) return false;
  std::unique_lock<std::mutex> lock(mutex_,std::try_to_lock);
  if(!lock.owns_lock()) { std::memset(out,0,frames*2*sizeof(float)); return true; }
  if(!active_) return false;
  std::memset(out,0,frames*2*sizeof(float));
  size_t n=std::min(frames,count_);
  for(size_t i=0;i<n;++i) {
    size_t pos=(read_+i)%previewFrames;
    out[i*2]=pcm_[pos*2]*(gain/32768.0f); out[i*2+1]=pcm_[pos*2+1]*(gain/32768.0f);
  }
  read_=(read_+n)%previewFrames; count_-=n;
  if(ended_ && !count_) active_=false;
  return true;
}
void Service::run() {
  std::unique_ptr<Decoder> decoder;
  std::array<int16_t,2048> chunk{};
  for(;;) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock,[&]{return exiting_ || job_!=Job::none || active_;});
    if(exiting_) return;
    if(job_==Job::none) {
      if(ended_ || count_>previewFrames-1024 || !decoder) { cv_.wait_for(lock,std::chrono::milliseconds(5)); continue; }
      auto gen=generation_; lock.unlock(); bool more=render(*decoder,chunk.data(),1024); lock.lock();
      if(gen!=generation_ || cancel_) continue;
      for(size_t i=0;i<1024;++i) { size_t at=(read_+count_+i)%previewFrames; pcm_[at*2]=chunk[i*2]; pcm_[at*2+1]=chunk[i*2+1]; }
      count_+=1024; ended_=!more; continue;
    }
    Job job=job_; job_=Job::none; auto gen=generation_; int rate=rate_; auto previous=previous_;
    auto fixture=std::move(fixture_); auto chosen=candidate_; auto slots=slots_; auto root=root_;
    lock.unlock();
    try {
      if(job==Job::replay) {
        // Replay cached audio immediately; decoder restart stays on its owning worker.
        xmp_restart_module(decoder->context);
        std::array<int16_t,previewFrames*2> skip{};
        bool more=render(*decoder,skip.data(),previewFrames);
        lock.lock();
        if(gen==generation_) ended_=!more;
        working_=false;
        continue;
      }
      if(job==Job::bank) {
        auto bank=stageBank(chosen,slots,root,cancel_);
        lock.lock();
        if(gen==generation_ && inside_ && !cancel_) pendingBank_=std::move(bank);
        else { working_=false; lock.unlock(); bank.reset(); }
        continue;
      }
      decoder.reset();
      std::shared_ptr<Candidate> prepared;
      std::string last="No supported candidate";
      for(unsigned attempt=0;attempt<(job==Job::fixture?1:maxCandidates);++attempt) {
        if(cancel_) throw Error("Cancelled");
        prepared=job==Job::fixture?fixture:acquire(previous,gen,cancel_,fetch_);
        if(!prepared) { last="Repeated candidate; try NEXT"; continue; }
        try { decoder=prepare(*prepared,rate,cancel_); break; }
        catch(const Error& e) { last=e.what(); prepared.reset(); }
      }
      if(!prepared || !decoder) throw Error(last);
      // Preparing includes the initial bounded audio buffer; readiness means PLAY can consume it now.
      std::array<int16_t,previewFrames*2> initial{};
      bool more=render(*decoder,initial.data(),previewFrames);
      lock.lock();
      if(gen!=generation_ || !inside_ || cancel_) { working_=false; lock.unlock(); decoder.reset(); continue; }
      pcm_=initial; initialPcm_=initial; read_=0; count_=previewFrames; ended_=!more;
      candidate_=std::move(prepared); previous_=candidate_->id;
      phase_=Phase::ready; message_="Ready: "+candidate_->title; working_=false; ++revision_;
    } catch(const NetworkError& e) {
      if(!lock.owns_lock()) lock.lock();
      if(e.retrySeconds) retryAt_=std::chrono::steady_clock::now()+std::chrono::seconds(e.retrySeconds);
      if(gen==generation_) { phase_=Phase::failed; message_=e.what(); candidate_.reset(); ++revision_; }
      working_=false;
    } catch(const std::exception& e) {
      if(!lock.owns_lock()) lock.lock();
      if(gen==generation_) { phase_=job==Job::bank?Phase::ready:Phase::failed; message_=e.what(); ++revision_; }
      working_=false;
    }
  }
}
#ifdef CHOOCHOO_MOD_LUCKY_TEST
static Service* testInstance=nullptr;
void testService(Service* replacement) { testInstance=replacement; }
#endif
Service& service() {
#ifdef CHOOCHOO_MOD_LUCKY_TEST
  if(testInstance) return *testInstance;
#endif
  static Service instance; return instance;
}
}
