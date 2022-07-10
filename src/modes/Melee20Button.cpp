#include "modes/Melee20Button.hpp"

Melee20Button::Melee20Button(socd::SocdType socd_type) : ControllerMode(socd_type, 80) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    horizontal_socd = false;
}

void Melee20Button::HandleSocd() {
    horizontal_socd = _inputs->left && _inputs->right;
    InputMode::HandleSocd();
}

void Melee20Button::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a;
    _outputs->b = _inputs->b;
    _outputs->x = _inputs->x;
    _outputs->y = _inputs->y;
    _outputs->buttonR = _inputs->z;
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

    if (_inputs->select)
        _outputs->dpadLeft = true;
    if (_inputs->home)
        _outputs->dpadRight = true;
}

void Melee20Button::UpdateAnalogOutputs() {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        _inputs->left,
        _inputs->right,
        _inputs->down,
        _inputs->up,
        _inputs->c_left,
        _inputs->c_right,
        _inputs->c_down,
        _inputs->c_up
    );

    bool shield_button_pressed = _inputs->l || _inputs->r || _inputs->lightshield || _inputs->midshield;
    if (directions.diagonal) {
        // L, R, LS, and MS + q1/2
        SetLeftStick(7000, 7000); // 45°

        // L, R, LS, and MS + q3/4 = Vanilla shield drop
        if (directions.y < 0 && shield_button_pressed) {
            SetLeftStick(7000, 6875); // 44.48384°
        }
    }

    if (_inputs->mod_x) {
        // MX + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(6625);
        }
        // MX + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(5375);
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4
            SetLeftStick(7375, 3125); // 22.96377°
            if (shield_button_pressed) {
                SetLeftStick(6375, 3750); // 30.46554°
            }
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetAngledFSmash(8500, 5250); // 31.70143°
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(7375, 3125); // 22.9638°
            if (_inputs->c_down) {
                SetLeftStick(7000, 3625); // 27.37104°
            }
            if (_inputs->c_left) {
                SetLeftStick(7875, 4875); // 31.77828°
            }
            if (_inputs->c_up) {
                SetLeftStick(7000, 5125); // 36.18552°
            }
            if (_inputs->c_right) {
                SetLeftStick(6125, 5250); // 40.59276°
            }

            /* Extended Up B Angles */
            if (_inputs->b) {
                SetLeftStick(9125, 3875); // 22.9638°
                if (_inputs->c_down) {
                    SetLeftStick(8750, 4500); // 27.37104°
                }
                if (_inputs->c_left) {
                    SetLeftStick(8500, 5250); // 31.77828°
                }
                if (_inputs->c_up) {
                    SetLeftStick(7375, 5375); // 36.18552°
                }
                if (_inputs->c_right) {
                    SetLeftStick(6375, 5375); // 40.59276°
                }
            }
        }
    }

    if (_inputs->mod_y) {
        // MY + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(3375);
        }
        // MY + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(7375);
        }
        if (directions.diagonal) {
            SetLeftStick(3125, 7375); // 67.03623°
            if (shield_button_pressed) {
                SetLeftStick(4750, 8750); // 61.50436°
                if (directions.y < 0) {
                    SetLeftStick(5000, 8500); // 59.53446°
                }
            }
        }

        // Turnaround neutral B nerf
        if (_inputs->b) {
            SetLeftStickX(10000);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(3125, 7375); // 67.03623°
            if (_inputs->c_down) {
                SetLeftStick(3625, 7000); // 62.62896°
            }
            if (_inputs->c_left) {
                SetLeftStick(4875, 7875); // 58.22172°
            }
            if (_inputs->c_up) {
                SetLeftStick(5125, 7000); // 53.81448°
            }
            if (_inputs->c_right) {
                SetLeftStick(6375, 7625); // 49.40724°
            }

            /* Extended Up B Angles */
            if (_inputs->b) {
                SetLeftStick(3875, 9125); // 67.0362°
                if (_inputs->c_down) {
                    SetLeftStick(4500, 8750); // 62.62896°
                }
                if (_inputs->c_left) {
                    SetLeftStick(5250, 8500); // 58.22172°
                }
                if (_inputs->c_up) {
                    SetLeftStick(5875, 8000); // 53.81448°
                }
                if (_inputs->c_right) {
                    SetLeftStick(5875, 7125); // 49.40724°
                }
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(5250, 8500); // 58.29857°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (horizontal_socd && !directions.vertical) {
        SetLeftStickX(10000);
    }

    if (_inputs->lightshield) {
        _outputs->triggerRAnalog = 49;
    }
    if (_inputs->midshield) {
        _outputs->triggerRAnalog = 94;
    }

    if (_outputs->triggerLDigital) {
        _outputs->triggerLAnalog = 140;
    }

    if (_outputs->triggerRDigital) {
        _outputs->triggerRAnalog = 140;
    }

    // Shut off c-stick when using dpad layer.
    if (_inputs->mod_x && _inputs->mod_y) {
        SetRightStick(0000, 0000); // 0°
    }

    // Nunchuk overrides left stick.
    if (_inputs->nunchuk_connected) {
        _outputs->leftStickX = _inputs->nunchuk_x;
        _outputs->leftStickY = _inputs->nunchuk_y;
    }
}
