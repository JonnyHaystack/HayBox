#ifndef _MODES_SHOVELKNIGHT_HPP
#define _MODES_SHOVELKNIGHT_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class ShovelKnight : public ControllerMode {
  public:
    ShovelKnight();

  private:
    virtual void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
