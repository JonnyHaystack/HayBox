#include "modes/extra/DarkSouls.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

DarkSouls::DarkSouls() : ControllerMode() {}

void DarkSouls::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.y = inputs.rf6;
    outputs.x = inputs.rf5;

    // Base layer.
    bool layer0 = !inputs.rf2 && !inputs.nunchuk_c;
    // Secondary layer when X is held.
    bool layerX = inputs.rf2 && !inputs.nunchuk_c;
    // DPad layer when Nunchuk C is held.
    bool layerC = inputs.nunchuk_c;

    if (layer0) {
        outputs.a = inputs.rt1;
        outputs.b = inputs.rf1;
        outputs.buttonR = inputs.rf3;
        outputs.buttonL = inputs.rf4;
        outputs.start = inputs.mb1 | inputs.nunchuk_z;
    } else if (layerX) {
        outputs.rightStickClick = inputs.rt1;
        outputs.triggerRDigital = inputs.rf3;
        outputs.triggerLDigital = inputs.rf4;
        outputs.select = inputs.mb1;
    } else if (layerC) {
        outputs.a = inputs.rt1;
        outputs.dpadLeft = inputs.rf1;
        outputs.dpadDown = inputs.rf2;
        outputs.dpadUp = inputs.rf3;
        outputs.dpadRight = inputs.rf4;
        outputs.select = inputs.nunchuk_z;
    }
}

void DarkSouls::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    UpdateDirections(
        inputs.lf3,
        inputs.lf1,
        inputs.lf2,
        inputs.lt1,
        inputs.rt3,
        inputs.rt5,
        inputs.rt2,
        inputs.rt4,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
