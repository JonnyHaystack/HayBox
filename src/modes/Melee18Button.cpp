#include "modes/Melee18Button.hpp"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

Melee18Button::Melee18Button(socd::SocdType socd_type, Melee18ButtonOptions options) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };

    _options = options;
    horizontal_socd = false;
}

void Melee18Button::HandleSocd(InputState &inputs) {
    horizontal_socd = inputs.left && inputs.right;
    InputMode::HandleSocd(inputs);
}

void Melee18Button::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.z;
    if (inputs.nunchuk_connected) {
        // Lightshield with C button.
        if (inputs.nunchuk_c) {
            outputs.triggerLAnalog = 49;
        }
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.l;
    }
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;

    // Activate D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
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

void Melee18Button::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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

    if (directions.diagonal && directions.y == -1 && _options.crouch_walk_os) {
        outputs.leftStickX = 128 + (directions.x * 56);
        outputs.leftStickY = 128 + (directions.y * 55);
    }

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 53);
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 43);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            // 8500 5250 = 68 42
            outputs.rightStickX = 128 + (directions.cx * 68);
            outputs.rightStickY = 128 + (directions.y * 42);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // 22.9638 - 7375 3125 = 59 25
            outputs.leftStickX = 128 + (directions.x * 59);
            outputs.leftStickY = 128 + (directions.y * 25);
            // 27.37104 - 7000 3625 (27.38) = 56 29
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 56);
                outputs.leftStickY = 128 + (directions.y * 29);
            }
            // 31.77828 - 7875 4875 (31.76) = 63 39
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 63);
                outputs.leftStickY = 128 + (directions.y * 39);
            }
            // 36.18552 - 7000 5125 (36.21) = 56 41
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 56);
                outputs.leftStickY = 128 + (directions.y * 41);
            }
            // 40.59276 - 6125 5250 (40.6) = 49 42
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 49);
                outputs.leftStickY = 128 + (directions.y * 42);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // 22.9638 - 9125 3875 (23.0) = 73 31
                outputs.leftStickX = 128 + (directions.x * 73);
                outputs.leftStickY = 128 + (directions.y * 31);
                // 27.37104 - 8750 4500 (27.2) = 70 36
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 70);
                    outputs.leftStickY = 128 + (directions.y * 36);
                }
                // 31.77828 - 8500 5250 (31.7) = 68 42
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 68);
                    outputs.leftStickY = 128 + (directions.y * 42);
                }
                // 36.18552 - 7375 5375 (36.1) = 59 43
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 59);
                    outputs.leftStickY = 128 + (directions.y * 43);
                }
                // 40.59276 - 6375 5375 (40.1) = 51 43
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 51);
                    outputs.leftStickY = 128 + (directions.y * 43);
                }
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 27);
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 59);
        }

        // Turnaround neutral B nerf
        if (inputs.b) {
            outputs.leftStickX = 128 + (directions.x * 80);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            // 67.0362 - 3125 7375 = 25 59
            outputs.leftStickX = 128 + (directions.x * 25);
            outputs.leftStickY = 128 + (directions.y * 59);
            // 62.62896 - 3625 7000 (62.62) = 29 56
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 29);
                outputs.leftStickY = 128 + (directions.y * 56);
            }
            // 58.22172 - 4875 7875 (58.24) = 39 63
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 39);
                outputs.leftStickY = 128 + (directions.y * 63);
            }
            // 53.81448 - 5125 7000 (53.79) = 41 56
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 41);
                outputs.leftStickY = 128 + (directions.y * 56);
            }
            // 49.40724 - 6375 7625 (50.10) = 51 61
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 61);
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                // 67.0362 - 3875 9125 = 31 73
                outputs.leftStickX = 128 + (directions.x * 31);
                outputs.leftStickY = 128 + (directions.y * 73);
                // 62.62896 - 4500 8750 (62.8) = 36 70
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 36);
                    outputs.leftStickY = 128 + (directions.y * 70);
                }
                // 58.22172 - 5250 8500 (58.3) = 42 68
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 42);
                    outputs.leftStickY = 128 + (directions.y * 68);
                }
                // 53.81448 - 5875 8000 (53.7) = 47 64
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 47);
                    outputs.leftStickY = 128 + (directions.y * 64);
                }
                // 49.40724 - 5875 7125 (50.49) = 47 57
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 47);
                    outputs.leftStickY = 128 + (directions.y * 57);
                }
            }
        }
    }

    if (inputs.l) {
        // L overrides modifiers, both for wavedash nerf and so MX/MY can give midshield/lightshield
        // without forcing shield tilt.
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 80);
            if (directions.y == 1) {
                outputs.leftStickX = 128 + (directions.x * 43);
                outputs.leftStickY = 128 + 43;
            }
            if (directions.y == -1) {
                outputs.leftStickX = 128 + (directions.x * 57);
                outputs.leftStickY = 128 - 55;
            }
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 80);
        }

        // L + Mod X = midshield
        if (inputs.mod_x) {
            outputs.triggerLDigital = false;
            outputs.triggerRAnalog = 94;

            if (directions.diagonal) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
        }
        // L + Mod Y = lightshield
        if (inputs.mod_y) {
            outputs.triggerLDigital = false;
            outputs.triggerRAnalog = 49;

            if (directions.diagonal) {
                outputs.leftStickX = 128 + (directions.x * 40);
                outputs.leftStickY = 128 + (directions.y * 68);
            }
        }
    }

    // Holding R gives special shield tilt and wavedash coordinates.
    if (inputs.r) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 51);
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 43);
        }
        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 43);
            if (inputs.mod_x) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
            if (inputs.mod_y) {
                outputs.leftStickX = 128 + (directions.x * 40);
                outputs.leftStickY = 128 + (directions.y * 68);
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

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (!inputs.r && horizontal_socd && !directions.vertical) {
        outputs.leftStickX = 128 + (directions.x * 80);
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
