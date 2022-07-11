#include "modes/extra/MKWii.hpp"

MKWii::MKWii(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right},
        socd::SocdPair{ &InputState::l,   &InputState::down },
        socd::SocdPair{ &InputState::l,   &InputState::mod_x},
        socd::SocdPair{ &InputState::l,   &InputState::mod_y},
    };
}

void MKWii::UpdateDigitalOutputs() {
    _outputs->a = _inputs->b;
    _outputs->b = _inputs->x;
    _outputs->triggerLDigital = _inputs->z;
    _outputs->buttonR = _inputs->up;
    _outputs->dpadUp = _inputs->a;
    _outputs->start = _inputs->start;
}

void MKWii::UpdateAnalogOutputs() {
    bool up = _inputs->down || _inputs->mod_x || _inputs->mod_y;

    UpdateDirections(
        _inputs->left,
        _inputs->right,
        _inputs->l,
        up,
        false,
        false,
        false,
        false
    );

    if (_inputs->z) {
        _outputs->triggerLAnalog = 140;
    }

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
