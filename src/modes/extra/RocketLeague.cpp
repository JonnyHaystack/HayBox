#include "modes/extra/RocketLeague.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

RocketLeague::RocketLeague(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type               },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x,   socd::SOCD_DIR2_PRIORITY},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type               },
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type               },
    };
}

void RocketLeague::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.midshield;
    outputs.y = inputs.up;
    outputs.buttonL = inputs.l;
    outputs.buttonR = inputs.lightshield;
    outputs.triggerLDigital = inputs.z;
    outputs.triggerRDigital = inputs.x;
    outputs.leftStickClick = inputs.r;

    // Hold accelerate and reverse simultaneously for rear view.
    if (inputs.z && inputs.x) {
        outputs.rightStickClick = true;
        // Override (deactivate) accelerator.
        outputs.triggerRDigital = false;
    }

    // MX + Start = Select
    if (inputs.mod_x)
        outputs.select = inputs.start;
    else
        outputs.start = inputs.start;

    // D-Pad
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void RocketLeague::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.mod_x,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    if (inputs.mod_y) {
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
    if (inputs.mod_x && inputs.mod_y) {
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
