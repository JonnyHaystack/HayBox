#ifndef _MODES_ULTIMATE_TERRY_HPP
#define _MODES_ULTIMATE_TERRY_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate_Terry : public ControllerMode {
  public:
    Ultimate_Terry(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
