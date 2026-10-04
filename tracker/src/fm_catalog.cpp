#include "fm_catalog.h"
#include "project_instruments.h"
#include "opl_patch.h"
#include "four_op_patch.h"
#include <cstdio>
#include <cstring>
bool loadFMCatalog(const char* path,std::vector<FMPresetEntry>& entries) {
  FILE* f=fopen(path,"rb");if(!f)return false;
  auto fail=[&]{fclose(f);return false;};
  char line[1024];if(!fgets(line,sizeof(line),f)||strcmp(line,"CCT-CHIP-CATALOG\t1\n"))return fail();
  std::vector<FMPresetEntry> parsed;
  while(fgets(line,sizeof(line),f)) {
    if(!strchr(line,'\n')||parsed.size()>=65536)return fail();
    std::vector<std::string> fields;char* start=line;
    for(char* c=line;;++c)if(*c=='\t'||*c=='\n'){bool end=*c=='\n';*c=0;fields.emplace_back(start);start=c+1;if(end)break;}
    if(fields.size()!=6||fields[2].empty()||fields[2].size()>63||fields[3].empty()||fields[3].size()>63||fields[4].empty()||fields[4].size()>63||fields[5].size()>200||fields[5].size()<5||fields[5].substr(fields[5].size()-4)!=".cni"||fields[5].find_first_of("/\\:")!=std::string::npos||fields[5].find("..")!=std::string::npos)return fail();
    int type=0,bank=0;char extra;
    if(sscanf(fields[0].c_str(),"%d%c",&type,&extra)!=1||(!isOPL((InstrumentType)type)&&!isFourOp((InstrumentType)type)&&type!=int(InstrumentType::DX7))||sscanf(fields[1].c_str(),"%d%c",&bank,&extra)!=1||bank<1||bank>65535)return fail();
    parsed.push_back({type,bank,fields[2],fields[3],fields[4],fields[5]});
  }
  bool ok=!ferror(f);fclose(f);if(ok)entries.swap(parsed);return ok;
}
