#ifndef _MODES_MELEERSWAP_HPP
#define _MODES_MELEERSWAP_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} MeleeRSwapOptions;

class MeleeRSwap : public ControllerMode {
  public:
    MeleeRSwap(socd::SocdType socd_type, MeleeRSwapOptions options = {});

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    MeleeRSwapOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
