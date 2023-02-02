#include "modes/extra/HollowKnight.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

HollowKnight::HollowKnight(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void HollowKnight::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a; // Attack
    outputs.b = inputs.b; // Dash
    outputs.x = inputs.x; // Jump
    outputs.y = inputs.mod_y; // Spell
    outputs.triggerLDigital = inputs.r; // Focus/cast
    outputs.triggerRDigital = inputs.z;
    outputs.buttonR = inputs.up; // Dream nail

    outputs.buttonL = inputs.lightshield; // Map
    outputs.select = inputs.midshield; // Inventory
    outputs.start = inputs.start; // Pause
}

void HollowKnight::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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
}
