#include "modes/extra/ShovelKnight.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

ShovelKnight::ShovelKnight(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::mod_x,   socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void ShovelKnight::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.dpadLeft = inputs.left;
    outputs.dpadRight = inputs.right;
    outputs.dpadDown = inputs.down;
    outputs.dpadUp = inputs.mod_x;

    outputs.b = inputs.x; // Jump
    outputs.a = inputs.a; // Attack
    outputs.y = inputs.b; // Attack
    outputs.x = inputs.z; // Subweapon
    outputs.buttonL = inputs.r; // Subweapon prev
    outputs.buttonR = inputs.y; // Subweapon next

    outputs.select = inputs.lightshield; // Inventory
    outputs.start = inputs.start; // Pause
}

void ShovelKnight::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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
