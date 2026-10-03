// Host execution of the Vita-guarded Lucky backend/path code. This does not
// emulate Vita newlib or networking; the separate hardware checklist still applies.
#include "mod_lucky_fixtures.h"
#include "experimental/mod_lucky/service.h"
#include "synth/sample_voice.h"
#include "project_utils.h"
#include "common.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <thread>
#include <cstdio>
static std::size_t available=256*1024*1024;
std::size_t vitaHeapAvailable() { return available; }
namespace fs=std::filesystem;
using namespace modLucky;
static Status wait(Service& s) {
  for(int i=0;i<2000;++i) { auto status=s.status(); if(!status.busy() || status.bankReady) return status; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
  throw Error("Vita configuration fixture timed out");
}
static void audible(Project& project) {
  unsigned count=0;
  for(auto& instrument:project.instruments) if(instrument.type==InstrumentType::Sample) {
    const auto& sample=instrument.chip.sample;
    assert(sample.data && sample.frameCount==256);
    assert(std::string(sample.path).rfind("ux0:data/choochootracker/samples/mod_lucky/102-",0)==0);
    SampleVoice voice; voice.init(48000);
    voice.configure(&sample,0,1,100,0,255,sample.loopMode,20000,0); voice.noteOn();
    std::vector<float> out(2048); voice.render(out.data(),1024);
    double energy=0; for(float value:out) energy+=value*value;
    assert(energy>0); ++count;
  }
  assert(count==4);
}
int main(int argc,char** argv) {
  assert(argc==3);
  const bool prepareBank=std::string(argv[1])=="prepare";
  const auto root=fs::absolute(argv[2]);
  if(prepareBank) {
    if(fs::exists(root)) {
      assert(fs::exists(root/"self-authored-fixture")); fs::remove_all(root);
    }
    fs::create_directories(root);
    std::ofstream(root/"self-authored-fixture")<<"ChooChoo Vita host fixture only\n";
  }
  fs::current_path(root);
  auto project=std::make_unique<Project>(); projectInitAY(project.get()); fillFXNames();
  if(prepareBank) {
    fs::create_directories("ux0:data/choochootracker/samples");
    unsigned networkCalls=0;
    Service service([&](const std::string&,size_t,const std::atomic<bool>&)->HttpResponse { ++networkCalls; throw Error("Fixture attempted network"); });
    service.enter();
    auto unchanged=snapshotProject(*project);
    available=1024;
    assert(service.fixture(luckyFixture::mod(),"100",48000));
    assert(wait(service).phase==Phase::failed);
    assert(snapshotProject(*project)==unchanged);
    available=256*1024*1024;
    assert(service.fixture(luckyFixture::mod(),"101",48000));
    assert(wait(service).phase==Phase::ready);
    assert(service.play());
    assert(service.fixture(luckyFixture::mod(1),"102",48000));
    auto ready=wait(service); assert(ready.phase==Phase::ready && !ready.playing);
    assert(ready.candidate->id=="102"); assert(service.play());
    std::vector<float> preview(8192); assert(service.audio(preview.data(),4096,1));
    double energy=0; for(float value:preview) energy+=value*value; assert(energy>0);
    assert(snapshotProject(*project)==unchanged);
    assert(service.import(*project,"ux0:data/choochootracker/samples"));
    assert(wait(service).bankReady); assert(service.commit(*project));
    assert(networkCalls==0); assert(projectSave(project.get(),"song.cct")==0);
    service.leave(); service.shutdown(); audible(*project);
  } else {
    assert(projectLoad(project.get(),"song.cct")==0); audible(*project);
  }
  projectFree(project.get());
  printf("Vita-guarded Lucky host fixture %s passed (hardware filesystem/HTTPS still pending)\n",argv[1]);
}
