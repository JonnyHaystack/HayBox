#ifndef _MODES_MULTIVERSUS_HPP
#define _MODES_MULTIVERSUS_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class MultiVersus : public ControllerMode {
  public:
    MultiVersus(socd::SocdType socd_type);

  protected:
    virtual void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
