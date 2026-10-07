#pragma once
#include <cstdint>
// Opens the current native instrument's USER library; remembers each engine's location.
void openUserPresetBrowser();
class UserPresets;
enum class InstrumentType : uint8_t;
bool setupUserPresetLibrary(UserPresets& library, InstrumentType type);
