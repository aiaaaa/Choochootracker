#include "sq80_voice.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include <algorithm>
#include <cmath>
#include <cstring>
void SQ80Voice::send(hybrid::EventKind kind,int key,float value){hybrid::Event e{0,kind,0,uint8_t(key),value};if(eventCount_<32)events_[eventCount_++]=e;}
void SQ80Voice::init(float rate){synth_.setSampleRate(rate);kill();}
void SQ80Voice::configure(const InstrumentSQ80* p,float cents,float gain){
 if(!p||!validSQ80(*p))return;
 const auto* bank=sq80Bank(p->bank);
 if(bank!=bank_){kill();bank_=bank;synth_.setWaveBank(bank_);}
 if(memcmp(&patch_,p,sizeof(*p))){patch_=*p;synth_.setPatch(decodeSQ80(*p));}
 cents_=std::isfinite(cents)?cents:6000;gain_=std::clamp(gain,0.f,1.f);
 if(active_)send(hybrid::EventKind::pitchBend,key_,cents_/100-key_);
}
void SQ80Voice::performance(const SQ80Performance& p,bool triggering){
 const int values[]={p.wheel,p.pedal,p.sustain,p.aftertouch,p.external};
 const hybrid::EventKind kinds[]={hybrid::EventKind::wheel,hybrid::EventKind::pedal,hybrid::EventKind::sustain,hybrid::EventKind::channelPressure,hybrid::EventKind::external};
 for(int n=0;n<5;++n){int v=std::clamp(values[n],0,n==2?1:127);if(v!=controllers_[n]){send(kinds[n],0,n==2?float(v):v/127.f);controllers_[n]=v;}}
 synth_.setPerformance(p.cutoff,p.resonance,p.lfo1Rate);nextPressure_=p.notePressure;
 if(!triggering&&p.notePressure>=0&&p.notePressure!=lastPressure_){send(hybrid::EventKind::latestPressure,0,std::clamp(p.notePressure,0,127)/127.f);lastPressure_=p.notePressure;}
}
void SQ80Voice::setGain(float gain){gain_=std::clamp(gain,0.f,1.f);}
void SQ80Voice::noteOn(){
 if(!bank_)return;
 if(controllers_[0]<0){SQ80Performance p;p.wheel=patch_.wheel;p.pedal=patch_.pedal;performance(p,true);}
 const int previous=key_;const bool wasGated=gated_;
 key_=std::clamp(int(std::round(cents_/100)),0,127);
 const bool legato=wasGated&&(patch_.raw[93]&128)&&previous!=key_;
 if(wasGated&&!legato)send(hybrid::EventKind::noteOff,previous,0);
 send(hybrid::EventKind::pitchBend,key_,cents_/100-key_);
 send(hybrid::EventKind::noteOn,key_,patch_.velocity/127.f);
 lastPressure_=std::max(0,nextPressure_);send(hybrid::EventKind::latestPressure,0,lastPressure_/127.f);
 if(legato)send(hybrid::EventKind::noteOff,previous,0);
 active_=gated_=true;
}
void SQ80Voice::noteOff(){if(gated_)send(hybrid::EventKind::noteOff,key_,0);gated_=false;}
void SQ80Voice::kill(){if(active_||eventCount_)synth_.reset();eventCount_=0;active_=gated_=false;level_=0;std::fill_n(controllers_,5,-1);nextPressure_=lastPressure_=-1;}
void SQ80Voice::render(float* out,size_t frames){
 float l[128],r[128];level_=0;
 for(size_t at=0;at<frames;){size_t n=std::min<size_t>(128,frames-at);if(!synth_.render(l,r,n,events_,eventCount_)){std::fill_n(l,n,0.f);std::fill_n(r,n,0.f);}eventCount_=0;
  for(size_t k=0;k<n;++k){out[2*(at+k)]=l[k]*gain_*.25f;out[2*(at+k)+1]=r[k]*gain_*.25f;level_=std::max(level_,std::max(std::abs(l[k]),std::abs(r[k]))*gain_);}at+=n;
 }active_=synth_.activeVoices()!=0;
}
#endif
