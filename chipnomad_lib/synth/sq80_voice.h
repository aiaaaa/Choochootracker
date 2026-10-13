#pragma once
#include "../sq80_patch.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
class SQ80Voice {
public:
 void init(float rate);
 void configure(const InstrumentSQ80*,float cents,float gain);
 void noteOn();void noteOff();void kill();
 void render(float* stereo,size_t frames);
 bool active() const {return active_;}
 float envelopeLevel() const {return active_?level_:0;}
private:
 hybrid::Synth synth_;
 hybrid::Event events_[32]{};size_t eventCount_=0;
 InstrumentSQ80 patch_{};
 const hybrid::WaveBank* bank_=nullptr;
 float cents_=6000,gain_=1,level_=0;
 int key_=60;bool active_=false,gated_=false;
 void send(hybrid::EventKind,int key,float value);
};
#endif
