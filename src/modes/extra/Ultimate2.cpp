/* Ultimate2 profile by Taker */
#include "modes/extra/Ultimate2.hpp"

Ultimate2::Ultimate2(socd::SocdType socd_type) : ControllerMode(socd_type, 100) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void Ultimate2::UpdateDigitalOutputs(InputState &inputs) {
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

void Ultimate2::UpdateAnalogOutputs(InputState &inputs) {
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
            // Vertical Shield Tilt
            if (shield_button_pressed) {
                SetLeftStickX(5100);
            }
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4
            SetLeftStick(5300, 4000); // 37.04247°
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
            SetLeftStick(5300, 4000); // 37.04247°
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
            SetLeftStickY(4400);
            // MY Vertical Tilts
            if (inputs.a) {
                SetLeftStickY(3600);
            }
        }
        if (directions.diagonal) {
            // MY + q1/2/3/4
            SetLeftStick(4100, 4400); // 47.02136°
            if (shield_button_pressed) {
                // MY + L, R, LS, and MS + q1/2
                SetLeftStick(3800, 7000); // 61.50436°
                // MY + L, R, LS, and MS + q3/4 = 40 68
                if (directions.x < 0) {
                    SetLeftStick(4000, 6800); // 59.53446°
                }
            }
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(4100, 4400); // 47.02136°

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
        SetRightStick(0000, 0000); // 0°
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs->leftStickX = inputs.nunchuk_x;
        outputs->leftStickY = inputs.nunchuk_y;
    }
}