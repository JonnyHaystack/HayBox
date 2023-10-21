#ifndef _MODES_CUSTOMKEYBOARDMODE_HPP
#define _MODES_CUSTOMKEYBOARDMODE_HPP

#include "core/KeyboardMode.hpp"
#include "core/state.hpp"

#include <config.pb.h>

class CustomKeyboardMode : public KeyboardMode {
  public:
    CustomKeyboardMode(const GameModeConfig &config, const KeyboardModeConfig &keyboard_config);

  private:
    const KeyboardModeConfig &_keyboard_config;

    void UpdateKeys(const InputState &inputs);
};

#endif