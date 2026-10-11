#ifdef WEB_BUILD
#include "service.h"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>

namespace modLucky {
namespace {
std::string retryHeader(emscripten_fetch_t* fetch) {
  size_t size=emscripten_fetch_get_response_headers_length(fetch);
  if (!size || size>16384) return {};
  std::vector<char> headers(size+1);
  emscripten_fetch_get_response_headers(fetch,headers.data(),headers.size());
  std::string text(headers.data()), lower=text;
  std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char c){return std::tolower(c);});
  size_t start=lower.find("retry-after:");
  if(start==std::string::npos) return {};
  start+=12; auto end=text.find('\r',start);
  auto result=text.substr(start,end-start);
  result.erase(0,result.find_first_not_of(" \t"));
  return result;
}
}

void Service::webCancelFetch() {
  auto* fetch=webFetch_; webFetch_=nullptr; webDone_=false; webTooLarge_=false;
  if(fetch) emscripten_fetch_close(fetch);
}

void Service::webRequest(const std::string& url,bool module) {
  webCancelFetch(); webModule_=module;
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod,"GET");
  attr.attributes=EMSCRIPTEN_FETCH_LOAD_TO_MEMORY|EMSCRIPTEN_FETCH_REPLACE;
  attr.timeoutMSecs=25000;
  attr.userData=this;
  attr.onsuccess=attr.onerror=[](emscripten_fetch_t* fetch) {
    auto& self=*static_cast<Service*>(fetch->userData);
    if(self.webFetch_==fetch) self.webDone_=true;
  };
  attr.onprogress=[](emscripten_fetch_t* fetch) {
    auto& self=*static_cast<Service*>(fetch->userData);
    if(self.webFetch_!=fetch) return;
    size_t limit=self.webModule_?maxModuleBytes:maxHtmlBytes;
    if(fetch->totalBytes>limit || fetch->dataOffset+fetch->numBytes>limit) {
      self.webCancelFetch(); self.webTooLarge_=true;
    }
  };
  webFetch_=emscripten_fetch(&attr,url.c_str());
  if(!webFetch_) throw NetworkError("Could not start download");
}

void Service::webNextCandidate() {
  if(webAttempts_++>=maxCandidates) throw Error(webLastError_);
  // Only the random HTML page needs the same-origin relay. Modules download
  // directly from the archive's existing CORS-enabled endpoint.
  webRequest("/api/lucky",false);
}

void Service::webPrepare(std::shared_ptr<Candidate> prepared) {
  webDecoder_=prepare(*prepared,rate_,cancel_);
  bool more=render(*webDecoder_,pcm_.data(),previewFrames);
  initialPcm_=pcm_; read_=0; count_=previewFrames; ended_=!more;
  candidate_=std::move(prepared); previous_=candidate_->id;
  phase_=Phase::ready; message_="Ready: "+candidate_->title;
  working_=false; ++revision_;
}

void Service::webPump() {
  if(exiting_ || !inside_ || cancel_) return;
  Job job=job_; job_=Job::none;
  try {
    if(job==Job::next) webNextCandidate();
    if(webTooLarge_) { webTooLarge_=false; throw Error("Response exceeds size limit"); }
    if(webDone_ && webFetch_) {
      HttpResponse response;
      response.status=webFetch_->status;
      response.retryAfter=retryHeader(webFetch_);
      size_t limit=webModule_?maxModuleBytes:maxHtmlBytes;
      if(webFetch_->numBytes>limit) throw Error("Response exceeds size limit");
      if(webFetch_->numBytes) response.body.assign(webFetch_->data,webFetch_->data+webFetch_->numBytes);
      bool module=webModule_;
      webCancelFetch();
      if(!response.status) throw NetworkError("No connection or browser blocked download");
      checkResponse(response);
      if(!module) {
        std::string html(response.body.begin(),response.body.end());
        auto selection=parsePlayerPage(html);
        if(selection.id==previous_) { webLastError_="Repeated candidate; try NEXT"; webNextCandidate(); return; }
        webCandidate_=std::make_shared<Candidate>();
        webCandidate_->id=selection.id; webCandidate_->generation=generation_;
        webCandidate_->originalFilename=selection.filename; webCandidate_->sourcePage=std::move(html);
        char timestamp[32]; time_t now=time(nullptr); std::tm t{};
        gmtime_r(&now,&t); strftime(timestamp,sizeof(timestamp),"%Y-%m-%dT%H:%M:%SZ",&t);
        webCandidate_->downloadedAt=timestamp;
        webRequest("https://api.modarchive.org/downloads.php?moduleid="+selection.id,true);
      } else {
        webCandidate_->bytes=std::move(response.body);
        try { webPrepare(webCandidate_); webCandidate_.reset(); }
        catch(const Error& e) {
          webLastError_=e.what(); webDecoder_.reset(); webCandidate_.reset(); webNextCandidate();
        }
      }
    }
    if(job==Job::fixture) webPrepare(std::move(fixture_));
    else if(job==Job::bank) pendingBank_=stageBank(candidate_,slots_,root_,cancel_);
    else if(job==Job::replay && webDecoder_) {
      xmp_restart_module(webDecoder_->context);
      std::array<int16_t,previewFrames*2> skip{};
      ended_=!render(*webDecoder_,skip.data(),previewFrames); working_=false;
    }
    // Fill at most one ring per UI frame, never decode or allocate in audio().
    if(active_ && webDecoder_ && !ended_) {
      std::array<int16_t,2048> chunk{};
      for(size_t work=0; work<previewFrames && count_<=previewFrames-1024 && !ended_; work+=1024) {
        bool more=render(*webDecoder_,chunk.data(),1024);
        for(size_t i=0;i<1024;++i) {
          size_t at=(read_+count_+i)%previewFrames;
          pcm_[at*2]=chunk[i*2]; pcm_[at*2+1]=chunk[i*2+1];
        }
        count_+=1024; ended_=!more;
      }
    }
  } catch(const std::exception& e) {
    webCancelFetch(); webCandidate_.reset(); active_=false;
    if(auto* network=dynamic_cast<const NetworkError*>(&e))
      if(network->retrySeconds) retryAt_=std::chrono::steady_clock::now()+std::chrono::seconds(network->retrySeconds);
    phase_=job==Job::bank?Phase::ready:Phase::failed;
    message_=e.what(); working_=false; ++revision_;
  }
}
}
#endif
