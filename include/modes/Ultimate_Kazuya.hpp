#ifndef _MODES_ULTIMATE_KAZUYA_HPP
#define _MODES_ULTIMATE_KAZUYA_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Ultimate_Kazuya: public ControllerMode {
  public:
    Ultimate_Kazuya(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
