#pragma once
#include <string>
#include <vector>
struct FMPresetEntry {
  int type,bank;
  std::string bankName,category,name,path;
  int imported=-1;
};
// UI/offline only. Transactional, bounded metadata read; no synthesis allocation.
bool loadFMCatalog(const char* path,std::vector<FMPresetEntry>& entries);
