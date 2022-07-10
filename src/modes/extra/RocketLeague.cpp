#include "modes/extra/RocketLeague.hpp"

RocketLeague::RocketLeague(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 3;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void RocketLeague::HandleSocd() {
    if (_inputs->mod_x && _inputs->down) {
        _inputs->down = false;
    }
    InputMode::HandleSocd();
}

void RocketLeague::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a;
    _outputs->b = _inputs->b;
    _outputs->x = _inputs->midshield;
    _outputs->y = _inputs->up;
    _outputs->buttonL = _inputs->l;
    _outputs->buttonR = _inputs->lightshield;
    _outputs->triggerLDigital = _inputs->z;
    _outputs->triggerRDigital = _inputs->x;
    _outputs->leftStickClick = _inputs->r;

    // Hold accelerate and reverse simultaneously for rear view.
    if (_inputs->z && _inputs->x) {
        _outputs->rightStickClick = true;
        // Override (deactivate) accelerator.
        _outputs->triggerRDigital = false;
    }

    // MX + Start = Select
    if (_inputs->mod_x)
        _outputs->select = _inputs->start;
    else
        _outputs->start = _inputs->start;

    // D-Pad
    if (_inputs->mod_x && _inputs->mod_y) {
        _outputs->dpadUp = _inputs->c_up;
        _outputs->dpadDown = _inputs->c_down;
        _outputs->dpadLeft = _inputs->c_left;
        _outputs->dpadRight = _inputs->c_right;
    }
}

void RocketLeague::UpdateAnalogOutputs() {
    // Coordinate calculations to make modifier handling simpler.
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

    if (_inputs->mod_y) {
        if (directions.diagonal) {
            _outputs->leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 70);
            // outputs->leftStickY =
            // ANALOG_STICK_NEUTRAL + (directions.y * 76);
        } else if (directions.horizontal) {
            _outputs->leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 35);
        } else if (directions.vertical) {
            _outputs->leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 76);
        }
    } else if (directions.diagonal) {
        // Good speed flip angle with no mods.
        _outputs->leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 70);
    }

    // Shut off right stick when using dpad layer.
    if (_inputs->mod_x && _inputs->mod_y) {
        _outputs->rightStickX = ANALOG_STICK_NEUTRAL;
        _outputs->rightStickY = ANALOG_STICK_NEUTRAL;
    }

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
