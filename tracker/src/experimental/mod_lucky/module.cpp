#ifdef VITA_BUILD
#include "../../../platforms/vita/platform.h"
#endif
#include "module.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace modLucky {
namespace {
struct Reader {
  const std::vector<uint8_t>& b;
  void span(size_t p, size_t n) const { if (p > b.size() || n > b.size()-p) throw Error("Truncated module"); }
  unsigned u8(size_t p) const { span(p,1); return b[p]; }
  unsigned u16(size_t p) const { return u8(p) | u8(p+1)<<8; }
  unsigned u32(size_t p) const { return u16(p) | u16(p+2)<<16; }
  unsigned be16(size_t p) const { return u8(p)*256+u8(p+1); }
  std::string text(size_t p, size_t n) const { span(p,n); std::string s; for(size_t i=0;i<n && b[p+i];++i) s += b[p+i]>=32 && b[p+i]<127 ? char(b[p+i]) : ' '; return s; }
  bool magic(size_t p, const char* s, size_t n) const { return p<=b.size() && n<=b.size()-p && !memcmp(b.data()+p,s,n); }
};
void budget(std::vector<Sample>& samples) {
  size_t total = 0;
  if(samples.size()>maxSamples) throw Error("Too many sample slots");
  for(auto& s:samples) {
    if(s.channels>2 || !s.channels || (s.bits!=8 && s.bits!=16)) throw Error("Unsupported PCM");
    const uint64_t bytes=uint64_t(s.frames)*s.channels*2;
    if(bytes>maxDecodedBytes-total) throw Error("Sample bank over 16 MiB");
    total+=bytes;
    if(s.frames && (!std::isfinite(s.referenceRate) || s.referenceRate<1000 || s.referenceRate>192000)) throw Error("Unsupported sample tuning");
    if(s.loopType && (s.loopStart>=s.loopEnd || s.loopEnd>s.frames)) throw Error("Invalid sample loop");
    if(s.sustainType && (s.sustainStart>=s.sustainEnd || s.sustainEnd>s.frames)) throw Error("Invalid sustain loop");
  }
}
}
std::string contentHash(const std::vector<uint8_t>& bytes) {
  // Identity checksum, not a security/authenticity claim. Bytes remain immutable.
  uint64_t hash=14695981039346656037ULL;
  for(auto b:bytes) { hash^=b; hash*=1099511628211ULL; }
  std::ostringstream out; out<<std::hex<<std::setfill('0')<<std::setw(16)<<hash; return out.str();
}
std::vector<Sample> inspectModule(const std::vector<uint8_t>& bytes, std::string& format) {
  if(bytes.empty() || bytes.size()>maxModuleBytes) throw Error("Module empty or over 8 MiB");
  Reader r{bytes}; std::vector<Sample> samples;
  size_t events=0;
  auto patternBudget=[&](size_t count) {
    if(count>maxPatternEvents-events) throw Error("Module patterns exceed memory limit");
    events+=count;
  };
  if(r.magic(0,"Extended Module: ",17)) {
    format="XM"; r.span(0,80);
    if(r.u16(58)!=0x104) throw Error("Need XM 1.04");
    unsigned channels=r.u16(68), patterns=r.u16(70), instruments=r.u16(72);
    if(!channels || channels>32 || patterns>256 || instruments>128) throw Error("XM exceeds limits");
    size_t p=60+r.u32(60); r.span(60,r.u32(60));
    for(unsigned i=0;i<patterns;++i) {
      r.span(p,9); unsigned h=r.u32(p), rows=r.u16(p+5), n=r.u16(p+7);
      if(h<9 || !rows || rows>256) throw Error("Invalid XM pattern");
      patternBudget(size_t(rows)*channels);
      r.span(p,size_t(h)+n); p+=h+n;
    }
    for(unsigned i=0;i<instruments;++i) {
      r.span(p,29); unsigned h=r.u32(p), count=r.u16(p+27);
      if(h<29 || count>32 || samples.size()+count>maxSamples) throw Error("Invalid XM instrument");
      r.span(p,h); std::string name=r.text(p+4,22);
      unsigned sh=count ? r.u32(p+29):0;
      if(count && (h<33 || sh<40 || sh>256)) throw Error("Invalid XM sample header");
      p+=h; size_t data=p+size_t(count)*sh; r.span(p,size_t(count)*sh);
      for(unsigned j=0;j<count;++j) {
        size_t at=p+j*sh; Sample s; s.index=samples.size();
        unsigned len=r.u32(at), flags=r.u8(at+14);
        if(flags & ~0x33U || (flags&3)==3) throw Error("Unsupported XM sample");
        s.bits=flags&16?16:8; s.channels=flags&32?2:1;
        unsigned stride=s.bits/8*s.channels;
        if(len%stride || r.u32(at+4)%stride || r.u32(at+8)%stride) throw Error("Invalid PCM frame alignment");
        s.frames=len/stride; s.loopStart=r.u32(at+4)/stride;
        s.loopEnd=s.loopStart+r.u32(at+8)/stride; s.loopType=flags&3;
        s.finetune=int8_t(r.u8(at+13)); s.transpose=int8_t(r.u8(at+16));
        s.referenceRate=8363.0*std::pow(2.0,(s.transpose+s.finetune/128.0)/12.0);
        s.name=r.text(at+18,22); if(s.name.empty()) s.name=name;
        r.span(data,len); if(r.magic(data,"OggS",4)) throw Error("Packed XM is unsupported");
        data+=len; samples.push_back(std::move(s));
      }
      p=data;
    }
  } else if(r.magic(0,"IMPM",4)) {
    format="IT"; r.span(0,192);
    unsigned orders=r.u16(32), instruments=r.u16(34), count=r.u16(36), patterns=r.u16(38);
    if(orders>256 || instruments>256 || count>maxSamples || patterns>256) throw Error("IT exceeds limits");
    size_t ptr=192+orders; r.span(ptr,size_t(instruments+count+patterns)*4);
    for(unsigned i=0;i<instruments;++i) { auto p=r.u32(ptr+i*4); if(p) { r.span(p,554); if(!r.magic(p,"IMPI",4)) throw Error("Invalid IT instrument"); } }
    ptr+=instruments*4;
    for(unsigned i=0;i<count;++i) {
      size_t p=r.u32(ptr+i*4); r.span(p,80);
      if(!r.magic(p,"IMPS",4)) throw Error("Invalid IT sample");
      Sample s; s.index=i; unsigned flags=r.u8(p+18), convert=r.u8(p+46);
      // R1 deliberately accepts uncompressed embedded PCM only.
      // The pinned loader handles plain signed/unsigned little-endian IT PCM.
      // It does not apply the obsolete endian/delta conversion bits here.
      if(flags&8 || convert&~1U) throw Error("IT sample representation unsupported");
      s.bits=flags&2?16:8; s.channels=flags&4?2:1; s.frames=flags&1?r.u32(p+48):0;
      s.name=r.text(p+20,26); s.referenceRate=r.u32(p+60);
      s.loopStart=r.u32(p+52); s.loopEnd=r.u32(p+56); s.loopType=flags&16?(flags&64?2:1):0;
      s.sustainStart=r.u32(p+64); s.sustainEnd=r.u32(p+68); s.sustainType=flags&32?(flags&128?2:1):0;
      r.span(r.u32(p+72),uint64_t(s.frames)*s.channels*(s.bits/8)); samples.push_back(std::move(s));
    }
    ptr+=count*4;
    for(unsigned i=0;i<patterns;++i) { auto p=r.u32(ptr+i*4); if(p) { r.span(p,8); if(r.u16(p+2)>256) throw Error("IT pattern too long"); patternBudget(size_t(r.u16(p+2))*64); r.span(p,8+r.u16(p)); } }
  } else if(r.magic(44,"SCRM",4)) {
    format="S3M"; r.span(0,96);
    unsigned orders=r.u16(32), count=r.u16(34), patterns=r.u16(36);
    if(orders>256 || count>maxSamples || patterns>256) throw Error("S3M exceeds limits");
    size_t ptr=96+orders; r.span(ptr,(count+patterns)*2);
    for(unsigned i=0;i<count;++i) {
      size_t p=r.u16(ptr+i*2)*16; r.span(p,80); Sample s; s.index=i;
      unsigned type=r.u8(p); if(type>1) throw Error("S3M synth patches unsupported");
      s.name=r.text(p+48,28);
      if(type==1) {
        if(!r.magic(p+76,"SCRS",4) || r.u8(p+30)) throw Error("Packed S3M unsupported");
        unsigned flags=r.u8(p+31); if(flags&~7U) throw Error("Unsupported S3M sample");
        s.bits=flags&4?16:8; s.channels=flags&2?2:1; s.frames=r.u32(p+16);
        s.loopStart=r.u32(p+20); s.loopEnd=r.u32(p+24); s.loopType=flags&1;
        s.referenceRate=r.u32(p+32);
        size_t data=(r.u8(p+13)*65536+r.u16(p+14))*16ULL;
        r.span(data,uint64_t(s.frames)*s.channels*(s.bits/8));
      }
      samples.push_back(std::move(s));
    }
    ptr+=count*2; patternBudget(size_t(patterns)*64*32);
    for(unsigned i=0;i<patterns;++i) { auto p=r.u16(ptr+i*2)*16; if(p) { r.span(p,2); r.span(p+2,r.u16(p)); } }
  } else {
    format="MOD"; r.span(0,1084);
    std::string sig=r.text(1080,4); unsigned channels=0;
    if(sig=="M.K." || sig=="M!K!" || sig=="FLT4" || sig=="4CHN") channels=4;
    else if(sig.size()==4 && sig.substr(1)=="CHN" && sig[0]>='1' && sig[0]<='9') channels=sig[0]-'0';
    else if(sig.size()==4 && sig.substr(2)=="CH" && sig[0]>='1' && sig[0]<='3' && sig[1]>='0' && sig[1]<='9') channels=(sig[0]-'0')*10+sig[1]-'0';
    if(!channels || channels>32) throw Error("Need self-contained MOD/XM/S3M/IT");
    unsigned orders=r.u8(950), patterns=0;
    if(!orders || orders>128) throw Error("Invalid MOD orders");
    for(unsigned i=0;i<128;++i) { unsigned v=r.u8(952+i); if(v>127) throw Error("Invalid MOD pattern"); patterns=std::max(patterns,v+1); }
    patternBudget(size_t(patterns)*64*channels);
    size_t p=1084+size_t(patterns)*64*channels*4; r.span(1084,p-1084);
    for(unsigned i=0;i<31;++i) {
      size_t at=20+i*30; Sample s; s.index=i; s.name=r.text(at,22); s.frames=r.be16(at+22)*2;
      int fine=r.u8(at+24)&15; if(fine>=8) fine-=16; s.finetune=fine*16;
      s.referenceRate=8287.0*std::pow(2.0,fine/96.0);
      s.loopStart=r.be16(at+26)*2; s.loopEnd=s.loopStart+r.be16(at+28)*2;
      s.loopType=r.be16(at+28)>1?1:0;
      r.span(p,s.frames); p+=s.frames; samples.push_back(std::move(s));
    }
  }
  budget(samples); return samples;
}
Decoder::Decoder():context(xmp_create_context()) { if(!context) throw Error("Decoder allocation failed"); }
Decoder::~Decoder() { if(context) { if(started) xmp_end_player(context); xmp_release_module(context); xmp_free_context(context); } }
std::unique_ptr<Decoder> prepare(Candidate& candidate,int rate,const std::atomic<bool>& cancel) {
#ifdef VITA_BUILD
  constexpr size_t preparationHeadroom=96*1024*1024;
  if(vitaHeapAvailable()<preparationHeadroom) throw Error("Not enough memory for module");
#endif
  auto slots=inspectModule(candidate.bytes,candidate.format);
  if(cancel) throw Error("Cancelled");
  auto decoder=std::make_unique<Decoder>();
  if(xmp_load_module_from_memory(decoder->context,candidate.bytes.data(),candidate.bytes.size())<0) throw Error("Invalid or unsupported module");
  xmp_module_info info{}; xmp_get_module_info(decoder->context,&info);
  if(!info.mod || info.mod->smp!=int(slots.size())) throw Error("Sample layout unsupported");
  candidate.title=std::string(info.mod->name,strnlen(info.mod->name,XMP_NAME_SIZE));
  candidate.credits=info.comment?std::string(info.comment,strnlen(info.comment,16384)):"";
  candidate.hash=contentHash(candidate.bytes); candidate.samples.clear(); candidate.loopFallbacks=0;
  for(auto& s:slots) {
    if(cancel) throw Error("Cancelled");
    if(!s.frames) continue;
    auto& src=info.mod->xxs[s.index];
    if(src.len!=int(s.frames) || !src.data || src.flg&XMP_SAMPLE_SYNTH ||
       ((src.flg&XMP_SAMPLE_STEREO)?2U:1U)!=s.channels || ((src.flg&XMP_SAMPLE_16BIT)?16U:8U)!=s.bits) throw Error("Incomplete or unsupported PCM");
    s.pcm.resize(size_t(s.frames)*s.channels);
    for(size_t i=0;i<s.pcm.size();++i) {
      if(s.bits==16) std::memcpy(&s.pcm[i],src.data+i*2,2); // decoder: signed native-endian, interleaved
      else s.pcm[i]=int16_t(int(int8_t(src.data[i]))*256);
    }
    if(s.loopType && !s.sustainType && s.loopStart==0 && s.loopEnd==s.frames) s.playableLoop=s.loopType;
    else if(s.loopType || s.sustainType) { s.limitation="Original sustain/partial loop retained as metadata; playback loop off"; ++candidate.loopFallbacks; }
    candidate.samples.push_back(std::move(s));
  }
  if(candidate.samples.empty()) throw Error("No importable PCM samples");
  if(cancel) throw Error("Cancelled");
  if(rate<8000 || rate>192000 || xmp_start_player(decoder->context,rate,0)<0) throw Error("Cannot prepare playback");
  decoder->started=true; return decoder;
}
bool render(Decoder& decoder,int16_t* out,int frames) {
  std::memset(out,0,size_t(frames)*4);
  return xmp_play_buffer(decoder.context,out,frames*4,1)==0; // one traversal, no automatic next
}
void writeWav(const std::string& path,const Sample& s) {
  if(!std::isfinite(s.referenceRate) || s.referenceRate<1000 || s.referenceRate>192000 ||
     !s.frames || s.channels<1 || s.channels>2 || s.pcm.size()!=size_t(s.frames)*s.channels)
    throw Error("Invalid staged PCM");
  std::ofstream f(path,std::ios::binary|std::ios::trunc); if(!f) throw Error("Cannot write sample bank");
  auto u16=[&](unsigned n){ f.put(n&255); f.put((n>>8)&255); };
  auto u32=[&](uint32_t n){ u16(n&65535); u16(n>>16); };
  unsigned loops=(s.loopType?1:0)+(s.sustainType?1:0), smpl=36+loops*24;
  unsigned rate=unsigned(std::lround(s.referenceRate)), bytes=s.pcm.size()*2;
  f.write("RIFF",4); u32(36+bytes+8+smpl); f.write("WAVEfmt ",8); u32(16); u16(1); u16(s.channels);
  u32(rate); u32(rate*s.channels*2); u16(s.channels*2); u16(16);
  f.write("data",4); u32(bytes); for(int16_t v:s.pcm) u16(uint16_t(v));
  f.write("smpl",4); u32(smpl); u32(0); u32(0); u32(1000000000U/rate); u32(60); u32(0); u32(0); u32(0); u32(loops); u32(0);
  auto loop=[&](unsigned id,unsigned type,unsigned start,unsigned end){ u32(id); u32(type==2?1:0); u32(start); u32(end-1); u32(0); u32(0); };
  if(s.loopType) loop(0,s.loopType,s.loopStart,s.loopEnd);
  if(s.sustainType) loop(1,s.sustainType,s.sustainStart,s.sustainEnd);
  f.flush(); if(!f) throw Error("Cannot finish sample bank"); f.close(); if(!f) throw Error("Cannot close sample bank");
}
}
