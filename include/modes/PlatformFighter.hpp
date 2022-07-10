#ifndef _MODES_PLATFORMFIGHTER_HPP
#define _MODES_PLATFORMFIGHTER_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class PlatformFighter : public ControllerMode {
  public:
    PlatformFighter(socd::SocdType socd_type, uint8_t analog_stick_length = 100);

  protected:
    bool _horizontal_socd;

    void HandleSocd();
};

#endif
