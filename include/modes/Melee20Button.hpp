#ifndef _MODES_MELEE20BUTTON_HPP
#define _MODES_MELEE20BUTTON_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

#include <config.pb.h>

class Melee20Button : public ControllerMode {
  public:
    Melee20Button();
    void SetConfig(GameModeConfig &config, const MeleeOptions options);

  protected:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);

  private:
    MeleeOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
