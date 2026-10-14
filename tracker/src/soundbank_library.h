#pragma once
#include "soundbank_index.h"
const std::vector<soundbank::BankEntry>& soundbankEntries();
void soundbankRescan(bool force = false);
void soundbankLoad(int instrument, const std::string& bank, const std::string& preset,
                   bool restoring = false);
void soundbankPoll();
bool soundbankBusy();
bool soundbankScanned();
bool soundbankScanning();
void soundbankCycle(int instrument, int direction);
const std::string& soundbankStatus();
std::string soundbankRoot();

void soundbankShutdown();
