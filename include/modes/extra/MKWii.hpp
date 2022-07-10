#ifndef _MODES_MKWII_HPP
#define _MODES_MKWII_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class MKWii : public ControllerMode {
  public:
    MKWii(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs(InputState &inputs);
    void UpdateAnalogOutputs(InputState &inputs);
};

#endif
