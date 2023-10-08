#include "modes/extra/MKWii.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MKWii::MKWii(const GameModeConfig &config) : ControllerMode(config) {}

void MKWii::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.b;
    outputs.b = inputs.x;
    outputs.triggerLDigital = inputs.z;
    outputs.buttonR = inputs.up;
    outputs.dpadUp = inputs.a;
    outputs.start = inputs.start;
}

void MKWii::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    bool up = inputs.down || inputs.mod_x || inputs.mod_y;

    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.l,
        up,
        false,
        false,
        false,
        false,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    if (inputs.z) {
        outputs.triggerLAnalog = 140;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
