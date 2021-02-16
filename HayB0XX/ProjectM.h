#ifndef PROJECTM_H_QXV0YP7Q
#define PROJECTM_H_QXV0YP7Q

#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

class ProjectM : public ControllerMode {
public:
  ProjectM(socd::SocdType socdType, state::InputState &rInputState,
           CommunicationBackend *communicationBackend);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();
};

#endif /* end of include guard: PROJECTM_H_QXV0YP7Q */
