#ifndef _MODES_MKWII_HPP
#define _MODES_MKWII_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class MKWii : public ControllerMode {
  public:
    MKWii(GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
