#ifndef _MODES_MELEE18BUTTON_HPP
#define _MODES_MELEE18BUTTON_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} Melee18ButtonOptions;

class Melee18Button : public ControllerMode {
  public:
    Melee18Button(socd::SocdType socd_type, Melee18ButtonOptions options = {});

  private:
    Melee18ButtonOptions _options;
    bool horizontal_socd;

    void HandleSocd(InputState &inputs);
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
