#ifndef _MODES_MELEE20BUTTON_HPP
#define _MODES_MELEE20BUTTON_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Melee20Button : public ControllerMode {
  public:
    Melee20Button(socd::SocdType socd_type);

  protected:
    virtual void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    bool horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
