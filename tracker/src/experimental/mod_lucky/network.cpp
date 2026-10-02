#include "network.h"
#include <curl/curl.h>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <mutex>
#include <regex>

namespace modLucky {
bool allowedUrl(const std::string& url) {
  return url.rfind("https://modarchive.org/",0)==0 || url.rfind("https://api.modarchive.org/",0)==0;
}
PageSelection parsePlayerPage(const std::string& html) {
  if(html.size()>maxHtmlBytes) throw Error("Player page too large");
  // The player-selected path is authoritative, not an arbitrary link/number.
  const std::regex selected(R"re(\bpath\s*=\s*["']jsplayer\.php\?moduleid=([1-9][0-9]{0,8})["'])re");
  std::sregex_iterator i(html.begin(),html.end(),selected),end;
  if(i==end) throw Error("Random player page changed");
  PageSelection result{(*i)[1].str(),""};
  for(++i;i!=end;++i) if((*i)[1].str()!=result.id) throw Error("Ambiguous random player page");
  const std::regex download("https://api\\.modarchive\\.org/downloads\\.php\\?moduleid="+result.id+R"re((?:#([^"'<>\s]*))?["'])re");
  std::smatch link;
  if(!std::regex_search(html,link,download)) throw Error("Player/download identity mismatch");
  if(link[1].matched) result.filename=link[1].str().substr(0,255); // metadata only, never a path
  return result;
}
unsigned retryDelay(const std::string& value,time_t now) {
  if(value.empty()) return 60;
  if(std::all_of(value.begin(),value.end(),[](unsigned char c){return std::isdigit(c);})) {
    try { return static_cast<unsigned>(std::min<uint64_t>(std::stoull(value),31536000)); } catch(...) { return 31536000; }
  }
  time_t date=curl_getdate(value.c_str(),nullptr);
  return date>now ? static_cast<unsigned>(std::min<int64_t>(date-now,31536000)) : 60;
}
namespace {
struct Transfer { HttpResponse response; size_t limit; const std::atomic<bool>& cancel; bool oversized=false, allocationFailed=false; };
size_t body(char* data,size_t a,size_t b,void* opaque) {
  auto& t=*static_cast<Transfer*>(opaque); size_t n=a*b;
  if(t.cancel) return 0;
  if(n>t.limit-t.response.body.size()) { t.oversized=true; return 0; }
  try { t.response.body.insert(t.response.body.end(),data,data+n); } catch(...) { t.allocationFailed=true; return 0; }
  return n;
}
size_t header(char* data,size_t a,size_t b,void* opaque) {
  auto& t=*static_cast<Transfer*>(opaque); size_t n=a*b;
  if(n>8192) return 0;
  try {
    std::string line(data,n); auto colon=line.find(':');
    if(colon!=std::string::npos) {
      std::string key=line.substr(0,colon),value=line.substr(colon+1);
      std::transform(key.begin(),key.end(),key.begin(),[](unsigned char c){return std::tolower(c);});
      auto start=value.find_first_not_of(" \t\r\n"),stop=value.find_last_not_of(" \t\r\n");
      value=start==std::string::npos?"":value.substr(start,stop-start+1);
      if(key=="location") t.response.location=value;
      else if(key=="retry-after") t.response.retryAfter=value;
      else if(key=="content-disposition") t.response.disposition=value;
    }
  } catch(...) { return 0; }
  return n;
}
int progress(void* opaque,curl_off_t,curl_off_t,curl_off_t,curl_off_t) { return static_cast<Transfer*>(opaque)->cancel?1:0; }
}
HttpResponse httpsGet(const std::string& first,size_t limit,const std::atomic<bool>& cancel) {
  static std::once_flag init;
  std::call_once(init,[]{ if(curl_global_init(CURL_GLOBAL_DEFAULT)) throw NetworkError("HTTPS unavailable"); });
  std::string url=first;
  const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(25);
  for(unsigned redirect=0;redirect<=3;++redirect) {
    if(cancel) throw Error("Cancelled");
    if(!allowedUrl(url)) throw NetworkError("Unexpected download host");
    long remaining=std::chrono::duration_cast<std::chrono::milliseconds>(deadline-std::chrono::steady_clock::now()).count();
    if(remaining<=0) throw NetworkError("Connection timed out");
    std::unique_ptr<CURL,decltype(&curl_easy_cleanup)> curl(curl_easy_init(),curl_easy_cleanup);
    if(!curl) throw NetworkError("HTTPS allocation failed");
    Transfer t{{},limit,cancel}; auto c=curl.get();
    curl_easy_setopt(c,CURLOPT_URL,url.c_str());
    curl_easy_setopt(c,CURLOPT_USERAGENT,"ChooChooTracker-ModLucky/0.1 (personal experiment; aiaaaa)");
    curl_easy_setopt(c,CURLOPT_SSL_VERIFYPEER,1L); curl_easy_setopt(c,CURLOPT_SSL_VERIFYHOST,2L);
    curl_easy_setopt(c,CURLOPT_CONNECTTIMEOUT_MS,5000L); curl_easy_setopt(c,CURLOPT_TIMEOUT_MS,remaining);
    curl_easy_setopt(c,CURLOPT_NOSIGNAL,1L); curl_easy_setopt(c,CURLOPT_FOLLOWLOCATION,0L);
    curl_easy_setopt(c,CURLOPT_PROTOCOLS,CURLPROTO_HTTPS);
    curl_easy_setopt(c,CURLOPT_WRITEFUNCTION,body); curl_easy_setopt(c,CURLOPT_WRITEDATA,&t);
    curl_easy_setopt(c,CURLOPT_HEADERFUNCTION,header); curl_easy_setopt(c,CURLOPT_HEADERDATA,&t);
    curl_easy_setopt(c,CURLOPT_XFERINFOFUNCTION,progress); curl_easy_setopt(c,CURLOPT_XFERINFODATA,&t); curl_easy_setopt(c,CURLOPT_NOPROGRESS,0L);
    CURLcode code=curl_easy_perform(c); curl_easy_getinfo(c,CURLINFO_RESPONSE_CODE,&t.response.status);
    if(cancel) throw Error("Cancelled");
    if(t.oversized) throw Error("Response exceeds size limit");
    if(t.allocationFailed) throw Error("Not enough memory");
    if(code!=CURLE_OK) throw NetworkError(code==CURLE_OPERATION_TIMEDOUT?"Connection timed out":"No connection");
    if(t.response.status>=300 && t.response.status<400) {
      if(redirect==3 || t.response.location.empty()) throw NetworkError("Too many redirects");
      auto origin=url.substr(0,url.find('/',8));
      url=t.response.location;
      if(url.size()>1 && url[0]=='/' && url[1]!='/') url=origin+url;
      continue;
    }
    return std::move(t.response);
  }
  throw NetworkError("Too many redirects");
}
void checkResponse(const HttpResponse& r) {
  if(r.status==200) return;
  const unsigned delay=r.retryAfter.empty() ? (r.status==429?60:0) : retryDelay(r.retryAfter,time(nullptr));
  if(r.status==403) throw NetworkError("Archive denied access",delay);
  if(r.status==404) throw NetworkError("Module not found",delay);
  if(r.status==429) throw NetworkError("Rate limited; try later",delay);
  if(r.status>=500) throw NetworkError("Archive unavailable",delay);
  throw NetworkError("Unexpected HTTP response",delay);
}
std::shared_ptr<Candidate> acquire(const std::string& previous,uint64_t generation,const std::atomic<bool>& cancel,const Fetch& fetch) {
  auto page=fetch("https://modarchive.org/index.php?request=view_player&query=random",maxHtmlBytes,cancel); checkResponse(page);
  if(page.body.size()>maxHtmlBytes) throw Error("Player page too large");
  std::string html(page.body.begin(),page.body.end()); auto selection=parsePlayerPage(html);
  if(selection.id==previous) return {};
  auto response=fetch("https://api.modarchive.org/downloads.php?moduleid="+selection.id,maxModuleBytes,cancel); checkResponse(response);
  if(response.body.size()>maxModuleBytes) throw Error("Module too large");
  auto result=std::make_shared<Candidate>(); result->id=selection.id; result->generation=generation;
  result->originalFilename=selection.filename; result->bytes=std::move(response.body); result->sourcePage=std::move(html);
  char timestamp[32]; time_t now=time(nullptr); std::tm t{};
  gmtime_r(&now,&t); strftime(timestamp,sizeof(timestamp),"%Y-%m-%dT%H:%M:%SZ",&t); result->downloadedAt=timestamp;
  return result;
}
}
