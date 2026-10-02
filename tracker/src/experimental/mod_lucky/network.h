#pragma once
#include "module.h"
#include <functional>
#include <chrono>
namespace modLucky {
struct HttpResponse { long status=0; std::vector<uint8_t> body; std::string location, retryAfter, disposition; };
struct NetworkError : Error {
  unsigned retrySeconds;
  NetworkError(const std::string& message,unsigned seconds=0):Error(message),retrySeconds(seconds){}
};
using Fetch = std::function<HttpResponse(const std::string&,size_t,const std::atomic<bool>&)>;
struct PageSelection { std::string id, filename; };
PageSelection parsePlayerPage(const std::string& html);
bool allowedUrl(const std::string& url);
unsigned retryDelay(const std::string& value,time_t now);
HttpResponse httpsGet(const std::string& url,size_t limit,const std::atomic<bool>& cancel);
void checkResponse(const HttpResponse& response);
std::shared_ptr<Candidate> acquire(const std::string& previous,uint64_t generation,
                                  const std::atomic<bool>& cancel,const Fetch& fetch);
}
