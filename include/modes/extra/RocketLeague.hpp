#ifndef _MODES_ROCKETLEAGUE_HPP
#define _MODES_ROCKETLEAGUE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class RocketLeague : public ControllerMode {
  public:
    RocketLeague(const GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
