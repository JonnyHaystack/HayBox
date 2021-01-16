#ifndef MELEE18BUTTON_H_NI7QLO5Z
#define MELEE18BUTTON_H_NI7QLO5Z

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class Melee18Button : public ControllerMode {
public:
  Melee18Button(socd::SocdType socdType, state::InputState &rInputState,
                CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  bool mHorizontalSocd;
  void HandleSocd();
};

#endif /* end of include guard: MELEE18BUTTON_H_NI7QLO5Z */
