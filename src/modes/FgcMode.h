#ifndef FGCMODE_H_K5EHYJDA
#define FGCMODE_H_K5EHYJDA

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class FgcMode : public ControllerMode {
public:
  FgcMode(socd::SocdType socdType, state::InputState &rInputState,
          CommunicationBackend *communicationBackend);
  void UpdateDigitalOutputs();
  void UpdateAnalogOutputs();

private:
  void HandleSocd();
};

#endif /* end of include guard: FGCMODE_H_K5EHYJDA */
