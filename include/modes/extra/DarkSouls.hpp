#ifndef _MODES_DARKSOULS_HPP
#define _MODES_DARKSOULS_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class DarkSouls : public ControllerMode {
  public:
    DarkSouls(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs);
    void UpdateAnalogOutputs(InputState &inputs);
};

#endif
