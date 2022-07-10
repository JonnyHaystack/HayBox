#ifndef _MODES_ULTIMATE_HPP
#define _MODES_ULTIMATE_HPP

#include "modes/PlatformFighter.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate : public PlatformFighter {
  public:
    Ultimate(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
