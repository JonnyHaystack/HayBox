#include "modes/FgcMode.hpp"

FgcMode::FgcMode(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 1;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right},
    };
}

void FgcMode::HandleSocd() {
    if (_inputs->down && (_inputs->mod_x || _inputs->c_up)) {
        _inputs->down = false;
    }
    InputMode::HandleSocd();
}

void FgcMode::UpdateDigitalOutputs() {
    // Directions
    _outputs->dpadLeft = _inputs->left;
    _outputs->dpadRight = _inputs->right;
    _outputs->dpadDown = _inputs->down;
    _outputs->dpadUp = _inputs->mod_x || _inputs->c_up;

    // Menu keys
    _outputs->start = _inputs->start;
    _outputs->select = _inputs->c_left;
    _outputs->home = _inputs->c_down;

    // Right hand bottom row
    _outputs->a = _inputs->b;
    _outputs->b = _inputs->x;
    _outputs->triggerRDigital = _inputs->z;
    _outputs->triggerLDigital = _inputs->up;

    // Right hand top row
    _outputs->x = _inputs->r;
    _outputs->y = _inputs->y;
    _outputs->buttonR = _inputs->lightshield;
    _outputs->buttonL = _inputs->midshield;
}

void FgcMode::UpdateAnalogOutputs() {
    _outputs->leftStickX = ANALOG_STICK_NEUTRAL;
    _outputs->leftStickY = ANALOG_STICK_NEUTRAL;
    _outputs->rightStickX = ANALOG_STICK_NEUTRAL;
    _outputs->rightStickY = ANALOG_STICK_NEUTRAL;
}
