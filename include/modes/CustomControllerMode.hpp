#ifndef _MODES_CUSTOMCONTROLLERMODE_HPP
#define _MODES_CUSTOMCONTROLLERMODE_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

#include <config.pb.h>

class CustomControllerMode : public ControllerMode {
  public:
    CustomControllerMode();
    void SetConfig(GameModeConfig &config, const CustomModeConfig &custom_mode_config);

  protected:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);

  private:
    const CustomModeConfig *_custom_mode_config;
    uint64_t _modifier_button_masks[10];
    uint64_t _button_combo_mappings_masks[5];
    uint64_t _buttons_to_ignore = 0;
    uint64_t _filtered_buttons = 0;

    Button GetDirectionButton(const Button *direction_buttons, StickDirectionButton direction);
};

#endif
