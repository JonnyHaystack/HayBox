#ifndef _MODES_FGCMODE_HPP
#define _MODES_FGCMODE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class FgcMode : public ControllerMode {
  public:
    FgcMode(socd::SocdType socd_type);

  private:
    void HandleSocd();
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
