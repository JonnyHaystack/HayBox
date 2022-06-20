#ifndef _MODES_LBXMELEE_HPP
#define _MODES_LBXMELEE_HPP

#include "modes/Melee20Button.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class LbxMelee : public Melee20Button {
  public:
    LbxMelee(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
