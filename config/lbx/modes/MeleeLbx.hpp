#ifndef _MODES_MELEELBX_HPP
#define _MODES_MELEELBX_HPP

#include "config/lbx/modes/MeleeLbx.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "modes/Melee20Button.hpp"

class MeleeLbx : public Melee20Button {
  public:
    MeleeLbx(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
