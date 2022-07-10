#ifndef _MODES_MELEE18BUTTON_HPP
#define _MODES_MELEE18BUTTON_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Melee18Button : public ControllerMode {
  public:
    Melee18Button(socd::SocdType socd_type);

  private:
    bool horizontal_socd;

    void HandleSocd();
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
