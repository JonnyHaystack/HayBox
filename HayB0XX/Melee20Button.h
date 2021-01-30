#ifndef MELEE20BUTTON_H_KHJ2DTIY
#define MELEE20BUTTON_H_KHJ2DTIY

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class Melee20Button : public ControllerMode {
public:
  Melee20Button(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  bool mHorizontalSocd;
  int mFoxCssMacro;
  void HandleSocd();
  int PickFox(int step);
};

#endif /* end of include guard: MELEE20BUTTON_H_KHJ2DTIY */
