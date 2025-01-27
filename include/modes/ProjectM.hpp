#ifndef _MODES_PROJECTM_HPP
#define _MODES_PROJECTM_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

#include <config.pb.h>

class ProjectM : public ControllerMode {
  public:
    ProjectM();
    void SetConfig(GameModeConfig &config, const ProjectMOptions options);

  private:
    ProjectMOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
