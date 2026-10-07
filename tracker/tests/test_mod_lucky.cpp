#ifdef CHOOCHOO_EXPERIMENTAL_MOD_LUCKY
#include "doctest.h"
#include "mod_lucky_fixtures.h"
#include "experimental/mod_lucky/service.h"
#include "synth/sample_voice.h"
#include "common.h"
#include "project_utils.h"
#include "app.h"
#include "corelib_gfx.h"
#include "app_ui_mock.h"
#include <filesystem>
#include <fstream>
#include <thread>
#include <numeric>
using namespace modLucky;
namespace {
struct Temp {
  std::filesystem::path path=std::filesystem::temp_directory_path()/("cct-lucky-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  Temp(){std::filesystem::create_directory(path);}
  ~Temp(){std::error_code ec;std::filesystem::remove_all(path,ec);}
};
std::shared_ptr<Candidate> candidate(luckyFixture::Bytes bytes=luckyFixture::mod()) {
  auto c=std::make_shared<Candidate>();c->id="123";c->bytes=std::move(bytes);std::atomic<bool> cancel{false};prepare(*c,48000,cancel);return c;
}
Status wait(Service& service) {
  for(int i=0;i<1000;++i) { auto s=service.status();if(!s.busy() || s.bankReady)return s;std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
  throw Error("Test worker timed out");
}
std::unique_ptr<Project> project() { auto p=std::make_unique<Project>();projectInitAY(p.get());fillFXNames();return p; }
double energy(const std::vector<float>& values){double e=0;for(float v:values)e+=v*v;return e;}
HttpResponse page(const std::string& id) {
  std::string html="<a href='noise?x=987'>other</a><script>path = \"jsplayer.php?moduleid="+id+"\";</script><a href='https://api.modarchive.org/downloads.php?moduleid="+id+"#own.mod'>Download</a>";
  return {200,{html.begin(),html.end()}};
}
}
TEST_CASE("ModLucky decoded PCM and strict validation for four formats") {
  std::atomic<bool> cancel{false};
  for(int type=0;type<4;++type) for(int bits=0;bits<2;++bits) for(int stereo=0;stereo<2;++stereo) {
    CAPTURE(type);CAPTURE(bits);CAPTURE(stereo);
    auto bytes=type==0?luckyFixture::mod():type==1?luckyFixture::xm(bits,stereo):type==2?luckyFixture::s3m(bits,stereo):luckyFixture::it(bits,stereo);
    Candidate c;c.bytes=bytes;auto decoder=prepare(c,48000,cancel);
    REQUIRE(!c.samples.empty());auto& s=c.samples[0];CHECK(s.frames==256);
    CHECK(s.channels==(type==0?1:stereo?2:1));
    CHECK(s.pcm[0]==0);CHECK(s.pcm[s.channels*8]==(type!=0 && bits?12000:11776));
    if(stereo && type!=0) CHECK(s.pcm[8*2+1]==(bits?6000:5888));
    std::vector<int16_t> preview(48000*2);render(*decoder,preview.data(),48000);
    int peak=0;for(auto v:preview)peak=std::max(peak,std::abs(int(v)));CHECK(peak>100);CHECK(peak<32767);
    bytes.pop_back();std::string format;CHECK_THROWS(inspectModule(bytes,format));
  }
  std::string format;CHECK_THROWS(inspectModule({1,2,3},format));
  CHECK_THROWS(inspectModule(std::vector<uint8_t>(maxModuleBytes+1),format));
  auto huge=luckyFixture::it(false,false);luckyFixture::le32(huge,256+48,0x7fffffff);CHECK_THROWS(inspectModule(huge,format));
}
TEST_CASE("ModLucky unsigned sample data converts without DC offset or byte swapping") {
  for(bool it:{false,true})for(bool sixteen:{false,true}) {
    auto bytes=it?luckyFixture::it(sixteen,true):luckyFixture::s3m(sixteen,true);
    if(it)bytes[256+46]=0;else luckyFixture::le16(bytes,42,2);
    for(size_t i=512;i<bytes.size();i+=sixteen?2:1)bytes[i+(sixteen?1:0)]^=128;
    auto c=candidate(bytes);CHECK(c->samples[0].pcm[0]==0);CHECK(c->samples[0].pcm[16]==(sixteen?12000:11776));
    CHECK(c->samples[0].pcm[17]==(sixteen?6000:5888));
  }
  auto bytes=luckyFixture::it(true,false);bytes[256+46]=3;std::string format;CHECK_THROWS(inspectModule(bytes,format));
  auto xm=candidate(luckyFixture::xm(true,false));CHECK(xm->samples[0].referenceRate==doctest::Approx(8363*std::pow(2.,12.25/12)));
}
TEST_CASE("ModLucky source tuning loops and WAV are preserved without pitch doubling") {
  auto c=candidate();REQUIRE(c->samples.size()==4);CHECK(c->loopFallbacks==1);
  CHECK(c->samples[0].playableLoop==0);CHECK(c->samples[1].playableLoop==1);CHECK(c->samples[2].playableLoop==0);
  CHECK(c->samples[2].loopStart==64);CHECK(c->samples[2].loopEnd==192);
  CHECK(c->samples[3].referenceRate==doctest::Approx(8287*std::pow(2.,.5/12)));
  Temp temp;auto p=project();auto snapshot=snapshotProject(*p);auto slots=availableSlots(*p);slots.resize(4);std::atomic<bool> cancel{false};
  auto bank=stageBank(c,slots,temp.path.string(),cancel);REQUIRE(commitBank(*bank,*p,snapshot));
  for(unsigned i=0;i<4;++i) {
    auto& instrument=p->instruments[slots[i]];auto& s=instrument.chip.sample;
    CHECK(instrument.type==InstrumentType::Sample);CHECK(instrument.transposeEnabled==1);CHECK(s.pitch==0);CHECK(s.slice==0);CHECK(s.speedPercent==100);CHECK(s.start==0);CHECK(s.end==255);
    CHECK(s.frameCount==256);CHECK(s.sampleRate==unsigned(std::lround(c->samples[i].referenceRate)));
    CHECK(std::equal(c->samples[i].pcm.begin(),c->samples[i].pcm.end(),s.data));
  }
  auto& sample=p->instruments[slots[1]].chip.sample;
  std::vector<float> low(4800*2),high(4800*2),semitone(4800*2);
  auto tone=[&](float cents,std::vector<float>& output){SampleVoice v;v.init(48000);v.configure(&sample,cents,1,100,0,255,1,20000,0);v.noteOn();v.render(output.data(),output.size()/2);};
  tone(0,low);tone(1200,high);tone(100,semitone);
  auto crossings=[](const std::vector<float>& a){int n=0;for(size_t i=2;i<a.size();i+=2)if(a[i-2]<=0 && a[i]>0)++n;return n;};
  CHECK(crossings(high)==doctest::Approx(crossings(low)*2).epsilon(.04));CHECK(crossings(semitone)>crossings(low));CHECK(energy(low)>1);
  for(float v:high)CHECK(std::abs(v)<1);
  projectFree(p.get());
}
TEST_CASE("ModLucky protects references configuration and tables transactionally") {
  auto p=project();getInstrumentFunctions(InstrumentType::Sample).init(&p->instruments[0]);
  p->instruments[1].name[0]='X';p->phrases[900].rows[12].instrument=2;p->tables[3].rows[0].volume=128;
  auto slots=availableSlots(*p);CHECK(std::find(slots.begin(),slots.end(),0)==slots.end());CHECK(std::find(slots.begin(),slots.end(),1)==slots.end());CHECK(std::find(slots.begin(),slots.end(),2)==slots.end());CHECK(std::find(slots.begin(),slots.end(),3)==slots.end());
  auto before=snapshotProject(*p);auto c=candidate();Temp temp;std::atomic<bool> cancel{false};slots.resize(c->samples.size());
  auto bank=stageBank(c,slots,temp.path.string(),cancel);CHECK(snapshotProject(*p)==before);
  p->tables[12].rows[0].volume=12;CHECK_FALSE(commitBank(*bank,*p,before));
  bank.reset();CHECK(std::filesystem::is_empty(temp.path/"mod_lucky"));
  std::ofstream(temp.path/"blocked")<<"file";CHECK_THROWS(stageBank(c,slots,(temp.path/"blocked").string(),cancel));
  cancel=true;CHECK_THROWS(stageBank(c,slots,temp.path.string(),cancel));CHECK(std::filesystem::is_empty(temp.path/"mod_lucky"));
  projectFree(p.get());
}
TEST_CASE("ModLucky NEXT A then B imports exact cached B without network") {
  std::atomic<int> calls{0},ids{0};
  Service s([&](const std::string& url,size_t,const std::atomic<bool>&){++calls;if(url.find("view_player")!=std::string::npos)return page(std::to_string(++ids));return HttpResponse{200,luckyFixture::mod(ids)};});
  auto p=project();auto before=snapshotProject(*p);s.enter();REQUIRE(s.next(48000));CHECK_FALSE(s.next(48000));REQUIRE(wait(s).canLoad());CHECK_FALSE(s.status().playing);
  auto a=s.status().candidate;CHECK(s.play());std::vector<float> out(2048);double previewEnergy=0;for(int i=0;i<10 && !previewEnergy;++i) { CHECK(s.audio(out.data(),1024,.5));previewEnergy+=energy(out);std::this_thread::sleep_for(std::chrono::milliseconds(1)); }CHECK(previewEnergy>0);
  REQUIRE(s.next(48000));REQUIRE(wait(s).canLoad());auto b=s.status().candidate;CHECK(a->hash!=b->hash);CHECK(b->id=="2");CHECK(snapshotProject(*p)==before);
  Temp temp;REQUIRE(s.import(*p,temp.path.string()));REQUIRE(wait(s).bankReady);REQUIRE(s.commit(*p));CHECK(calls==4);CHECK_FALSE(s.status().canLoad());CHECK_FALSE(s.import(*p,temp.path.string()));
  auto& imported=p->instruments[0].chip.sample;CHECK(std::equal(b->samples[0].pcm.begin(),b->samples[0].pcm.end(),imported.data));
  auto expected=before;auto* expectedProject=reinterpret_cast<Project*>(expected.data());
  for(int i=0;i<4;++i)expectedProject->instruments[i]=p->instruments[i];
  CHECK(snapshotProject(*p)==expected); // Every other project byte unchanged.
  projectFree(p.get());
}
TEST_CASE("ModLucky insufficient capacity and changed projects never partly import") {
  Service s;auto p=project();for(int i=0;i<PROJECT_MAX_INSTRUMENTS;++i)p->instruments[i].name[0]='X';
  auto before=snapshotProject(*p);s.enter();REQUIRE(s.fixture(luckyFixture::mod(),"100",48000));REQUIRE(wait(s).canLoad());Temp temp;
  CHECK_FALSE(s.import(*p,temp.path.string()));CHECK(snapshotProject(*p)==before);CHECK(std::filesystem::is_empty(temp.path));
  for(auto& instrument:p->instruments)instrument.name[0]=0;
  before=snapshotProject(*p);REQUIRE(s.import(*p,temp.path.string()));REQUIRE(wait(s).bankReady);
  p->title[0]='X';auto changed=snapshotProject(*p);CHECK_FALSE(s.commit(*p));CHECK(snapshotProject(*p)==changed);
  CHECK(std::filesystem::is_empty(temp.path/"mod_lucky"));projectFree(p.get());
}
TEST_CASE("ModLucky fresh process reload (opt-in)" * doctest::skip(!std::getenv("CCT_MOD_LUCKY_RELOAD"))) {
  auto p=project();REQUIRE(projectLoad(p.get(),std::getenv("CCT_MOD_LUCKY_RELOAD"))==0);
  for(int i=0;i<4;++i) {
    auto& instrument=p->instruments[i];REQUIRE(instrument.type==InstrumentType::Sample);
    REQUIRE(instrument.chip.sample.data);SampleVoice v;v.init(48000);
    v.configure(&instrument.chip.sample,0,1,100,0,255,instrument.chip.sample.loopMode,20000,0);v.noteOn();
    std::vector<float> out(2048);v.render(out.data(),1024);CHECK(energy(out)>0);
  }
  projectFree(p.get());
}
TEST_CASE("ModLucky persistence survives candidate replacement and process-style reload") {
  Temp temp;auto p=project();auto c=candidate();auto slots=availableSlots(*p);slots.resize(4);auto before=snapshotProject(*p);std::atomic<bool> cancel{false};
  auto bank=stageBank(c,slots,temp.path.string(),cancel);REQUIRE(commitBank(*bank,*p,before));
  auto saved=temp.path/"project.cct";REQUIRE(projectSave(p.get(),saved.c_str())==0);
  bank.reset();c.reset();projectFree(p.get());p=project();REQUIRE(projectLoad(p.get(),saved.c_str())==0);
  auto& sample=p->instruments[slots[1]].chip.sample;REQUIRE(sample.data);CHECK(sample.frameCount==256);
  SampleVoice voice;voice.init(48000);voice.configure(&sample,0,1,100,0,255,1,20000,0);voice.noteOn();std::vector<float> out(2048);voice.render(out.data(),1024);CHECK(energy(out)>0);projectFree(p.get());
}
TEST_CASE("ModLucky cancellation rapid controls and shutdown") {
  std::atomic<int> calls{0};Service s([&](const std::string&,size_t,const std::atomic<bool>& cancel){++calls;while(!cancel)std::this_thread::sleep_for(std::chrono::milliseconds(1));throw Error("Cancelled");return HttpResponse{};});
  s.enter();REQUIRE(s.next(48000));for(int i=0;i<100;++i)CHECK_FALSE(s.next(48000));
  std::this_thread::sleep_for(std::chrono::milliseconds(10));s.leave();s.enter();std::this_thread::sleep_for(std::chrono::milliseconds(10));CHECK_FALSE(s.status().canLoad());CHECK_FALSE(s.status().playing);CHECK(calls<=1);
  REQUIRE(s.next(48000));s.projectChanged();s.shutdown();CHECK_FALSE(s.status().playing);
}
TEST_CASE("ModLucky HTML identity host bounds errors and rate limiting") {
  auto p=page("54321");std::string html(p.body.begin(),p.body.end());CHECK(parsePlayerPage(html).id=="54321");
  CHECK_THROWS(parsePlayerPage("<a href='?moduleid=123'>not the player</a>"));CHECK_THROWS(parsePlayerPage(html+"path='jsplayer.php?moduleid=9'"));
  CHECK(allowedUrl("https://modarchive.org/index.php"));CHECK_FALSE(allowedUrl("http://modarchive.org/"));CHECK_FALSE(allowedUrl("https://modarchive.org.evil/"));CHECK_FALSE(allowedUrl("https://modarchive.org@evil/"));
  CHECK(retryDelay("120",0)==120);CHECK(retryDelay("Thu, 01 Jan 1970 00:02:00 GMT",0)==120);
  for(long code:{403,404,429,500,503})CHECK_THROWS(checkResponse({code}));
  std::atomic<int> calls{0};Service s([&](const std::string&,size_t,const std::atomic<bool>&){++calls;HttpResponse r;r.status=429;r.retryAfter="60";return r;});
  s.enter();REQUIRE(s.next(48000));CHECK(wait(s).phase==Phase::failed);CHECK_FALSE(s.next(48000));CHECK(calls==1);
  for(const char* message:{"No connection","Connection timed out"}) {
    Service offline([&](const std::string&,size_t,const std::atomic<bool>&)->HttpResponse{throw NetworkError(message);});offline.enter();REQUIRE(offline.next(48000));CHECK(wait(offline).phase==Phase::failed);
  }
}
TEST_CASE("ModLucky one Settings row edge activation and focus ownership") {
  std::atomic<int> calls{0};std::atomic<bool> release{false};
  Service s([&](const std::string& url,size_t,const std::atomic<bool>& cancel){
    ++calls;while(!release && !cancel)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return url.find("view_player")!=std::string::npos?page(std::to_string(calls.load())):HttpResponse{200,luckyFixture::mod()};
  });
  struct Restore {
    AppSettings settings=appSettings;ChipNomadState* old=chipnomadState;const AppScreen* screen=currentScreen;
    ~Restore(){testService(nullptr);chipnomadDestroy(chipnomadState);chipnomadState=old;appSettings=settings;currentScreen=screen;}
  } restore;
  chipnomadState=chipnomadCreate();projectInitAY(&chipnomadState->project);initDefaultAppSettings();testService(&s);
  currentScreen=&screenSettings;screenSettings.setup(0);screenSettings.fullRedraw();auto* data=mockScreenData;
  REQUIRE(data);CHECK(data->rows==11);CHECK(data->getColumnCount(9)==3);
  for(int row=0;row<11;++row)if(row!=9)CHECK(data->getColumnCount(row)==1);
  data->cursorRow=9;data->cursorCol=0;gfxClear();screenSettings.draw();
  CHECK(std::string(mockGfxCells[17],32)=="I'm Feeling Lucky NEXT PLAY LOAD");CHECK(mockCursorX==18);
  screenSettings.onInput(0,0,0);screenSettings.onInput(1,keyEdit,1);
  for(int i=0;i<20;++i)screenSettings.onInput(1,keyEdit,1);
  CHECK(s.status().busy());CHECK_FALSE(s.status().canLoad());release=true;REQUIRE(wait(s).canLoad());
  screenSettings.draw();CHECK(data->cursorCol==1);CHECK(mockCursorX==23);CHECK_FALSE(s.status().playing);
  screenSettings.onInput(1,keyEdit,1);CHECK_FALSE(s.status().playing); // Same held press cannot PLAY.
  screenSettings.onInput(0,0,0);screenSettings.onInput(1,keyEdit,1);CHECK(s.status().playing);CHECK(calls==2);
  screenSettings.onInput(0,0,0);data->cursorCol=0;release=false;screenSettings.onInput(1,keyEdit,1);
  screenSettings.onInput(0,0,0);screenSettings.onInput(1,keyDown,1);data->cursorRow=10;release=true;
  REQUIRE(wait(s).canLoad());screenSettings.draw();CHECK(data->cursorRow==10);CHECK(data->cursorCol==0);CHECK_FALSE(s.status().playing);
  s.leave();screenSettings.setup(0);CHECK_FALSE(s.status().playing);CHECK(calls==4);s.leave();
}
TEST_CASE("ModLucky bounded retries malformed responses and staging rollback") {
  for(bool duplicate:{false,true}) {
    std::atomic<int> calls{0};
    Service s([&](const std::string& url,size_t,const std::atomic<bool>&){++calls;return url.find("view_player")!=std::string::npos?page("42"):HttpResponse{200,{1,2,3}};});
    s.enter();if(duplicate){REQUIRE(s.fixture(luckyFixture::mod(),"42",48000));REQUIRE(wait(s).canLoad());}
    REQUIRE(s.next(48000));CHECK(wait(s).phase==Phase::failed);CHECK(calls==(duplicate?3:6));CHECK_FALSE(s.status().canLoad());
  }
  Service malformed([](const std::string&,size_t,const std::atomic<bool>&){return HttpResponse{200,{'<','h','t','m','l','>'}};});
  malformed.enter();REQUIRE(malformed.next(48000));CHECK(wait(malformed).phase==Phase::failed);
  Temp temp;auto p=project();auto c=candidate();auto slots=availableSlots(*p);slots.resize(4);auto before=snapshotProject(*p);std::atomic<bool> cancel{false};
  // Fail on the second asset after the first has been written and loaded.
  for(bool allocation:{false,true}) {
    testBankFailure(1,allocation);CHECK_THROWS(stageBank(c,slots,temp.path.string(),cancel));
    CHECK(snapshotProject(*p)==before);CHECK(std::filesystem::is_empty(temp.path/"mod_lucky"));
  }
  c->samples[1].referenceRate=0;CHECK_THROWS(stageBank(c,slots,temp.path.string(),cancel));
  CHECK(snapshotProject(*p)==before);CHECK(std::filesystem::is_empty(temp.path/"mod_lucky"));
  projectFree(p.get());
}
TEST_CASE("ModLucky natural end retains candidate and replays cached audio") {
  Service s;s.enter();REQUIRE(s.fixture(luckyFixture::xm(false,false),"42",8000));REQUIRE(wait(s).canLoad());REQUIRE(s.play());
  std::vector<float> out(2048);double total=0;
  for(int i=0;i<1000 && s.status().playing;++i){s.audio(out.data(),1024,1);total+=energy(out);std::this_thread::sleep_for(std::chrono::milliseconds(1));}
  CHECK_FALSE(s.status().playing);CHECK(s.status().canLoad());CHECK(total>0);auto id=s.status().candidate;
  REQUIRE(s.play());total=0;for(int i=0;i<1000 && s.status().playing;++i){s.audio(out.data(),1024,1);total+=energy(out);std::this_thread::sleep_for(std::chrono::milliseconds(1));}
  CHECK_FALSE(s.status().playing);CHECK(total>0);CHECK(s.status().candidate==id);
}
TEST_CASE("ModLucky live acquisition (opt-in)" * doctest::skip(!std::getenv("CCT_MOD_LUCKY_LIVE"))) {
  Service s;s.enter();REQUIRE(s.next(48000));
  Status status;for(int i=0;i<1800;++i){status=s.status();if(!status.busy())break;std::this_thread::sleep_for(std::chrono::milliseconds(100));}
  INFO(status.message);REQUIRE(status.canLoad());CHECK_FALSE(status.playing);REQUIRE(s.play());
  std::vector<float> out(4096);double total=0;for(int i=0;i<30;++i){s.audio(out.data(),2048,1);total+=energy(out);std::this_thread::sleep_for(std::chrono::milliseconds(5));}
  CHECK(total>0);std::printf("Live ModLucky: id=%s format=%s samples=%zu hash=%s energy=%.3f\n",status.candidate->id.c_str(),status.candidate->format.c_str(),status.candidate->samples.size(),status.candidate->hash.c_str(),total);
}
#endif
