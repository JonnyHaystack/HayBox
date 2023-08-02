/* Ultimate2 profile by Taker */
#include "modes/extra/Ultimate2.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate2::Ultimate2(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void Ultimate2::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.z;
    outputs.triggerLDigital = inputs.l;
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;

    // Turn on D-Pad layer by holding Mod X + Mod Y, or Nunchuk C button.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }

    if (inputs.select)
        outputs.dpadLeft = true;
    if (inputs.home)
        outputs.dpadRight = true;
}

void Ultimate2::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.r || inputs.lightshield || inputs.midshield;

    if (inputs.mod_x) {
        // MX + Horizontal = 6625 = 53
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 53);
            // Horizontal Shield tilt = 51
            if (shield_button_pressed) {
                outputs.leftStickX = 128 + (directions.x * 51);
            }
            // Horizontal Tilts = 36
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 36);
            }
        }
        // MX + Vertical = 44
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 44);
            // Vertical Shield Tilt = 51
            if (shield_button_pressed) {
                outputs.leftStickY = 128 + (directions.y * 51);
            }
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4 = 53 40
            outputs.leftStickX = 128 + (directions.x * 53);
            outputs.leftStickY = 128 + (directions.y * 40);
            if (shield_button_pressed) {
                // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
        }

        // Angled fsmash/ftilt with C-Stick + MX
        if (directions.cx != 0) {
            outputs.rightStickX = 128 + (directions.cx * 127);
            outputs.rightStickY = 128 + (directions.y * 59);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // (33.44) = 53 40
            outputs.leftStickX = 128 + (directions.x * 53);
            outputs.leftStickY = 128 + (directions.y * 40);

            // Angled Ftilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 36);
                outputs.leftStickY = 128 + (directions.y * 26);
            }
        }
    }

    if (inputs.mod_y) {
        // MY + Horizontal (even if shield is held) = 41
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 41);
            // MY Horizontal Tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 36);
            }
        }
        // MY + Vertical (even if shield is held) = 44
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 44);
            // MY Vertical Tilts
            if (inputs.a) {
                outputs.leftStickY = 128 + (directions.y * 36);
            }
        }
        if (directions.diagonal) {
            // MY + q1/2/3/4 = 41 44
            outputs.leftStickX = 128 + (directions.x * 41);
            outputs.leftStickY = 128 + (directions.y * 44);
            if (shield_button_pressed) {
                // MY + L, R, LS, and MS + q1/2 = 38 70
                outputs.leftStickX = 128 + (directions.x * 38);
                outputs.leftStickY = 128 + (directions.y * 70);
                // MY + L, R, LS, and MS + q3/4 = 40 68
                if (directions.x == -1) {
                    outputs.leftStickX = 128 + (directions.x * 40);
                    outputs.leftStickY = 128 + (directions.y * 68);
                }
            }
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // (56.56) = 41 44
            outputs.leftStickX = 128 + (directions.x * 41);
            outputs.leftStickY = 128 + (directions.y * 44);

            // MY Pivot Uptilt/Dtilt
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 34);
                outputs.leftStickY = 128 + (directions.y * 38);
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        // 5250 8500 = 42 68
        outputs.rightStickX = 128 + (directions.cx * 42);
        outputs.rightStickY = 128 + (directions.cy * 68);
    }

    if (inputs.l) {
        outputs.triggerLAnalog = 140;
    }

    if (inputs.r) {
        outputs.triggerRAnalog = 140;
    }

    // Shut off C-stick when using D-Pad layer.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}