#ifndef PROJECTM_H_QXV0YP7Q
#define PROJECTM_H_QXV0YP7Q

#include "core/CommunicationBackend.h"
#include "core/ControllerMode.h"
#include "core/socd.h"
#include "core/state.h"

class ProjectM : public ControllerMode {
public:
  ProjectM(socd::SocdType socdType, state::InputState &rInputState,
           CommunicationBackend *communicationBackend,
           bool ledgedashMaxJumpTraj, bool trueZPress);
  virtual void UpdateDigitalOutputs();
  virtual void UpdateAnalogOutputs();

private:
  void HandleSocd();
  bool mLedgedashMaxJumpTraj;
  bool mTrueZPress;
  bool mHorizontalSocd;
};

#endif /* end of include guard: PROJECTM_H_QXV0YP7Q */
