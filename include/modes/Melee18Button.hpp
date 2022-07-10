#ifndef _MODES_MELEE18BUTTON_HPP
#define _MODES_MELEE18BUTTON_HPP

#include "modes/PlatformFighter.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Melee18Button : public PlatformFighter {
  public:
    Melee18Button(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
