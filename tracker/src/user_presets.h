#pragma once
#include "project.h"
#include "preset_zip.h"
#include <string>
#include <vector>

const char* userPresetFolder(InstrumentType type);
bool userPresetCompatible(InstrumentType target, InstrumentType source);

// A single engine's folder browser. No catalog required, no ZIP extraction.
class UserPresets {
public:
  enum class Kind { folder, zip, bank, preset };
  struct Item { std::string name, path; Kind kind; int voice = -1; bool legacy = false; };
  void setup(const std::string& root, InstrumentType type, const std::string& legacyRoot = "");
  bool refresh(std::string& error);
  bool enter(size_t index, std::string& error);
  bool back(std::string& error);
  bool load(size_t index, Project* destination, int slot, std::string& error);
  const std::vector<Item>& items() const { return items_; }
  std::string label() const;
  bool atRoot() const { return stack_.size() <= 1; }
private:
  struct Location { std::string path, member; bool zip = false, bank = false, legacy = false; };
  const std::string& locationRoot() const { return stack_.back().legacy ? legacyRoot_ : root_; }
  bool bytes(const std::string& path, std::vector<uint8_t>& data, std::string& error);
  bool addPreset(const std::string& name, const std::string& path, std::string& error);
  std::string root_, legacyRoot_;
  InstrumentType type_ = InstrumentType::none;
  std::vector<Location> stack_;
  std::vector<Item> items_;
  PresetZip archive_;
  std::vector<InstrumentDX7> voices_;
};
