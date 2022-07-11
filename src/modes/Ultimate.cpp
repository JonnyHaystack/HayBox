/* Ultimate profile by Taker */
#include "modes/Ultimate.hpp"

Ultimate::Ultimate(socd::SocdType socd_type)
    : PlatformFighter(socd_type, 100) { }

void Ultimate::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a;
    _outputs->b = _inputs->b;
    _outputs->x = _inputs->x;
    _outputs->y = _inputs->y;
    _outputs->buttonR = _inputs->z;
    _outputs->triggerLDigital = _inputs->l;
    _outputs->triggerRDigital = _inputs->r;
    _outputs->start = _inputs->start;

    // D-Pad
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

void Ultimate::UpdateAnalogOutputs() {
    UpdateDirections();

    bool shield_button_pressed = _inputs->l || _inputs->r || _inputs->lightshield || _inputs->midshield;

    if (_inputs->mod_x) {
        // MX + Horizontal
        if (directions.horizontal) {
            SetLeftStickX(5300);
            // Horizontal Shield tilt
            if (shield_button_pressed) {
                SetLeftStickX(5100);
            }
            // Horizontal Tilts
            if (_inputs->a) {
                SetLeftStickX(3600);
            }
        }
        // MX + Vertical
        if (directions.vertical) {
            SetLeftStickY(4400);
            if (shield_button_pressed) {
                SetLeftStickY(5100);
            }
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4
            SetLeftStick(5300, 3500); // 33.43987°
            if (shield_button_pressed) {
                // MX + L, R, LS, and MS + q1/2/3/4
                SetLeftStick(5100, 3000); // 30.46554°
            }
        }

        // Angled fsmash/ftilt with C-Stick + MX
        if (directions.cx != 0) {
            SetAngledFSmash(12700, 5900); // 24.91802°
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(5300, 3500); // 33.43987°
            if (_inputs->c_down) {
                SetLeftStick(5300, 4300); // 39.05314°
            }
            if (_inputs->c_left) {
                SetLeftStick(5300, 3900); // 36.34746°
            }
            if (_inputs->c_up) {
                SetLeftStick(5300, 3100); // 30.32361°
            }
            if (_inputs->c_right) {
                SetLeftStick(5300, 2800); // 27.84758°
            }

            /* Extended Up B Angles */
            if (_inputs->b) {
                SetLeftStick(6700, 4400); // 33.29356°
                if (_inputs->c_down) {
                    SetLeftStick(6700, 5500); // 39.38242°
                }
                if (_inputs->c_left) {
                    SetLeftStick(6700, 4900); // 36.17962°
                }
                if (_inputs->c_up) {
                    SetLeftStick(6700, 3900); // 30.20324°
                }
                if (_inputs->c_right) {
                    SetLeftStick(6700, 3500); // 27.58203°
                }
            }

            // Angled Ftilts
            if (_inputs->a) {
                SetLeftStick(3600, 2600); // 35.83765°
            }
        }
    }

    if (_inputs->mod_y) {
        // MY + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(4100);
            // MY Horizontal Tilts
            if (_inputs->a) {
                SetLeftStickX(3600);
            }
        }
        // MY + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(5300);
            // MY Vertical Tilts
            if (_inputs->a) {
                SetLeftStickY(3600);
            }
        }
        if (directions.diagonal) {
            // MY + q1/2/3/4
            SetLeftStick(3500, 5300); // 56.56013°
            if (shield_button_pressed) {
                // MY + L, R, LS, and MS + q1/2
                SetLeftStick(3800, 7000); // 61.50436°
                // MY + L, R, LS, and MS + q3/4
                if (directions.x < 0) {
                    SetLeftStick(4000, 6800); // 59.53446°
                }
            }
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(3500, 5300); // 56.56013°
            if (_inputs->c_down) {
                SetLeftStick(4300, 5300); // 50.94686°
            }
            if (_inputs->c_left) {
                SetLeftStick(3900, 5300); // 53.65254°
            }
            if (_inputs->c_up) {
                SetLeftStick(3100, 5300); // 59.67639°
            }
            if (_inputs->c_right) {
                SetLeftStick(2800, 5300); // 62.15242°
            }

            /* Extended Up B Angles */
            if (_inputs->b) {
                SetLeftStick(4400, 6700); // 56.70644°
                if (_inputs->c_down) {
                    SetLeftStick(5500, 6700); // 50.61758°
                }
                if (_inputs->c_left) {
                    SetLeftStick(4900, 6700); // 53.82038°
                }
                if (_inputs->c_up) {
                    SetLeftStick(3900, 6700); // 59.79676°
                }
                if (_inputs->c_right) {
                    SetLeftStick(3500, 6700); // 62.41797°
                }
            }

            // MY Pivot Uptilt/Dtilt
            if (_inputs->a) {
                SetLeftStick(3400, 3800); // 48.17983°
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(4200, 6800); // 58.29857°
    }

    if (_inputs->l) {
        _outputs->triggerLAnalog = 140;
    }

    if (_inputs->r) {
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