#ifndef Ultimate2_H_KHJ2DTIY
#define Ultimate2_H_KHJ2DTIY

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class Ultimate2 : public ControllerMode {
public:
  Ultimate2(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  bool mHorizontalSocd;
  void HandleSocd();
};

#endif /* end of include guard: Ultimate2_H_KHJ2DTIY */
