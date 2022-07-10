#ifndef _MODES_SALTANDSANCTUARY_HPP
#define _MODES_SALTANDSANCTUARY_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class SaltAndSanctuary : public ControllerMode {
  public:
    SaltAndSanctuary(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs);
    void UpdateAnalogOutputs(InputState &inputs);
};

#endif
