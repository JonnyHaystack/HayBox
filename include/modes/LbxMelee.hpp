#ifndef _MODES_LBXMELEE_HPP
#define _MODES_LBXMELEE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class LbxMelee : public ControllerMode {
  public:
    LbxMelee(socd::SocdType socd_type);

  private:
    bool horizontal_socd;

    void HandleSocd(InputState &inputs);
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
