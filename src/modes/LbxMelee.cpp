#include "modes/LbxMelee.hpp"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

LbxMelee::LbxMelee(socd::SocdType socd_type) : Melee20Button(socd_type) {
}

void LbxMelee::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    Melee20Button::UpdateDigitalOutputs(inputs, outputs);
    if (inputs.select || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void LbxMelee::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    Melee20Button::UpdateAnalogOutputs(inputs, outputs);
    if (inputs.select) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }
}
