#include "modes/FgcMode.hpp"

FgcMode::FgcMode(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 1;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right},
    };
}

void FgcMode::HandleSocd(InputState &inputs) {
    if (inputs.down && (inputs.mod_x || inputs.c_up)) {
        inputs.down = false;
    }
    InputMode::HandleSocd(inputs);
}

void FgcMode::UpdateDigitalOutputs(InputState &inputs) {
    // Directions
    outputs->dpadLeft = inputs.left;
    outputs->dpadRight = inputs.right;
    outputs->dpadDown = inputs.down;
    outputs->dpadUp = inputs.mod_x || inputs.c_up;

    // Menu keys
    outputs->start = inputs.start;
    outputs->select = inputs.c_left;
    outputs->home = inputs.c_down;

    // Right hand bottom row
    outputs->a = inputs.b;
    outputs->b = inputs.x;
    outputs->triggerRDigital = inputs.z;
    outputs->triggerLDigital = inputs.up;

    // Right hand top row
    outputs->x = inputs.r;
    outputs->y = inputs.y;
    outputs->buttonR = inputs.lightshield;
    outputs->buttonL = inputs.midshield;
}

void FgcMode::UpdateAnalogOutputs(InputState &inputs) {
    SetLeftStick(0000, 0000);
    SetRightStick(0000, 0000);
}
