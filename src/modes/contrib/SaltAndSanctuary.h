#ifndef SALTANDSANCTUARY_H_LHTFQNFK
#define SALTANDSANCTUARY_H_LHTFQNFK

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class SaltAndSanctuary : public ControllerMode {
public:
  SaltAndSanctuary(socd::SocdType socdType, state::InputState &rInputState,
                   CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: SALTANDSANCTUARY_H_LHTFQNFK */
