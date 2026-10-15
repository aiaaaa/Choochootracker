// Host-only audit through the real USER loader and SID voice, including ZIPs.
#include "user_presets.h"
#include "sid_patch.h"
#include "synth/sid_voice.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <set>
#include <cmath>
#include <cstring>

static void wav(const std::filesystem::path& path, const std::vector<float>& pcm) {
  std::ofstream f(path, std::ios::binary);
  auto u16=[&](unsigned n){f.put(n);f.put(n>>8);};
  auto u32=[&](unsigned n){u16(n);u16(n>>16);};
  f.write("RIFF",4);u32(36+pcm.size()*2);f.write("WAVEfmt ",8);u32(16);
  u16(1);u16(1);u32(48000);u32(96000);u16(2);u16(16);
  f.write("data",4);u32(pcm.size()*2);
  for(float x:pcm)u16(uint16_t(int16_t(std::clamp(x,-1.f,1.f)*32767)));
}
int main(int argc,char** argv) {
  if(argc!=3)return 2;
  std::filesystem::path out=argv[2];std::filesystem::create_directories(out);
  UserPresets library;library.setup(argv[1],InstrumentType::SID);
  std::string error;auto sounds=library.scan(error);
  std::ofstream report(out/"audit.tsv");
  report<<"name\tfile\tformat\tpeak\trms\treleased\tunique_audio\n";
  if(!error.empty())fprintf(stderr,"SCAN: %s\n",error.c_str());
  auto p=std::make_unique<Project>();projectInit(p.get());fillFXNames();
  std::set<uint64_t> hashes;int failed=0;
  for(const auto& sound:sounds) {
    if(!library.load(sound,p.get(),0,error)){fprintf(stderr,"LOAD %s: %s\n",sound.name.c_str(),error.c_str());++failed;continue;}
    auto& patch=p->instruments[0].chip.sid;
    std::vector<float> audition;double peak=0,sum=0,squares=0;uint64_t hash=1469598103934665603ull;
    bool released=true;
    for(int rate:{44100,48000})for(int note:{48,60,72}) {
      SIDVoice v;v.init(rate);v.configure(&patch,note*100,.65f);v.noteOn();
      std::vector<float> pcm(rate*2);v.render(pcm.data(),pcm.size());
      for(float x:pcm){if(!std::isfinite(x)||std::abs(x)>=1){++failed;break;}}
      if(rate==48000&&note==60) {
        for(float x:pcm){peak=std::max(peak,double(std::abs(x)));sum+=x;squares+=x*x;
          uint32_t bits;memcpy(&bits,&x,4);hash=(hash^bits)*1099511628211ull;}
        audition=pcm;
      }
      v.noteOff();
      for(int seconds=0;seconds<32&&v.active();++seconds)v.render(pcm.data(),rate);
      released&=!v.active();
    }
    double variance=squares/96000-std::pow(sum/96000,2);
    if(variance<1e-10||!released)++failed;
    // Canonical save/reload verifies embedded tables and editable recipes alike.
    auto before=patch;auto roundtrip=(out/"roundtrip.cni").string();
    int saved=instrumentSave(p.get(),roundtrip.c_str(),0);
    int loaded=saved?1:instrumentLoad(p.get(),roundtrip.c_str(),1);
    const auto& after=p->instruments[1].chip.sid;
    // Compare serialized content, not padding or bytes beyond a string terminator.
    if(saved||loaded||before.schema!=after.schema||before.bankId!=after.bankId||
       strcmp(before.presetName,after.presetName)||memcmp(before.value,after.value,sizeof(before.value))||
       before.program.format!=after.program.format||before.program.rate!=after.program.rate||
       before.program.size!=after.program.size||memcmp(before.program.data,after.program.data,before.program.size)) {
      fprintf(stderr,"ROUNDTRIP %s: save=%d load=%d\n",sound.name.c_str(),saved,loaded);++failed;
    }
    bool unique=hashes.insert(hash).second;
    report<<sound.name<<'\t'<<sound.archive<<'/'<<sound.path<<'\t'<<int(patch.program.format)<<'\t'
          <<peak<<'\t'<<std::sqrt(std::max(0.,variance))<<'\t'<<released<<'\t'<<unique<<'\n';
    wav(out/(std::to_string(hashes.size())+"-"+std::filesystem::path(sound.path).stem().string()+".wav"),audition);
  }
  projectFree(p.get());
  printf("Sounds %zu; unique rendered sounds %zu; failures %d\n",sounds.size(),hashes.size(),failed);
  return failed?1:0;
}
