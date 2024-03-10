#ifndef _MODES_MELEE20BUTTON_HPP
#define _MODES_MELEE20BUTTON_HPP

#include "core/ControllerMode.hpp"
#include "core/state.hpp"

#include <config.pb.h>

typedef struct {
    bool crouch_walk_os = false;
} Melee20ButtonOptions;

class Melee20Button : public ControllerMode {
  public:
    Melee20Button(Melee20ButtonOptions options = {});

  protected:
    void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);

  private:
    Melee20ButtonOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
