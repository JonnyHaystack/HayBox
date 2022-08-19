#ifndef _MODES_SMASHBOXCLONE_HPP
#define _MODES_SMASHBOXCLONE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class SmashboxClone : public ControllerMode {
  public:
    SmashboxClone(socd::SocdType socd_type);

  private:
    RemappedInputState mapped;
    int b2_counter;
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
    void HandleSocd(InputState &inputs);
};

#endif
