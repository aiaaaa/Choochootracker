#pragma once
#include "project_instruments.h"
#include <cstdio>
#include <string>
#include <vector>
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "experimental/hybrid/include/preset.hpp"
void initSQ80Patch(InstrumentSQ80&);
bool validSQ80(const InstrumentSQ80&);
int loadSQ80Data(FILE*,Instrument*);
void saveSQ80Data(FILE*,const Instrument*);
bool importSQ80(const std::vector<uint8_t>&,std::vector<hybrid::Preset>&,std::string&);
hybrid::Patch decodeSQ80(const InstrumentSQ80&);
// UI thread only; loaded banks remain immutable until process shutdown.
bool sq80LoadBanks(const std::string& root,std::string& error);
const hybrid::WaveBank* sq80Bank(unsigned bank);
#endif
