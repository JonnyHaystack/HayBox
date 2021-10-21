#ifndef Ultimate_H_KHJ2DTIY
#define Ultimate_H_KHJ2DTIY

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class Ultimate : public ControllerMode {
public:
  Ultimate(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  bool mHorizontalSocd;
  void HandleSocd();
};

#endif /* end of include guard: Ultimate_H_KHJ2DTIY */
