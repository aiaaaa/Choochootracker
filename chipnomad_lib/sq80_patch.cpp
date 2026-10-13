#include "sq80_patch.h"
#if CHOOCHOO_EXPERIMENTAL_SQ80ESQ
#include "project_io_common.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <memory>
#include <stdexcept>
namespace {
std::unique_ptr<hybrid::WaveBank> owned[2];
std::atomic<const hybrid::WaveBank*> banks[2]{};
}
const hybrid::WaveBank* sq80Bank(unsigned bank){return bank<2?banks[bank].load():nullptr;}
bool sq80LoadBanks(const std::string& root,std::string& error){
 error.clear();
 for(unsigned n=0;n<2;++n){
  if(sq80Bank(n))continue;
  auto b=std::make_unique<hybrid::WaveBank>();std::string why;
  if(!b->load(root+(n?"/hardware-1.hwb":"/hardware-80.hwb"),why)){if(!error.empty())error+="; ";error+=why;continue;}
  if(b->romSize()!=(n?65536u:262144u)){error="Bank model does not match filename";continue;}
  owned[n]=std::move(b);banks[n].store(owned[n].get());
 }
 return sq80Bank(0)||sq80Bank(1);
}
void initSQ80Patch(InstrumentSQ80& p){
 p={};p.schema=1;p.velocity=100;auto* b=p.raw;memcpy(b,"INIT  ",6);
 for(int e=0;e<4;++e){int n=6+10*e;b[n]=126;b[n+1]=90;b[n+2]=80;b[n+4]=24;b[n+5]=16;b[n+6]=24;}
 for(int l=0;l<3;++l){int n=46+4*l;b[n]=24;b[n+1]=192;b[n+2]=192;b[n+3]=128;}
 for(int o=0;o<3;++o){int n=58+10*o;b[n]=36;b[n+2]=255;b[n+6]=100|(o==0?128:0);b[n+7]=255;}
 b[88]=100;b[89]=100;b[91]=255;b[94]=128;b[95]=128;b[100]=143;
}
bool validSQ80(const InstrumentSQ80& p){return p.schema==1&&p.bank<2&&p.profile<2&&p.velocity>0&&p.velocity<=127;}
hybrid::Patch decodeSQ80(const InstrumentSQ80& p){std::array<uint8_t,102>b;std::copy_n(p.raw,102,b.begin());return hybrid::decodePreset(b,p.profile?hybrid::PresetProfile::hardware:hybrid::PresetProfile::legacyPlugin).patch;}
int loadSQ80Data(FILE* f,Instrument* i){
 InstrumentSQ80 p{};unsigned seen=0;
 while(char* line=peekLine(f)){
  if(*line=='#')break;
  if(!strncmp(line,"- SQ80 identity: ",17)){
   unsigned s,b,m,v;char tail;
   if((seen&1)||sscanf(line+17,"%u,%u,%u,%u %c",&s,&b,&m,&v,&tail)!=4||s!=1||b>1||m>1||!v||v>127)return 1;
   p.schema=s;p.bank=b;p.profile=m;p.velocity=v;seen|=1;
  }else if(!strncmp(line,"- SQ80 program: ",16)){
   const char* hex=line+16;if((seen&2)||strlen(hex)!=204)return 1;
   auto digit=[](char c){return c>='0'&&c<='9'?c-'0':c>='A'&&c<='F'?c-'A'+10:c>='a'&&c<='f'?c-'a'+10:-1;};
   for(int n=0;n<102;++n){int a=digit(hex[2*n]),b=digit(hex[2*n+1]);if(a<0||b<0)return 1;p.raw[n]=(a<<4)|b;}seen|=2;
  }
  consumeLine(f);
 }
 if(seen!=3||!validSQ80(p))return 1;i->chip.sq80=p;return 0;
}
void saveSQ80Data(FILE* f,const Instrument* i){const auto&p=i->chip.sq80;
 fprintf(f,"- SQ80 identity: %u,%u,%u,%u\n- SQ80 program: ",p.schema,p.bank,p.profile,p.velocity);
 for(auto b:p.raw)fprintf(f,"%02X",b);fputc('\n',f);
}
bool importSQ80(const std::vector<uint8_t>& bytes,std::vector<hybrid::Preset>& out,std::string& error){
 try{
  std::vector<hybrid::Preset> parsed;
  if(bytes.empty()||bytes.size()>1024*1024)throw std::runtime_error("Invalid SQ80 bank size");
  if(bytes.size()==102){std::array<uint8_t,102>b;std::copy(bytes.begin(),bytes.end(),b.begin());parsed.push_back(hybrid::decodePreset(b));}
  else for(size_t pos=0;pos<bytes.size();){
   if(bytes.size()-pos<6||bytes[pos]!=240||bytes[pos+1]!=15||bytes[pos+2]!=2||bytes[pos+3]>15)throw std::runtime_error("Not SQ80/ESQ program SysEx");
   unsigned count=bytes[pos+4]==1?1:bytes[pos+4]==2?40:0;size_t length=6+count*204;
   if(!count||length>bytes.size()-pos||bytes[pos+length-1]!=247)throw std::runtime_error("Truncated or unsupported SQ80 bank");
   size_t at=pos+5;
   for(unsigned n=0;n<count;++n){std::array<uint8_t,102>b;
    for(auto& v:b){unsigned lo=bytes[at++],hi=bytes[at++];if(lo>15||hi>15)throw std::runtime_error("Invalid SQ80 SysEx nibble");v=lo|(hi<<4);}
    parsed.push_back(hybrid::decodePreset(b));
   }pos+=length;
  }
  out=std::move(parsed);error.clear();return true;
 }catch(const std::exception&e){error=e.what();return false;}
}
#endif
