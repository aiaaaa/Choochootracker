#pragma once
#include "module.h"
#include "project.h"
namespace modLucky {
#ifdef CHOOCHOO_MOD_LUCKY_TEST
void testBankFailure(int afterSamples,bool allocation);
#endif
std::vector<int> availableSlots(const Project& project);
struct Bank {
  std::shared_ptr<const Candidate> candidate;
  std::vector<int> slots;
  std::vector<Instrument> instruments;
  std::string directory;
  bool committed=false;
  ~Bank();
};
std::unique_ptr<Bank> stageBank(std::shared_ptr<const Candidate> candidate,const std::vector<int>& slots,
                              const std::string& sampleRoot,const std::atomic<bool>& cancel);
// Caller pauses tracker audio. No allocation, file IO, or fallible operation after checks.
bool commitBank(Bank& bank,Project& project,const std::vector<uint8_t>& snapshot);
std::vector<uint8_t> snapshotProject(const Project& project);
}
