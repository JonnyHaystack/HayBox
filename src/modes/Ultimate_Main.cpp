/* Ultimate profile by Taker */
#include "modes/Ultimate_Main.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate_Main::Ultimate_Main(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}
//////////////////////////////////////////////////////////////////////////
//if(inputs.c_up){set_mode(backend, new Ultimate_Main(socd::SOCD_NEUTRAL))      inputs.mod_x && inputs.start && c_up
//////////////////////////////////////////////////////////////////////////

void Ultimate_Main::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;           //Code was inputs.a
    outputs.b = inputs.b;
    outputs.x = inputs.x             //inputs.x||inputs.y this code will make a short hop macro if both buttons are set to jump
    outputs.y = inputs.y;
    outputs.triggerLDigital = inputs.l;
    outputs.triggerRDigital = inputs.r;

    // Turn on D-Pad layer by holding Nunchuk C button.
    if (inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
        outputs.leftStickClick = inputs.lightshield;
        outputs.rightStickClick = inputs.z;
        //changed from outputs.select = inputs.start; to nothing. Supposed to be on line 37
        //changed from outputs.home = inputs.home; to nothing. Supposed to be line 38
    }
    else
    {
        outputs.buttonL = inputs.lightshield;
        outputs.buttonR = inputs.z;
        outputs.start = inputs.start;
        outputs.select = inputs.select;
        outputs.home = inputs.home;
    }
    if (inputs.mod_x) {                                 /////////////      Testing to see if it works 
        outputs.x = inputs.x||inputs.y                  /////////////               
    }                                                   /////////////   
        
    else                                                /////////////      Should make it so that mod_x in the config.cpp file modifys x to
    {                                                   /////////////      input both x and y at the same time crating a short hop
        outputs.x = inputs.x                            /////////////
    }                                                   /////////////
    if (inputs.mod_x && inputs.mod_y) {
        outputs.home = inputs.home;
    }
}

void Ultimate_Main::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.mod_x,           //Code was inputs.up
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

    if (inputs.up) {                     //I think this is where I have to change inputs.mod_x to inputs.up to make mod_x input up
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
