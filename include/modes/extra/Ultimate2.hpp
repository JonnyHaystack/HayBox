#ifndef _MODES_ULTIMATE2_HPP
#define _MODES_ULTIMATE2_HPP

#include "modes/PlatformFighter.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate2 : public PlatformFighter {
  public:
    Ultimate2(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
