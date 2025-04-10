#ifndef _MODES_RIVALS_2_HPP
#define _MODES_RIVALS_2_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

class Rivals2 : public ControllerMode {
  public:
    Rivals2();

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
