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

    Button GetDirectionButton(const Button *direction_buttons, StickDirectionButton direction);
};

#endif
