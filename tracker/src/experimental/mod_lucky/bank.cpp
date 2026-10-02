#include "bank.h"
#include "synth/sample_voice.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unistd.h>

namespace modLucky {
#ifdef CHOOCHOO_MOD_LUCKY_TEST
static std::atomic<int> failAfter{-1};
static bool failAllocation=false;
void testBankFailure(int afterSamples,bool allocation) { failAllocation=allocation; failAfter=afterSamples; }
#endif
std::vector<int> availableSlots(const Project& p) {
  bool referenced[PROJECT_MAX_INSTRUMENTS]{};
  for(const auto& phrase:p.phrases) for(const auto& row:phrase.rows)
    if(row.instrument<PROJECT_MAX_INSTRUMENTS) referenced[row.instrument]=true;
  Instrument empty{}; getInstrumentFunctions(InstrumentType::none).init(&empty);
  Table emptyTable{}; tableClear(&emptyTable);
  std::vector<int> slots;
  for(int i=0;i<PROJECT_MAX_INSTRUMENTS;++i) {
    // Stronger than instrumentIsEmpty: preserve residual configuration too.
    if(referenced[i] || std::memcmp(&empty,&p.instruments[i],sizeof(empty))) continue;
    if(std::memcmp(&emptyTable,&p.tables[i],sizeof(emptyTable))) continue;
    slots.push_back(i);
  }
  return slots;
}
std::vector<uint8_t> snapshotProject(const Project& p) {
  const auto* bytes=reinterpret_cast<const uint8_t*>(&p);
  return {bytes,bytes+sizeof(p)};
}
Bank::~Bank() {
  for(auto& instrument:instruments) getInstrumentFunctions(instrument.type).free(&instrument);
  if(!committed && !directory.empty()) { std::error_code ec; std::filesystem::remove_all(directory,ec); }
}
namespace {
std::string json(const std::string& text) {
  std::ostringstream out; out<<'"';
  for(unsigned char c:text) {
    if(c=='"' || c=='\\') out<<'\\'<<c;
    else if(c<32 || c>=127) out<<"\\u00"<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(c);
    else out<<c;
  }
  out<<'"'; return out.str();
}
void checkedWrite(const std::filesystem::path& path,const std::string& text) {
  std::ofstream out(path,std::ios::binary); out<<text; out.flush(); if(!out) throw Error("Cannot write source record"); out.close(); if(!out) throw Error("Cannot close source record");
}
}
std::unique_ptr<Bank> stageBank(std::shared_ptr<const Candidate> candidate,const std::vector<int>& slots,
                              const std::string& sampleRoot,const std::atomic<bool>& cancel) {
  if(!candidate || slots.size()!=candidate->samples.size() || slots.empty()) throw Error("Invalid bank allocation");
  if(!std::all_of(candidate->id.begin(),candidate->id.end(),[](char c){return c>='0' && c<='9';}) || candidate->id.empty()) throw Error("Invalid module ID");
  auto bank=std::make_unique<Bank>(); bank->candidate=candidate; bank->slots=slots;
  namespace fs=std::filesystem;
  // The user-selected sample root may legitimately be a symlink (e.g. /var on macOS).
  // Resolve that trusted root, then reject symlinks in our generated bank namespace.
  fs::path root=fs::weakly_canonical(fs::absolute(sampleRoot))/"mod_lucky";
  if(fs::is_symlink(fs::symlink_status(root))) throw Error("Unsafe sample destination");
  fs::create_directories(root);
  std::string base=candidate->id+"-"+candidate->hash;
  for(unsigned i=0;i<1000;++i) {
    fs::path dest=root/(base+"-"+std::to_string(i));
    if((dest/"s255.wav").string().size()>PROJECT_SAMPLE_PATH_LENGTH) throw Error("Sample path too long");
    if(fs::create_directory(dest)) { bank->directory=dest.string(); break; }
  }
  if(bank->directory.empty()) throw Error("Bank destination exists");
  bank->instruments.reserve(slots.size());
  std::ostringstream metadata;
  metadata<<"{\n  \"module_id\": "<<json(candidate->id)<<",\n  \"source\": "<<json("https://modarchive.org/index.php?request=view_by_moduleid&query="+candidate->id)
    <<",\n  \"title\": "<<json(candidate->title)<<",\n  \"format\": "<<json(candidate->format)
    <<",\n  \"original_filename\": "<<json(candidate->originalFilename)<<",\n  \"downloaded_at\": "<<json(candidate->downloadedAt)
    <<",\n  \"fnv1a64\": "<<json(candidate->hash)<<",\n  \"credits_from_module\": "<<json(candidate->credits)
    <<",\n  \"licensing\": \"No permissions inferred. Original supplied page retained in source-page.html.\",\n"
    <<"  \"tuning\": \"Reference frequency encoded once as rounded integer WAV sample rate; instrument pitch 0, C4 root. No keymaps/envelopes/effects imported.\",\n"
    <<"  \"loop_fallbacks\": "<<candidate->loopFallbacks<<",\n  \"samples\": [\n";
  for(size_t i=0;i<slots.size();++i) {
    if(cancel) throw Error("Cancelled");
#ifdef CHOOCHOO_MOD_LUCKY_TEST
    if(int(i)==failAfter.load()) {
      failAfter=-1;
      if(failAllocation) throw std::bad_alloc();
      throw Error("Cannot write sample bank (injected IO failure)");
    }
#endif
    const auto& sample=candidate->samples[i];
    char filename[16]; snprintf(filename,sizeof(filename),"s%03u.wav",sample.index);
    fs::path path=fs::path(bank->directory)/filename;
    writeWav(path.string(),sample);
    bank->instruments.emplace_back(); auto& instrument=bank->instruments.back();
    getInstrumentFunctions(InstrumentType::Sample).init(&instrument);
    char error[96];
    if(sampleLoadWav16(path.c_str(),&instrument.chip.sample,error,sizeof(error))) throw Error(error);
    // Durable absolute path also works after saving a project into another folder.
    std::strcpy(instrument.chip.sample.path,path.c_str());
    std::string name=sample.name.empty()?"Sample "+std::to_string(sample.index+1):sample.name;
    for(char& c:name) if(static_cast<unsigned char>(c)<32 || static_cast<unsigned char>(c)>=127) c=' ';
    std::strncpy(instrument.name,name.c_str(),PROJECT_INSTRUMENT_NAME_LENGTH);
    instrument.name[PROJECT_INSTRUMENT_NAME_LENGTH]=0;
    instrument.chip.sample.loopMode=sample.playableLoop;
    instrument.chip.sample.filterEnabled=0; instrument.chip.sample.filterCharacter=0;
    instrument.chip.sample.attack=0; instrument.chip.sample.decay=0;
    instrument.chip.sample.sustain=255; instrument.chip.sample.release=8;
    if(i) metadata<<",\n";
    metadata<<"    {\"source_slot\": "<<sample.index<<", \"instrument\": "<<slots[i]<<", \"name\": "<<json(sample.name)
      <<", \"file\": "<<json(filename)<<", \"frames\": "<<sample.frames<<", \"channels\": "<<sample.channels<<", \"original_bits\": "<<sample.bits
      <<", \"reference_rate\": "<<std::setprecision(12)<<sample.referenceRate<<", \"transpose\": "<<sample.transpose<<", \"finetune_128\": "<<sample.finetune
      <<", \"loop_start\": "<<sample.loopStart<<", \"loop_end_exclusive\": "<<sample.loopEnd<<", \"loop_type\": "<<sample.loopType
      <<", \"sustain_start\": "<<sample.sustainStart<<", \"sustain_end_exclusive\": "<<sample.sustainEnd<<", \"sustain_type\": "<<sample.sustainType
      <<", \"playable_loop\": "<<unsigned(sample.playableLoop)<<", \"limitation\": "<<json(sample.limitation)<<"}";
  }
  metadata<<"\n  ]\n}\n";
  checkedWrite(fs::path(bank->directory)/"source.json",metadata.str());
  checkedWrite(fs::path(bank->directory)/"source-page.html",candidate->sourcePage);
  if(cancel) throw Error("Cancelled");
  return bank;
}
bool commitBank(Bank& bank,Project& project,const std::vector<uint8_t>& snapshot) {
  if(bank.committed || snapshot.size()!=sizeof(project) || std::memcmp(snapshot.data(),&project,sizeof(project))) return false;
  if(bank.slots.size()!=bank.instruments.size()) return false;
  // Snapshot includes all reference/table/configuration checks made at preflight.
  for(size_t i=0;i<bank.slots.size();++i) {
    project.instruments[bank.slots[i]]=bank.instruments[i];
    bank.instruments[i].chip.sample.data=nullptr; // ownership transferred to Project
  }
  bank.committed=true; return true;
}
}
