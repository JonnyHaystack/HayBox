#include "input/GpioButtonInput.hpp"

#include "gpio.hpp"

GpioButtonInput::GpioButtonInput(GpioButtonMapping *button_mappings, size_t button_count) {
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
        GpioButtonMapping button_mapping = _button_mappings[i];
        inputs.*(button_mapping.button) = !gpio::read_digital(button_mapping.pin);
    }
}
