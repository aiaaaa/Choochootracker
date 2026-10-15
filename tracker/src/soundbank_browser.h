#pragma once
#include <algorithm>
#include <map>
#include <cctype>
#include <tuple>
#include <set>
#include "soundbank_index.h"
namespace soundbank {
inline std::string browserLabel(std::string name) {
  auto dot=name.rfind('.');
  if (dot!=std::string::npos) {
    auto ext=name.substr(dot);
    for(auto& c:ext)c=char(std::tolower(static_cast<unsigned char>(c)));
    if (ext==".zip" || ext==".sf2" || ext==".sf3" || ext==".sfz" ||
        ext==".exs" || ext==".wav" || ext==".aif" || ext==".aiff") name.resize(dot);
  }
  auto dash=name.rfind('-');
  if (dash!=std::string::npos && name.size()-dash-1==40 &&
      name.find_first_not_of("0123456789abcdefABCDEF",dash+1)==std::string::npos)
    name.resize(dash);
  // Freesound transport names: numeric ID__uploader__descriptive-name.
  auto first=name.find("__"), second=first==std::string::npos?first:name.find("__",first+2);
  if(first>0 && first!=std::string::npos && second!=std::string::npos &&
     name.find_first_not_of("0123456789",0)==first) name.erase(0,second+2);
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
struct PresetReference { int bank, preset; std::string label; };
struct Collection { std::string name; std::vector<PresetReference> presets; };
inline std::string sortLabel(std::string value) {
  for(auto& c:value)c=char(std::tolower(static_cast<unsigned char>(c)));
  return value;
}
inline std::vector<Collection> collections(const std::vector<BankEntry>& banks) {
  std::map<std::string,std::set<std::string>> formats;
  const auto mapped=mappedCollections(banks);
  for(const auto& b:banks) if(!rawSample(b)) formats[bankFolders(b.id).front()].insert(b.format);
  std::map<std::string,Collection> groups;
  for(size_t n=0;n<banks.size();++n) {
    const auto& b=banks[n]; if(!b.error.empty())continue;
    auto parts=bankFolders(b.id); auto top=parts.front();
    if(rawSample(b) && mapped.count(top)) continue; // Supporting recordings stay with their mapped preset.
    auto name=parts.size()>1 || !rawSample(b)?browserLabel(top):std::string("Loose samples");
    if(formats[top].size()>1 && !rawSample(b))name+=" ("+b.format+")";
    auto& group=groups[sortLabel(name)];group.name=name;
    for(size_t p=0;p<b.presets.size();++p)
      group.presets.push_back({int(n),int(p),browserLabel(b.presets[p].second)});
  }
  std::vector<Collection> result;
  for(auto& entry:groups) {
    auto& g=entry.second;
    std::sort(g.presets.begin(),g.presets.end(),[&](const auto& a,const auto& b){
      return std::make_tuple(sortLabel(a.label),banks[a.bank].id,banks[a.bank].presets[a.preset].first)<
             std::make_tuple(sortLabel(b.label),banks[b.bank].id,banks[b.bank].presets[b.preset].first);
    });
    if(!g.presets.empty())result.push_back(std::move(g));
  }
  return result;
}
inline std::pair<int,int> cyclePreset(const std::vector<BankEntry>& banks,
    const std::string& bank, const std::string& preset, int direction) {
  std::vector<std::pair<int,int>> sequence; int current=-1;
  for(const auto& group:collections(banks)) for(const auto& ref:group.presets) {
    if(banks[ref.bank].id==bank && banks[ref.bank].presets[ref.preset].first==preset)
      current=int(sequence.size());
    sequence.emplace_back(ref.bank,ref.preset);
  }
  if(sequence.empty())return {-1,-1};
  int n=int(sequence.size());
  return sequence[current<0?(direction>0?0:n-1):(current+(direction>0?1:n-1))%n];
}
} // namespace soundbank
