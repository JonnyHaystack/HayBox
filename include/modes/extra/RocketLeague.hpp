#ifndef _MODES_ROCKETLEAGUE_HPP
#define _MODES_ROCKETLEAGUE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class RocketLeague : public ControllerMode {
  public:
    RocketLeague(socd::SocdType socd_type);

  private:
    void HandleSocd(InputState &inputs);
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
