#ifndef _MODES_ULTPIKAV2_HPP
#define _MODES_ULTPIKAV2_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class UltPikaV2 : public ControllerMode {
  public:
    UltPikaV2(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
