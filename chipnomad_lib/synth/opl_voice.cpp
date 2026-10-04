#include "opl_voice.h"
#include <cstring>
void OPLVoice::init(float rate){rate_=rate;opl2_.reset();opl3_.reset();configured_=false;resampler_.init(opl2_.sample_rate(3579545),rate_);kill();}
void OPLVoice::write(unsigned reg,unsigned value){
  if(type_==InstrumentType::OPL3){if(reg&0x100)opl3_.write_address_hi(reg);else opl3_.write_address(reg);opl3_.write_data(value);}
  else{opl2_.write_address(reg);opl2_.write_data(value);}
}
void OPLVoice::configure(InstrumentType type,const InstrumentOPL* patch,float cents,float gain){
  if(!patch)return;
  InstrumentOPL comparable=*patch;comparable.fineTune=patch_.fineTune;
  bool changed=!configured_||type_!=type||memcmp(&patch_,&comparable,sizeof(*patch));
  if(changed){bool gate=gated_,wasActive=active_;kill();type_=type;patch_=*patch;applyPatch();configured_=true;gated_=gate;active_=wasActive;}
  patch_.fineTune=patch->fineTune;
  cents_=std::isfinite(cents)?cents+patch_.fineTune:6000;gain_=std::clamp(gain,0.f,1.f);pitch();
}
void OPLVoice::applyPatch(){
  // Clear pairing, routes and key state before topology changes.
  opl2_.reset();opl3_.reset();
  write(1,0x20);
  if(type_==InstrumentType::OPL3){write(0x105,1);write(0x104,patch_.topology==OPLTopology::fourOperator?1:0);}
  write(0xbd,(patch_.deepTremolo?0x80:0)|(patch_.deepVibrato?0x40:0));
  // Channel 0 + channel 3 form the first native OPL3 four-operator pair.
  const int addresses[]={0,3,8,11};
  int count=patch_.topology==OPLTopology::twoOperator?2:4;
  for(int i=0;i<count;++i){auto& o=patch_.operators[i];int a=addresses[i];
    write(0x20+a,(o.tremolo<<7)|(o.vibrato<<6)|(o.sustained<<5)|(o.rateScale<<4)|o.multiplier);
    write(0x40+a,(o.keyScale<<6)|o.level);write(0x60+a,(o.attack<<4)|o.decay);
    write(0x80+a,(o.sustain<<4)|o.release);write(0xe0+a,o.waveform);
  }
  for(int i=0;i<2;++i)write(0xc0+(i?3:0),(patch_.feedback[i]<<1)|patch_.connection[i]|(type_==InstrumentType::OPL3?patch_.pan[i]<<4:0));
}
void OPLVoice::pitch(){
  for(int i=0;i<(patch_.topology==OPLTopology::twoOperator?1:2);++i){
    float base=(patch_.percussion||patch_.fixedNote)?patch_.drumKey*100.f:cents_;
    double cents=base+patch_.noteOffset[i]*100.0;
    // WOPL second-voice detune quantizes signed units to 1/32 semitone.
    if(i&&patch_.topology==OPLTopology::dualVoice)cents+=((int(patch_.secondDetune)+128)/2-64)*100.0/32;
    double fnum=440*std::exp2((std::clamp(cents,0.,14000.)-6900)/1200)*1048576.0/opl2_.sample_rate(3579545);
    int block=0;while(fnum>1023&&block<7){fnum*=.5;++block;}
    int f=std::clamp(int(std::lround(fnum)),1,1023),lo=f&255,hi=(f>>8)|(block<<2)|(gated_?0x20:0),ch=i?3:0;
    if(lo!=low_[i]){write(0xa0+ch,lo);low_[i]=lo;}if(hi!=high_[i]){write(0xb0+ch,hi);high_[i]=hi;}
  }
}
void OPLVoice::noteOn(){gated_=false;pitch();pendingKeyOn_=true;active_=true;silent_=0;}
void OPLVoice::noteOff(){pendingKeyOn_=false;gated_=false;pitch();}
void OPLVoice::kill(){pendingKeyOn_=false;write(0xb0,0);write(0xb3,0);active_=gated_=false;low_[0]=low_[1]=high_[0]=high_[1]=-1;silent_=0;level_=0;resampler_.reset();}
void OPLVoice::native(float& l,float& r){
  if(type_==InstrumentType::OPL3){ymfm::ymf262::output_data out;opl3_.generate(&out);l=(out.data[0]+out.data[2])/32768.f;r=(out.data[1]+out.data[3])/32768.f;}
  else{ymfm::ym3812::output_data out;opl2_.generate(&out);l=r=out.data[0]/32768.f;}
  if(pendingKeyOn_){pendingKeyOn_=false;gated_=true;pitch();}
  if(!gated_&&l==0&&r==0)++silent_;else silent_=0;
}
void OPLVoice::render(float* stereo,size_t frames){
  for(size_t i=0;i<frames;++i){float l=0,r=0;if(active_)resampler_.next([&](float& a,float& b){native(a,b);},l,r);
    // Native OPL full scale, conservative instrument gain; normal track mixer follows.
    stereo[2*i]=l*gain_*.25f;stereo[2*i+1]=r*gain_*.25f;
    level_=std::max(std::max(std::abs(l),std::abs(r)),level_*.999f);
    if(silent_>49715)kill();
  }
}
