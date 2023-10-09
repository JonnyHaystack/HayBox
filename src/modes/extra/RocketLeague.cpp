#include "modes/extra/RocketLeague.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

RocketLeague::RocketLeague(const GameModeConfig &config) : ControllerMode(config) {}

void RocketLeague::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.rt1;
    outputs.b = inputs.rf1;
    outputs.x = inputs.rf8;
    outputs.y = inputs.rf4;
    outputs.buttonL = inputs.lf4;
    outputs.buttonR = inputs.rf7;
    outputs.triggerLDigital = inputs.rf3;
    outputs.triggerRDigital = inputs.rf2;
    outputs.leftStickClick = inputs.rf5;

    // Hold accelerate and reverse simultaneously for rear view.
    if (inputs.rf3 && inputs.rf2) {
        outputs.rightStickClick = true;
        // Override (deactivate) accelerator.
        outputs.triggerRDigital = false;
    }

    // MX + Start = Select
    if (inputs.lt1)
        outputs.select = inputs.mb1;
    else
        outputs.start = inputs.mb1;

    // D-Pad
    if (inputs.lt1 && inputs.lt2) {
        outputs.dpadUp = inputs.rt4;
        outputs.dpadDown = inputs.rt2;
        outputs.dpadLeft = inputs.rt3;
        outputs.dpadRight = inputs.rt5;
    }
}

void RocketLeague::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
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

    if (inputs.lt2) {
        if (directions.diagonal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 70);
            // outputs.leftStickY =
            // ANALOG_STICK_NEUTRAL + (directions.y * 76);
        } else if (directions.horizontal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 35);
        } else if (directions.vertical) {
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 76);
        }
    } else if (directions.diagonal) {
        // Good speed flip angle with no mods.
        outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 70);
    }

    // Shut off right stick when using dpad layer.
    if (inputs.lt1 && inputs.lt2) {
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
