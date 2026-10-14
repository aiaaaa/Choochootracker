#pragma once
#include <algorithm>
#include <map>
#include <set>
#include "soundbank_index.h"
namespace soundbank {
inline std::string browserLabel(std::string name) {
  auto dot=name.rfind('.');
  if (dot!=std::string::npos) {
    const auto ext=name.substr(dot);
    if (ext==".zip" || ext==".sf2" || ext==".sf3" || ext==".sfz" ||
        ext==".exs" || ext==".wav" || ext==".aif" || ext==".aiff") name.resize(dot);
  }
  auto dash=name.rfind('-');
  if (dash!=std::string::npos && name.size()-dash-1==40 &&
      name.find_first_not_of("0123456789abcdefABCDEF",dash+1)==std::string::npos)
    name.resize(dash);
  std::replace(name.begin(),name.end(),'_',' ');
  return name;
}
inline bool rawSample(const BankEntry& bank) {
  return bank.format == "wav" || bank.format == "aif" || bank.format == "aiff";
}
inline std::vector<std::string> bankFolders(std::string id) {
  auto archive=id.find("::");
  if (archive!=std::string::npos) id.replace(archive,2,"/");
  std::vector<std::string> parts;
  size_t start=0, slash;
  while ((slash=id.find('/',start))!=std::string::npos) {
    parts.push_back(id.substr(start,slash-start)); start=slash+1;
  }
  parts.push_back(id.substr(start)); return parts;
}
inline std::set<std::string> mappedCollections(const std::vector<BankEntry>& banks) {
  std::set<std::string> result;
  for (const auto& bank:banks) if (!rawSample(bank)) result.insert(bankFolders(bank.id).front());
  return result;
}
inline bool hasHiddenSamples(const std::vector<BankEntry>& banks) {
  auto mapped=mappedCollections(banks);
  for (const auto& bank:banks) if (rawSample(bank) && mapped.count(bankFolders(bank.id).front())) return true;
  return false;
}
struct BrowserEntry { std::string name; int bank; }; // -1 means a folder.
inline std::vector<BrowserEntry> folderEntries(const std::vector<BankEntry>& banks,
    const std::vector<std::string>& folder, bool raw=false) {
  std::map<std::string,int> children;
  auto mapped=mappedCollections(banks);
  for (size_t i=0;i<banks.size();++i) {
    const auto& b=banks[i];
    if (raw ? !rawSample(b) : rawSample(b) && mapped.count(bankFolders(b.id).front())) continue;
    auto parts=bankFolders(b.id);
    if (parts.size()<=folder.size() || !std::equal(folder.begin(),folder.end(),parts.begin())) continue;
    children[parts[folder.size()]]=parts.size()==folder.size()+1 ? int(i) : -1;
  }
  std::vector<BrowserEntry> result;
  for (const auto& child:children) result.push_back({child.first,child.second});
  return result;
}
inline std::pair<int,int> cyclePreset(const std::vector<BankEntry>& banks,
    const std::string& bank, const std::string& preset, int direction) {
  std::vector<std::pair<int,int>> presets; int current=-1;
  auto mapped=mappedCollections(banks);
  for (size_t b=0;b<banks.size();++b) {
    if (!banks[b].error.empty() || (rawSample(banks[b]) && mapped.count(bankFolders(banks[b].id).front()))) continue;
    for (size_t p=0;p<banks[b].presets.size();++p) {
      if (banks[b].id==bank && banks[b].presets[p].first==preset) current=int(presets.size());
      presets.emplace_back(b,p);
    }
  }
  if (presets.empty()) return {-1,-1};
  int n=int(presets.size());
  return presets[current<0 ? (direction>0 ? 0 : n-1) : (current+(direction>0 ? 1 : n-1))%n];
}
} // namespace soundbank
