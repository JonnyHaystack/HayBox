#include "modes/RivalsOfAether.hpp"

RivalsOfAether::RivalsOfAether(socd::SocdType socd_type)
    : PlatformFighter(socd_type) { }

void RivalsOfAether::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a;
    _outputs->b = _inputs->b;
    _outputs->x = _inputs->x;
    _outputs->y = _inputs->y;
    _outputs->buttonR = _inputs->z;
    if (_inputs->nunchuk_connected) {
        // Lightshield with C button.
        if (_inputs->nunchuk_c) {
            _outputs->triggerLAnalog = 49;
        }
        _outputs->triggerLDigital = _inputs->nunchuk_z;
    } else {
        _outputs->triggerLDigital = _inputs->l;
    }
    _outputs->triggerRDigital = _inputs->r;
    _outputs->start = _inputs->start;

    /********* DPAD *********/
    if (_inputs->mod_x && _inputs->mod_y) {
        _outputs->dpadUp = _inputs->c_up;
        _outputs->dpadDown = _inputs->c_down;
        _outputs->dpadLeft = _inputs->c_left;
        _outputs->dpadRight = _inputs->c_right;
    }

    _outputs->select = _inputs->select;
    _outputs->home = _inputs->home;
}

void RivalsOfAether::UpdateAnalogOutputs() {
    UpdateDirections();

    bool shield_button_pressed = _inputs->l || _inputs->r;

    if (_inputs->mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(6600);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetAngledFSmash(6500, 2300); // 19.48613°
        }

        // Need to check coord system in RoA

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(5900, 2300); // 21.29735°

            if (_inputs->c_down) {
                SetLeftStick(4900, 2400); // 26.09542°
            }

            if (_inputs->c_left) {
                SetLeftStick(5200, 3100); // 30.80145°
            }

            if (_inputs->c_up) {
                SetLeftStick(4900, 3500); // 35.53768°
            }

            if (_inputs->c_right) {
                SetLeftStick(5100, 4300); // 40.13549°
            }
        }
    }

    if (_inputs->mod_y) {
        if (directions.horizontal) {
            SetLeftStickX(4400);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(4400, 11300); // 68.72503°

            if (_inputs->c_down) {
                SetLeftStick(4400, 9000); // 63.9465°
            }

            if (_inputs->c_left) {
                SetLeftStick(4400, 7400); // 59.26451°
            }

            if (_inputs->c_up) {
                SetLeftStick(4500, 6300); // 54.46232°
            }

            if (_inputs->c_right) {
                SetLeftStick(4700, 5700); // 50.49232°
            }
        }
    }

    if (_inputs->l) {
        if (directions.horizontal)
            SetLeftStickX(10000);
        if (directions.vertical)
            SetLeftStickY(10000);
        if (directions.horizontal && directions.y < 0) {
            SetLeftStick(10000, 2800); // 15.64225°
        }
    }

    if (_inputs->r) {
        if (directions.diagonal) {
            if (_inputs->mod_y) {
                SetLeftStick(4000, 6800); // 59.53446°
            }
        }
    }

    // Shut off c-stick when using dpad layer.
    if (_inputs->mod_x && _inputs->mod_y) {
        SetLeftStick(0000, 0000); // 0°
    }

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
