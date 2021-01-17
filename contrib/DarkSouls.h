#ifndef DARKSOULS_H_MBFY6REI
#define DARKSOULS_H_MBFY6REI

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class DarkSouls : public ControllerMode {
public:
  DarkSouls(socd::SocdType socdType, state::InputState &rInputState,
            CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: DARKSOULS_H_MBFY6REI */
