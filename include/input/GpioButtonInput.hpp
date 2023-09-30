#ifndef _INPUT_GPIOBUTTONINPUT_HPP
#define _INPUT_GPIOBUTTONINPUT_HPP

#include "core/InputSource.hpp"
#include "core/state.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

typedef struct {
    Button button;
    uint pin;
} GpioButtonMapping;

class GpioButtonInput : public InputSource {
  public:
    GpioButtonInput(GpioButtonMapping *button_mappings, size_t button_count);
    InputScanSpeed ScanSpeed();
    void UpdateInputs(InputState &inputs);

  protected:
    GpioButtonMapping *_button_mappings;
    size_t _button_count;
};

#endif