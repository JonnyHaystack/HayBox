#ifndef _MODES_RIVALS_2_HPP
#define _MODES_RIVALS_2_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

#include <config.pb.h> //this was not here, but doesn't seem to matter if I remove it from Melee20Button.hpp anyway

class Rivals2 : public ControllerMode {
  public:
    Rivals2();

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
