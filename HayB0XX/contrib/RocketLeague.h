#ifndef ROCKETLEAGUE_H_HX4RVTGL
#define ROCKETLEAGUE_H_HX4RVTGL

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

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
