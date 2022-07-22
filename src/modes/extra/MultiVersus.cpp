#include "modes/extra/MultiVersus.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MultiVersus::MultiVersus(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    horizontal_socd = false;
}

void MultiVersus::HandleSocd(InputState &inputs) {
    horizontal_socd = inputs.left && inputs.right;
    InputMode::HandleSocd(inputs);
}

void MultiVersus::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonL = inputs.lightshield;
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;
    outputs.select = inputs.select || inputs.midshield;
    outputs.home = inputs.home;

    const bool l_pressed = (inputs.nunchuk_connected && inputs.nunchuk_z) || inputs.l;

    // Use D-Pad as "neutral" binds
    if (inputs.mod_x) {
        outputs.dpadLeft = inputs.a;
        outputs.dpadRight = inputs.b;
        outputs.dpadDown = inputs.r || inputs.z || l_pressed;
    } else {
        outputs.a = inputs.a;
        outputs.b = inputs.b;
        outputs.triggerRDigital = inputs.r;
        outputs.buttonR = inputs.z;
        outputs.triggerLDigital = l_pressed;
    }

    outputs.dpadUp = inputs.mod_x && inputs.mod_y && inputs.c_up;
}

void MultiVersus::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 140;
    }
    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 140;
    }
}
