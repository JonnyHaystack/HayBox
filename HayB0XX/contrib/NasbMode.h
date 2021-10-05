#ifndef NASBMODE_H_C65L457Q
#define NASBMODE_H_C65L457Q

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class NasbMode : public ControllerMode {
public:
  NasbMode(state::InputState &rInputState, CommunicationBackend *communicationBackend);
  void UpdateDigitalOutputs();
  void UpdateAnalogOutputs();

private:
  void HandleSocd();
};

#endif /* end of include guard: NASBMODE_H_C65L457Q */
