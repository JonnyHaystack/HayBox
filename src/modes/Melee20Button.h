#ifndef MELEE20BUTTON_H_KHJ2DTIY
#define MELEE20BUTTON_H_KHJ2DTIY

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class Melee20Button : public ControllerMode {
public:
  Melee20Button(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  bool mHorizontalSocd;
  void HandleSocd();
};

#endif /* end of include guard: MELEE20BUTTON_H_KHJ2DTIY */
