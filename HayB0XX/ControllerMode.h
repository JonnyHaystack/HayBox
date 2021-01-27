#ifndef CONTROLLERMODE_H_KAANOLIC
#define CONTROLLERMODE_H_KAANOLIC

#include "CommunicationBackend.h"
#include "InputMode.h"
#include "socd.h"
#include "state.h"

class ControllerMode : public InputMode {
public:
  ControllerMode(socd::SocdType socdType, state::InputState &rInputState,
                 CommunicationBackend *communicationBackend);
  void ResetOutputs();
  void UpdateOutputs();
  void ResetVectors();
  virtual void HandleVectors(bool lsLeft, bool lsRight, bool lsDown, bool lsUp,
                             bool rsLeft, bool rsRight, bool rsDown, bool rsUp,
                             uint8_t analogStickMin, uint8_t analogStickNeutral,
                             uint8_t analogStickMax);
  virtual void UpdateDigitalOutputs() = 0;
  virtual void UpdateAnalogOutputs() = 0;

protected:
  state::VectorState mVectorState;
  state::OutputState mOutputState;
  CommunicationBackend *mpCommunicationBackend;
};

#endif /* end of include guard: CONTROLLERMODE_H_KAANOLIC */
