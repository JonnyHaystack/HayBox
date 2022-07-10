#ifndef _MODES_MELEE20BUTTON_HPP
#define _MODES_MELEE20BUTTON_HPP

#include "modes/PlatformFighter.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Melee20Button : public PlatformFighter {
  public:
    Melee20Button(socd::SocdType socd_type);

  private:
    virtual void UpdateDigitalOutputs();
    virtual void UpdateAnalogOutputs();
};

#endif
