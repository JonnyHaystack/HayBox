/* Ultimate profile by Taker */
#include "modes/Ultimate.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate::Ultimate(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void Ultimate::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.c_down;                                  //14
    outputs.b = inputs.b;                                       //26
    outputs.x = inputs.x||inputs.y;                             //21
    outputs.y = inputs.y;                                       //
    outputs.triggerLDigital = inputs.l;                         //20
    outputs.triggerRDigital = inputs.r;                         //17

    // Turn on D-Pad layer by holding Nunchuk C button.
    if (inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;                           //12
        outputs.dpadDown = inputs.a;                            //15
        outputs.dpadLeft = inputs.c_left;                       //13
        outputs.dpadRight = inputs.c_right;                     //16
        outputs.leftStickClick = inputs.l;                      
        outputs.rightStickClick = inputs.r;
        outputs.select = inputs.start;                          //10
        outputs.home = inputs.mod_y;                            //11    
    }
    else
    {
        outputs.buttonL = inputs.lightshield;                   //19   
        outputs.buttonR = inputs.z;                             //18
        outputs.start = inputs.start;                           //0
        outputs.select = inputs.select;                         //10
        outputs.home = inputs.home;                             //11
    }
}

void Ultimate::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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

    bool shield_button_pressed = inputs.l || inputs.r;

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
            // MX + q1/2/3/4 = 53 35
            outputs.leftStickX = 128 + (directions.x * 53);
            outputs.leftStickY = 128 + (directions.y * 35);
            if (shield_button_pressed) {
                // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
        }

        // Angled fsmash/ftilt with C-Stick + MX
        if (directions.cx != 0) {
            outputs.rightStickX = 128 + (directions.cx * 72);
            outputs.rightStickY = 128 + (directions.y * 59);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // (33.44) = 53 35
            outputs.leftStickX = 128 + (directions.x * 53);
            outputs.leftStickY = 128 + (directions.y * 35);
            // (39.05) = 53 43
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 43);
            }
            // (36.35) = 53 39
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 39);
            }
            // (30.32) = 56 41
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 31);
            }
            // (27.85) = 49 42
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 28);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // 22.5 degrees
                outputs.leftStickX = 128 + (directions.x * 67);
                outputs.leftStickY = 128 + (directions.y * 28);
                // 37.5 degrees
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 67);
                    outputs.leftStickY = 128 + (directions.y * 51);
                }
                // 30 degrees
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 67);
                    outputs.leftStickY = 128 + (directions.y * 39);
                }
                // 15 degrees
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 67);
                    outputs.leftStickY = 128 + (directions.y * 18);
                }
                // 7.5 degrees
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 67);
                    outputs.leftStickY = 128 + (directions.y * 9);
                }
            }

            // Angled Ftilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 36);
                outputs.leftStickY = 128 + (directions.y * 30);
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
        // MY + Vertical (even if shield is held) = 53
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 53);
            // MY Vertical Tilts
            if (inputs.a) {
                outputs.leftStickY = 128 + (directions.y * 36);
            }
        }
        if (directions.diagonal) {
            // MY + q1/2/3/4 = 35 59
            outputs.leftStickX = 128 + (directions.x * 35);
            outputs.leftStickY = 128 + (directions.y * 53);
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
            // (56.56) = 35 53
            outputs.leftStickX = 128 + (directions.x * 35);
            outputs.leftStickY = 128 + (directions.y * 53);
            // (50.95) = 43 53
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 43);
                outputs.leftStickY = 128 + (directions.y * 53);
            }
            // (53.65) = 39 53
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 49);
                outputs.leftStickY = 128 + (directions.y * 53);
            }
            // (59.68) = 31 53
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 31);
                outputs.leftStickY = 128 + (directions.y * 53);
            }
            // (62.15) = 28 53
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 28);
                outputs.leftStickY = 128 + (directions.y * 53);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // 67.5 degrees
                outputs.leftStickX = 128 + (directions.x * 28);
                outputs.leftStickY = 128 + (directions.y * 67);
                // 52.5 degrees
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 51);
                    outputs.leftStickY = 128 + (directions.y * 67);
                }
                // 60 degrees
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 39);
                    outputs.leftStickY = 128 + (directions.y * 67);
                }
                // 75 degrees
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 18);
                    outputs.leftStickY = 128 + (directions.y * 67);
                }
                // 82.5 degrees
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 9);
                    outputs.leftStickY = 128 + (directions.y * 67);
                }
            }

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
    if (inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
