#ifndef SHOVELKNIGHT_H_J4VTP91C
#define SHOVELKNIGHT_H_J4VTP91C

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class ShovelKnight : public ControllerMode {
public:
  ShovelKnight(socd::SocdType socdType, state::InputState &rInputState,
               CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: SHOVELKNIGHT_H_J4VTP91C */
