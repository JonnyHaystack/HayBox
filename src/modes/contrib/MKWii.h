#ifndef MKWII_H_YYSTFB84
#define MKWII_H_YYSTFB84

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class MKWii : public ControllerMode {
public:
  MKWii(socd::SocdType socdType, state::InputState &rInputState,
        CommunicationBackend *communicationBackend);
  void UpdateDigitalOutputs();
  void UpdateAnalogOutputs();
};

#endif /* end of include guard: MKWII_H_YYSTFB84 */
