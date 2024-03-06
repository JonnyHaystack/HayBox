#ifndef _MODES_RIVALSOFAETHER_HPP
#define _MODES_RIVALSOFAETHER_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

class RivalsOfAether : public ControllerMode {
  public:
    RivalsOfAether(GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
