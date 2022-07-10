/* Ultimate profile by Taker */
#include "modes/Ultimate.hpp"

Ultimate::Ultimate(socd::SocdType socd_type) : ControllerMode(socd_type, 100) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void Ultimate::UpdateDigitalOutputs(InputState &inputs) {
    outputs->a = inputs.a;
    outputs->b = inputs.b;
    outputs->x = inputs.x;
    outputs->y = inputs.y;
    outputs->buttonR = inputs.z;
    outputs->triggerLDigital = inputs.l;
    outputs->triggerRDigital = inputs.r;
    outputs->start = inputs.start;

    // D-Pad
    if (inputs.mod_x && inputs.mod_y) {
        outputs->dpadUp = inputs.c_up;
        outputs->dpadDown = inputs.c_down;
        outputs->dpadLeft = inputs.c_left;
        outputs->dpadRight = inputs.c_right;
    }

    if (inputs.select)
        outputs->dpadLeft = true;
    if (inputs.home)
        outputs->dpadRight = true;
}

void Ultimate::UpdateAnalogOutputs(InputState &inputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up
    );

    bool shield_button_pressed = inputs.l || inputs.r || inputs.lightshield || inputs.midshield;

    if (inputs.mod_x) {
        // MX + Horizontal
        if (directions.horizontal) {
            SetLeftStickX(5300);
            // Horizontal Shield tilt
            if (shield_button_pressed) {
                SetLeftStickX(5100);
            }
            // Horizontal Tilts
            if (inputs.a) {
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
            SetStick(&outputs->rightStickX, &outputs->rightStickY, directions.cx, directions.y, 12700, 5900); // 24.91802°
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(5300, 3500); // 33.43987°
            if (inputs.c_down) {
                SetLeftStick(5300, 4300); // 39.05314°
            }
            if (inputs.c_left) {
                SetLeftStick(5300, 3900); // 36.34746°
            }
            if (inputs.c_up) {
                SetLeftStick(5300, 3100); // 30.32361°
            }
            if (inputs.c_right) {
                SetLeftStick(5300, 2800); // 27.84758°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(6700, 4400); // 33.29356°
                if (inputs.c_down) {
                    SetLeftStick(6700, 5500); // 39.38242°
                }
                if (inputs.c_left) {
                    SetLeftStick(6700, 4900); // 36.17962°
                }
                if (inputs.c_up) {
                    SetLeftStick(6700, 3900); // 30.20324°
                }
                if (inputs.c_right) {
                    SetLeftStick(6700, 3500); // 27.58203°
                }
            }

            // Angled Ftilts
            if (inputs.a) {
                SetLeftStick(3600, 2600); // 35.83765°
            }
        }
    }

    if (inputs.mod_y) {
        // MY + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(4100);
            // MY Horizontal Tilts
            if (inputs.a) {
                SetLeftStickX(3600);
            }
        }
        // MY + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(5300);
            // MY Vertical Tilts
            if (inputs.a) {
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
            if (inputs.c_down) {
                SetLeftStick(4300, 5300); // 50.94686°
            }
            if (inputs.c_left) {
                SetLeftStick(3900, 5300); // 53.65254°
            }
            if (inputs.c_up) {
                SetLeftStick(3100, 5300); // 59.67639°
            }
            if (inputs.c_right) {
                SetLeftStick(2800, 5300); // 62.15242°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(4400, 6700); // 56.70644°
                if (inputs.c_down) {
                    SetLeftStick(5500, 6700); // 50.61758°
                }
                if (inputs.c_left) {
                    SetLeftStick(4900, 6700); // 53.82038°
                }
                if (inputs.c_up) {
                    SetLeftStick(3900, 6700); // 59.79676°
                }
                if (inputs.c_right) {
                    SetLeftStick(3500, 6700); // 62.41797°
                }
            }

            // MY Pivot Uptilt/Dtilt
            if (inputs.a) {
                SetLeftStick(3400, 3800); // 48.17983°
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(4200, 6800); // 58.29857°
    }

    if (inputs.l) {
        outputs->triggerLAnalog = 140;
    }

    if (inputs.r) {
        outputs->triggerRAnalog = 140;
    }

    // Shut off c-stick when using dpad layer.
    if (inputs.mod_x && inputs.mod_y) {
        SetRightStick(0000, 0000);
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs->leftStickX = inputs.nunchuk_x;
        outputs->leftStickY = inputs.nunchuk_y;
    }
}