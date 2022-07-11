#include "modes/ProjectM.hpp"

ProjectM::ProjectM(socd::SocdType socd_type, bool ledgedash_max_jump_traj, bool true_z_press)
    : PlatformFighter(socd_type) {
    this->ledgedash_max_jump_traj = ledgedash_max_jump_traj;
    this->true_z_press = true_z_press;
}

void ProjectM::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a;
    _outputs->b = _inputs->b;
    _outputs->x = _inputs->x;
    _outputs->y = _inputs->y;
    // True Z press vs macro lightshield + A.
    if (true_z_press || _inputs->mod_x) {
        _outputs->buttonR = _inputs->z;
    } else {
        _outputs->a = _inputs->a || _inputs->z;
    }
    if (_inputs->nunchuk_connected) {
        _outputs->triggerLDigital = _inputs->nunchuk_z;
    } else {
        _outputs->triggerLDigital = _inputs->l;
    }
    _outputs->triggerRDigital = _inputs->r;
    _outputs->start = _inputs->start;

    // D-Pad layer can be activated by holding Mod X + Mod Y, or by holding the C
    // button on a nunchuk.
    if ((_inputs->mod_x && _inputs->mod_y) || _inputs->nunchuk_c) {
        _outputs->dpadUp = _inputs->c_up;
        _outputs->dpadDown = _inputs->c_down;
        _outputs->dpadLeft = _inputs->c_left;
        _outputs->dpadRight = _inputs->c_right;
    }

    // Don't override dpad up if it's already pressed using the MX + MY dpad
    // layer.
    _outputs->dpadUp = _outputs->dpadUp || _inputs->midshield;

    if (_inputs->select)
        _outputs->dpadLeft = true;
    if (_inputs->home)
        _outputs->dpadRight = true;
}

void ProjectM::UpdateAnalogOutputs() {
    UpdateDirections();

    bool shield_button_pressed = _inputs->l || _inputs->lightshield || _inputs->midshield;

    if (directions.diagonal) {
        if (directions.y > 0) {
            SetLeftStick(8300, 9300); // 48.25195°
        }
    }

    if (_inputs->mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(7000);
        }
        if (directions.vertical) {
            SetLeftStickY(6000);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetAngledFSmash(6500, 2300); // 19.48613°
        }

        if (directions.diagonal) {
            SetLeftStick(7000, 3400); // 25.90651°

            if (_inputs->b) {
                SetLeftStick(8500, 3100); // 20.03721°
            }

            if (_inputs->r) {
                SetLeftStick(8200, 3500); // 23.11421°
            }

            if (_inputs->c_up) {
                SetLeftStick(7700, 5500); // 35.53768°
            }

            if (_inputs->c_down) {
                SetLeftStick(8200, 3600); // 23.70265°
            }

            if (_inputs->c_left) {
                SetLeftStick(8400, 5000); // 30.76272°
            }

            if (_inputs->c_right) {
                SetLeftStick(7200, 6100); // 40.27201°
            }
        }
    }

    if (_inputs->mod_y) {
        if (directions.horizontal) {
            SetLeftStickX(3500);
        }
        if (directions.vertical) {
            SetLeftStickY(7000);
        }

        if (directions.diagonal) {
            SetLeftStick(2800, 5800); // 64.23067°

            if (_inputs->b) {
                SetLeftStick(2800, 8500); // 71.76751°
            }

            if (_inputs->r) {
                SetLeftStick(5100, 8200); // 58.1204°
            }

            if (_inputs->c_up) {
                SetLeftStick(5500, 7700); // 54.46232°
            }

            if (_inputs->c_down) {
                SetLeftStick(3400, 8200); // 67.47943°
            }

            if (_inputs->c_left) {
                SetLeftStick(4000, 8400); // 64.53665°
            }

            if (_inputs->c_right) {
                SetLeftStick(6200, 7200); // 49.26789°
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    // We don't apply this for c-up + c-left/c-right in case we want to implement
    // C-stick nair somehow.
    if (directions.cx != 0 && directions.cy < 0) {
        SetLeftStick(3500, 9800); // 70.34618°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (ledgedash_max_jump_traj && _horizontal_socd && !directions.vertical &&
        !shield_button_pressed) {
        SetLeftStickX(10000);
    }

    if (_inputs->lightshield) {
        _outputs->triggerRAnalog = 49;
    }
    if (_inputs->midshield) {
        _outputs->triggerRAnalog = 94;
    }

    // Send lightshield input if we are using Z = lightshield + A macro.
    if (_inputs->z && !(_inputs->mod_x || true_z_press)) {
        _outputs->triggerRAnalog = 49;
    }

    if (_outputs->triggerLDigital) {
        _outputs->triggerLAnalog = 140;
    }

    if (_outputs->triggerRDigital) {
        _outputs->triggerRAnalog = 140;
    }

    // Shut off c-stick when using dpad layer.
    if (_inputs->mod_x && _inputs->mod_y) {
        SetRightStick(0000, 0000);
    }

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
