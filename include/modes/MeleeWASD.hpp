#ifndef _MODES_MELEEWASD_HPP
#define _MODES_MELEEWASD_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} MeleeWASDOptions;

class MeleeWASD : public ControllerMode {
  public:
    MeleeWASD(socd::SocdType socd_type, MeleeWASDOptions options = {});

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    MeleeWASDOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
