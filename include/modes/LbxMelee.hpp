#ifndef _MODES_LbxMelee_HPP
#define _MODES_LbxMelee_HPP

#include "modes/Melee20Button.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class LbxMelee : public Melee20Button {
  public:
    LbxMelee(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
};

#endif
