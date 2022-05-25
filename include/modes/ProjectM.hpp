#ifndef _MODES_PROJECTM_HPP
#define _MODES_PROJECTM_HPP

#include "core/CommunicationBackend.hpp"
#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class ProjectM : public ControllerMode {
  public:
    ProjectM(socd::SocdType socd_type, bool ledgedash_max_jump_traj, bool true_z_press);
    virtual void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    void HandleSocd(InputState &inputs);
    bool ledgedash_max_jump_traj;
    bool true_z_press;
    bool horizontal_socd;
};

#endif
