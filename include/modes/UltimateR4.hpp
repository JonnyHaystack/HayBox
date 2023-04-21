#ifndef _MODES_ULTIMATE_R4_HPP
#define _MODES_ULTIMATE_R4_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class UltimateR4 : public ControllerMode {
  public:
    UltimateR4(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif

