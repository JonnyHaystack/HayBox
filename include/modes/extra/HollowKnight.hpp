#ifndef _MODES_HOLLOWKNIGHT_HPP
#define _MODES_HOLLOWKNIGHT_HPP

#include "core/CommunicationBackend.hpp"
#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class HollowKnight : public ControllerMode {
  public:
    HollowKnight(GameModeConfig &config);

  private:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
