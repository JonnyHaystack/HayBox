#include "modes/extra/SaltAndSanctuary.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

SaltAndSanctuary::SaltAndSanctuary(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void SaltAndSanctuary::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.dpadRight = inputs.l; // Block
    outputs.b = inputs.b; // Roll
    outputs.a = inputs.a; // Attack
    outputs.y = inputs.z; // Strong
    outputs.dpadDown = inputs.mod_y; // Use
    outputs.x = inputs.x; // Jump

    outputs.buttonL = inputs.r; // Previous item
    outputs.buttonR = inputs.y; // Next item
    outputs.triggerLDigital = inputs.lightshield; // Use item

    outputs.triggerRDigital = inputs.midshield; // Use torch

    outputs.dpadLeft = inputs.up; // Switch loadout

    outputs.start = inputs.start; // Inventory
}

void SaltAndSanctuary::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    bool up_button_pressed = inputs.mod_x || inputs.up2;
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        up_button_pressed,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );
}
