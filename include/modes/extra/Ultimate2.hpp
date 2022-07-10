#ifndef _MODES_ULTIMATE_HPP
#define _MODES_ULTIMATE_HPP

#include "modes/PlatformFighter.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate2 : public PlatformFighter {
  public:
    Ultimate2(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs);
    void UpdateAnalogOutputs(InputState &inputs);
};

#endif
