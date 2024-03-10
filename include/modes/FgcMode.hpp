#ifndef _MODES_FGCMODE_HPP
#define _MODES_FGCMODE_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

class FgcMode : public ControllerMode {
  public:
    FgcMode();

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
