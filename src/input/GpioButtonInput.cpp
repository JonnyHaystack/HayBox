#include "input/GpioButtonInput.hpp"

#include "gpio.hpp"
#include "util/state_util.hpp"

GpioButtonInput::GpioButtonInput(const GpioButtonMapping *button_mappings, size_t button_count) {
    _button_mappings = button_mappings;
    _button_count = button_count;

    // Initialize button pins.
    for (size_t i = 0; i < _button_count; i++) {
        uint pin = _button_mappings[i].pin;
        gpio::init_pin(pin, gpio::GpioMode::GPIO_INPUT_PULLUP);
    }
}

InputScanSpeed GpioButtonInput::ScanSpeed() {
    return InputScanSpeed::FAST;
}

void GpioButtonInput::UpdateInputs(InputState &inputs) {
    for (size_t i = 0; i < _button_count; i++) {
        UpdateButtonState(inputs, i, !gpio::read_digital(_button_mappings[i].pin));
    }
}

void GpioButtonInput::UpdateButtonState(
    InputState &inputs,
    size_t button_mapping_index,
    bool pressed
) {
    set_button(inputs.buttons, _button_mappings[button_mapping_index].button, pressed);
}
