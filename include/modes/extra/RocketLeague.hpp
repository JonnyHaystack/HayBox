#ifndef _MODES_ROCKETLEAGUE_HPP
#define _MODES_ROCKETLEAGUE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class RocketLeague : public ControllerMode {
  public:
    RocketLeague(socd::SocdType socd_type);

  private:
    void HandleSocd();
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
