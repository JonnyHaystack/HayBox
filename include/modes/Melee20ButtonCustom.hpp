#ifndef _MODES_MELEE20BUTTONCUSTOM_HPP
#define _MODES_MELEE20BUTTONCUSTOM_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Melee20ButtonCustom : public ControllerMode {
  public:
    Melee20ButtonCustom(socd::SocdType socd_type);

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    bool horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
