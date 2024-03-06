#include "modes/extra/MKWii.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MKWii::MKWii(GameModeConfig &config) : ControllerMode(config) {}

void MKWii::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.rf1;
    outputs.b = inputs.rf2;
    outputs.triggerLDigital = inputs.rf3;
    outputs.buttonR = inputs.rf4;
    outputs.dpadUp = inputs.rt1;
    outputs.start = inputs.mb1;
}

void MKWii::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    bool up = inputs.lf2 || inputs.lt1 || inputs.lt2;

    UpdateDirections(
        inputs.lf3,
        inputs.lf1,
        inputs.lf4,
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

    if (inputs.rf3) {
        outputs.triggerLAnalog = 140;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
