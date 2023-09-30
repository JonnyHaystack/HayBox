#ifndef _MODES_FGCMODE_HPP
#define _MODES_FGCMODE_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

class FgcMode : public ControllerMode {
  public:
    FgcMode(GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
