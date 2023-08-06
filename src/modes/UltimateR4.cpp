#include "modes/UltimateR4.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

UltimateR4::UltimateR4(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,    &InputState::right },
        socd::SocdPair{ &InputState::down,   &InputState::up },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right },
        socd::SocdPair{ &InputState::c_down, &InputState::c_up },
    };
}

void UltimateR4::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonL = inputs.midshield;
    outputs.buttonR = inputs.z;
    outputs.triggerLDigital = inputs.l;
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;
    outputs.select = inputs.select;
    outputs.home = inputs.home;

    // Turn on D-Pad layer by holding Mod X + Mod Y
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void UltimateR4::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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
        if (directions.horizontal) {
          if (shield_button_pressed) {
            // MX + Horizontal = 51 for shield tilt
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 51);
          } else {
            // Fastest walking speed before run
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
          }
        } else if (directions.vertical) {
            // Vertical Shield Tilt and crouch with mod_x = 65
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 65);
        } else if (directions.diagonal) {
            // MX + q1/2/3/4 = 53 35
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 34);
            if (shield_button_pressed) {
                // TODO: If holding shield, then side, THEN down, it will spot dodge.
                // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 51);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 30);
            }
        }

        // Angled fsmash/ftilt with C-Stick + MX
        if (directions.cx != 0) {
            outputs.rightStickX = ANALOG_STICK_NEUTRAL + (directions.cx * 100);
            outputs.rightStickY = ANALOG_STICK_NEUTRAL + (directions.y * 59);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // (39.05) = 53 43
            if (inputs.c_down) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 43);
            }
            // (36.35) = 53 39
            if (inputs.c_left) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 39);
            }
            // (30.32) = 56 41
            if (inputs.c_up) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 31);
            }
            // (27.85) = 49 42
            if (inputs.c_right) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 28);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // (33.29) = 67 44
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 67);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 44);
                // (39.38) = 67 55
                if (inputs.c_down) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 67);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 55);
                }
                // (36.18) = 67 49
                if (inputs.c_left) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 67);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 49);
                }
                // (30.2) = 67 39
                if (inputs.c_up) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 67);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 39);
                }
                // (27.58) = 67 35
                if (inputs.c_right) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 67);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 35);
                }
            }

            // Angled Ftilts
            if (inputs.a) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 36);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 26);
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
          if (shield_button_pressed) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 51);
          } else {
            // Allow tink/yink walk shield
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 28);
          }
        } else if (directions.vertical) {
            // Vertical Shield Tilt and crouch with mod_x = 50
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 50);
        } else if (directions.diagonal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 53);
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 34);
            if (shield_button_pressed) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 51);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 30);
            }
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // (50.95) = 43 53
            if (inputs.c_down) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 43);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 53);
            }
            // (53.65) = 39 53
            if (inputs.c_left) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 49);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 53);
            }
            // (59.68) = 31 53
            if (inputs.c_up) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 31);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 53);
            }
            // (62.15) = 28 53
            if (inputs.c_right) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 28);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 53);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // (56.71) = 44 67
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 44);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 67);
                // (50.62) = 55 67
                if (inputs.c_down) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 55);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 67);
                }
                // (53.82) = 49 67
                if (inputs.c_left) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 49);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 67);
                }
                // (59.8) = 39 67
                if (inputs.c_up) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 39);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 67);
                }
                // (62.42) = 35 67
                if (inputs.c_right) {
                    outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 35);
                    outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 67);
                }
            }

            // MY Pivot Uptilt/Dtilt
            if (inputs.a) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 50);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 65);
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        // 5250 8500 = 42 68
        outputs.rightStickX = ANALOG_STICK_NEUTRAL + (directions.cx * 42);
        outputs.rightStickY = ANALOG_STICK_NEUTRAL + (directions.cy * 68);
    }

    if (inputs.l) {
        outputs.triggerLAnalog = 140;
    }

    if (inputs.r) {
        outputs.triggerRAnalog = 140;
    }

    // Shut off C-stick when using D-Pad layer.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
      
        if (inputs.lightshield) {
            outputs.select = true;
        }

        if (inputs.midshield) {
            outputs.home = true;
        }

        if (inputs.start) {
            outputs.home = true;
            outputs.start = false;
        }
    }

}

