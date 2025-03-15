#include "input/Pca9671Input.hpp"

#include "util/state_util.hpp"

Pca9671Input::Pca9671Input(
    const Pca9671ButtonMapping *button_mappings,
    size_t button_count,
    TwoWire &wire,
    int sda_pin,
    int scl_pin,
    uint8_t i2c_addr
)
    : _pcf(i2c_addr, &wire) {
    _button_mappings = button_mappings;
    _button_count = button_count;

#ifdef ARDUINO_PICO_REVISION
    wire.setClock(3'000'000);
    if (sda_pin > 0 && scl_pin > 0) {
        wire.setSDA(sda_pin);
        wire.setSCL(scl_pin);
    }
#endif

    wire.begin();
    _pcf.begin();
}

InputScanSpeed Pca9671Input::ScanSpeed() {
    return InputScanSpeed::FAST;
}

void Pca9671Input::UpdateInputs(InputState &inputs) {
    uint16_t pin_values = _pcf.read16();

    for (size_t i = 0; i < _button_count; i++) {
        UpdateButtonState(inputs, i, !(pin_values & (1 << _button_mappings[i].bit)));
    }
}

void Pca9671Input::UpdateButtonState(
    InputState &inputs,
    size_t button_mapping_index,
    bool pressed
) {
    set_button(inputs.buttons, _button_mappings[button_mapping_index].button, pressed);
}