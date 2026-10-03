#include "touch_modulation.h"
#include "chipnomad_lib.h"
#include "chipnomad_lib_live_stick.h"
#include <atomic>
#include <algorithm>
#include <cmath>

static_assert(std::atomic<uint32_t>::is_always_lock_free, "Touch snapshots must be lock-free");
static std::atomic<uint32_t> values[2];
static std::atomic<uint32_t> generation;
void touchModReset() {
  ++generation;
  for(auto& value:values) value.store(0);
}
void TouchModInput::refresh() {
  const auto current=generation.load();
  if(current!=generation_) {
    generation_=current;
    for(auto& p:panels_) p.active=false;
  }
}
bool TouchModInput::event(unsigned panel,int64_t id,Event event,float x,float y,bool armed) {
  refresh();
  if(panel>=2) return false;
  auto& p=panels_[panel];
  unsigned i=0;
  while(i<p.count && p.ids[i]!=id) ++i;
  if(event==Event::down) {
    if(i<p.count || p.count==16) return false;
    if(!p.count && armed) { p.primary=id; p.active=true; }
    p.ids[p.count++]=id;
  }
  bool owned=p.active && p.primary==id;
  if(event==Event::up) {
    if(i<p.count) p.ids[i]=p.ids[--p.count];
    if(owned) { p.active=false; values[panel].store(0); }
  } else if(owned) {
    if(!std::isfinite(x) || !std::isfinite(y)) {
      p.active=false; values[panel].store(0); return true;
    }
    x=std::clamp(x,0.0f,1.0f); y=std::clamp(y,0.0f,1.0f);
    const uint32_t xx=std::lround(x*32766), yy=std::lround((1-y)*32766);
    values[panel].store(0x80000000u | xx | (yy<<15));
  }
  return owned;
}
int16_t touchModOutput(unsigned panel,unsigned parameter,int amount) {
  if(panel>=2 || parameter>2 || !chipnomadLiveStickIsEnabled()) return 0;
  const uint32_t packed=values[panel].load();
  if(!(packed&0x80000000u)) return 0;
  int value=16383;
  if(parameter<2) value=int((packed>>(parameter*15))&32767)-16383;
  return std::clamp(value*std::clamp(amount,-128,127)*255/16383,-32385,32385);
}
