#ifndef _MODES_SALTANDSANCTUARY_HPP
#define _MODES_SALTANDSANCTUARY_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class SaltAndSanctuary : public ControllerMode {
  public:
    SaltAndSanctuary(const GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
