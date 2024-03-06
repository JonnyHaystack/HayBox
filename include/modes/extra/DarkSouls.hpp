#ifndef _MODES_DARKSOULS_HPP
#define _MODES_DARKSOULS_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class DarkSouls : public ControllerMode {
  public:
    DarkSouls(GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
