#include "modes/Melee18Button.hpp"

Melee18Button::Melee18Button(socd::SocdType socd_type) : ControllerMode(socd_type, 80) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    horizontal_socd = false;
}

void Melee18Button::HandleSocd() {
    horizontal_socd = _inputs->left && _inputs->right;
    InputMode::HandleSocd();
}

void Melee18Button::UpdateDigitalOutputs() {
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

    if (_inputs->select)
        _outputs->dpadLeft = true;
    if (_inputs->home)
        _outputs->dpadRight = true;
}

void Melee18Button::UpdateAnalogOutputs() {
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

    bool shield_button_pressed = _inputs->l || _inputs->r;

    if (_inputs->mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(6625);
        }
        if (directions.vertical) {
            SetLeftStickY(2875);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetStick(&_outputs->rightStickX, &_outputs->rightStickY, directions.cx, directions.y, 8500, 5250); // 31.70143°
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
        if (directions.horizontal) {
            SetLeftStickX(3375);
        }
        if (directions.vertical) {
            SetLeftStickY(7375);
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

    if (_inputs->l) {
        if (directions.horizontal)
            SetLeftStickX(10000);
        if (directions.vertical)
            SetLeftStickY(10000);
        if (directions.horizontal && directions.y > 0) {
            SetLeftStick(5375, 5375); // 45°
        }
        if (directions.horizontal && directions.y < 0) {
            SetLeftStick(7125, 6875); // 43.97697°
        }
        if (_inputs->mod_x || _inputs->mod_y) {
            if (!(_inputs->mod_x && _inputs->mod_y)) {
                _outputs->triggerLDigital = false;
                _outputs->triggerRAnalog = 49;
            }

            if (directions.diagonal) {
                if (_inputs->mod_x) {
                    SetLeftStick(6375, 3750); // 30.46554°
                }
                if (_inputs->mod_y) {
                    SetLeftStick(5000, 8500); // 59.53446°
                }
            }
        }
    }

    if (_inputs->r) {
        if (directions.horizontal) {
            SetLeftStickX(6375);
        }
        if (directions.vertical) {
            SetLeftStickY(5375);
        }
        if (directions.diagonal) {
            SetLeftStickX(5375);
            if (_inputs->mod_x) {
                SetLeftStick(6375, 3750); // 30.46554°
            }
            if (_inputs->mod_y) {
                SetLeftStick(5000, 8500); // 59.53446°
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
    if (!_inputs->r && horizontal_socd && !directions.vertical) {
        SetLeftStickX(10000);
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
