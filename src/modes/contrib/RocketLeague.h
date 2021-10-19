#ifndef ROCKETLEAGUE_H_HX4RVTGL
#define ROCKETLEAGUE_H_HX4RVTGL

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class RocketLeague : public ControllerMode {
public:
  RocketLeague(socd::SocdType socdType, state::InputState &rInputState,
               CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  void HandleSocd();
};

#endif /* end of include guard: ROCKETLEAGUE_H_HX4RVTGL */
