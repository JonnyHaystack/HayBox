#ifndef _MODES_ULTIMATE_HPP
#define _MODES_ULTIMATE_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

class Ultimate : public ControllerMode {
  public:
    Ultimate(const GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
