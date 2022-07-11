#ifndef _MODES_SHOVELKNIGHT_HPP
#define _MODES_SHOVELKNIGHT_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class ShovelKnight : public ControllerMode {
  public:
    ShovelKnight(socd::SocdType socd_type);

  private:
    virtual void UpdateDigitalOutputs();
    virtual void UpdateAnalogOutputs();
};

#endif
