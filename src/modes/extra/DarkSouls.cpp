#include "modes/extra/DarkSouls.hpp"

DarkSouls::DarkSouls(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void DarkSouls::UpdateDigitalOutputs() {
    _outputs->y = _inputs->y;
    _outputs->x = _inputs->r;

    // Base layer.
    bool layer0 = !_inputs->x && !_inputs->nunchuk_c;
    // Secondary layer when X is held.
    bool layerX = _inputs->x && !_inputs->nunchuk_c;
    // DPad layer when Nunchuk C is held.
    bool layerC = _inputs->nunchuk_c;

    if (layer0) {
        _outputs->a = _inputs->a;
        _outputs->b = _inputs->b;
        _outputs->buttonR = _inputs->z;
        _outputs->buttonL = _inputs->up;
        _outputs->start = _inputs->start | _inputs->nunchuk_z;
    } else if (layerX) {
        _outputs->rightStickClick = _inputs->a;
        _outputs->triggerRDigital = _inputs->z;
        _outputs->triggerLDigital = _inputs->up;
        _outputs->select = _inputs->start;
    } else if (layerC) {
        _outputs->a = _inputs->a;
        _outputs->dpadLeft = _inputs->b;
        _outputs->dpadDown = _inputs->x;
        _outputs->dpadUp = _inputs->z;
        _outputs->dpadRight = _inputs->up;
        _outputs->select = _inputs->nunchuk_z;
    }
}

void DarkSouls::UpdateAnalogOutputs() {
    UpdateDirections(
        _inputs->left,
        _inputs->right,
        _inputs->down,
        _inputs->mod_x,
        _inputs->c_left,
        _inputs->c_right,
        _inputs->c_down,
        _inputs->c_up
    );

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
