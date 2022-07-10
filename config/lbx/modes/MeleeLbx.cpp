#include "MeleeLbx.hpp"

#define ANALOG_STICK_LENGTH 80

MeleeLbx::MeleeLbx(socd::SocdType socd_type) : Melee20Button(socd_type) {}

void MeleeLbx::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    Melee20Button::UpdateDigitalOutputs(inputs, outputs);
    if (inputs.select || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void MeleeLbx::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    Melee20Button::UpdateAnalogOutputs(inputs, outputs);
    if (inputs.select) {
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
    }
}
