#ifndef ROA_H_3X8XXUH0
#define ROA_H_3X8XXUH0

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class RoA : public ControllerMode {
public:
  RoA(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: ROA_H_3X8XXUH0 */
