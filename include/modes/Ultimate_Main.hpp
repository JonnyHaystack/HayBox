#ifndef _MODES_ULTIMATE_MAIN_HPP
#define _MODES_ULTIMATE_MAIN_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate_Main : public ControllerMode {
  public:
    Ultimate_Main(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
