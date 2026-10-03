#pragma once
#include "common.h"
#include <SDL2/SDL.h>

inline void vitaDefaultControls(KeyMapping& keys) {
  // Physical NESW matches the user's R36H: Play, Edit, Opt, Select.
  keys.keyPlay[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_Y};
  keys.keyEdit[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_B};
  keys.keyOpt[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_A};
  keys.keyShift[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_X};
  keys.keyMotionLive[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER};
  keys.keyMotionRecord[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER};
  keys.keyMotionErase[1] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_BACK};
  keys.keyPlay[2] = {InputDeviceType::gamepad, SDL_CONTROLLER_BUTTON_START};
}

inline bool vitaMigrateDefaultControls(KeyMapping& keys) {
  InputCode* rows[] = {keys.keyUp, keys.keyDown, keys.keyLeft, keys.keyRight,
    keys.keyEdit, keys.keyOpt, keys.keyPlay, keys.keyShift, keys.keyMotionLive,
    keys.keyMotionRecord, keys.keyMotionErase};
  const int previous[] = {SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_START,
    SDL_CONTROLLER_BUTTON_BACK, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y};
  for (unsigned i = 0; i < sizeof(rows) / sizeof(rows[0]); ++i) {
    // Preserve any customized gamepad map; keyboard mappings are independent.
    if (rows[i][1].deviceType != InputDeviceType::gamepad || rows[i][1].code != previous[i] ||
        rows[i][2].deviceType != InputDeviceType::none) return false;
  }
  vitaDefaultControls(keys);
  return true;
}
