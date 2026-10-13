#pragma once
#include "sq80_patch.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
struct SQ80Parameter {const char* group;const char* label;uint8_t byte,mask,shift;int minimum,maximum;bool bipolar;const char* hint;};
const std::vector<SQ80Parameter>& sq80Parameters();
int sq80Get(const InstrumentSQ80&,const SQ80Parameter&);
void sq80Set(InstrumentSQ80&,const SQ80Parameter&,int);
#endif
