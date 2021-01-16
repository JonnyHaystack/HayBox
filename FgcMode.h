#ifndef FGCMODE_H_K5EHYJDA
#define FGCMODE_H_K5EHYJDA

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class FgcMode : public ControllerMode {
public:
  FgcMode(socd::SocdType socdType, state::InputState &rInputState,
          CommunicationBackend *communicationBackend);
  void UpdateDigitalOutputs();
  void UpdateAnalogOutputs();
};

#endif /* end of include guard: FGCMODE_H_K5EHYJDA */
