#ifndef ROA_H_3X8XXUH0
#define ROA_H_3X8XXUH0

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class RoA : public ControllerMode {
public:
  RoA(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: ROA_H_3X8XXUH0 */
