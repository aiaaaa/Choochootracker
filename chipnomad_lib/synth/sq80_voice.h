#pragma once
#include "../sq80_patch.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
struct SQ80Performance {
 int wheel=0,pedal=0,sustain=0,aftertouch=0,external=0,notePressure=-1;
 int cutoff=-1,resonance=-1,lfo1Rate=-1;
};
class SQ80Voice {
public:
 void init(float rate);
 void configure(const InstrumentSQ80*,float cents,float gain);
 void performance(const SQ80Performance&, bool triggering=false);
 void setGain(float gain);
 void noteOn();void noteOff();void kill();
 void render(float* stereo,size_t frames);
 bool active() const {return active_;}
 float envelopeLevel() const {return active_?level_:0;}
private:
 hybrid::Synth synth_;
 int controllers_[5]={-1,-1,-1,-1,-1};
 int nextPressure_=-1,lastPressure_=-1;
 hybrid::Event events_[32]{};size_t eventCount_=0;
 InstrumentSQ80 patch_{};
 const hybrid::WaveBank* bank_=nullptr;
 float cents_=6000,gain_=1,level_=0;
 int key_=60;bool active_=false,gated_=false;
 void send(hybrid::EventKind,int key,float value);
};
#endif
