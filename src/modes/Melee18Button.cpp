#include "modes/Melee18Button.hpp"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_MAX 208

Melee18Button::Melee18Button(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

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

    /********* DPAD *********/
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

    if (inputs.mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(outputs, 6625);
        }
        if (directions.vertical) {
            SetLeftStickY(outputs, 2875);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetStick(&outputs.rightStickX, &outputs.rightStickY, directions.cx, directions.y, 8500, 5250); // 31.70143°
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(outputs, 7375, 3125); // 22.9638°
            if (inputs.c_down) {
                SetLeftStick(outputs, 7000, 3625); // 27.37104°
            }
            if (inputs.c_left) {
                SetLeftStick(outputs, 7875, 4875); // 31.77828°
            }
            if (inputs.c_up) {
                SetLeftStick(outputs, 7000, 5125); // 36.18552°
            }
            if (inputs.c_right) {
                SetLeftStick(outputs, 6125, 5250); // 40.59276°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(outputs, 9125, 3875); // 22.9638°
                if (inputs.c_down) {
                    SetLeftStick(outputs, 8750, 4500); // 27.37104°
                }
                if (inputs.c_left) {
                    SetLeftStick(outputs, 8500, 5250); // 31.77828°
                }
                if (inputs.c_up) {
                    SetLeftStick(outputs, 7375, 5375); // 36.18552°
                }
                if (inputs.c_right) {
                    SetLeftStick(outputs, 6375, 5375); // 40.59276°
                }
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            SetLeftStickX(outputs, 3375);
        }
        if (directions.vertical) {
            SetLeftStickY(outputs, 7375);
        }

        // Turnaround neutral B nerf
        if (inputs.b) {
            SetLeftStickX(outputs, 10000);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(outputs, 3125, 7375); // 67.03623°
            if (inputs.c_down) {
                SetLeftStick(outputs, 3625, 7000); // 62.62896°
            }
            if (inputs.c_left) {
                SetLeftStick(outputs, 4875, 7875); // 58.22172°
            }
            if (inputs.c_up) {
                SetLeftStick(outputs, 5125, 7000); // 53.81448°
            }
            if (inputs.c_right) {
                SetLeftStick(outputs, 6375, 7625); // 49.40724°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(outputs, 3875, 9125); // 67.0362°
                if (inputs.c_down) {
                    SetLeftStick(outputs, 4500, 8750); // 62.62896°
                }
                if (inputs.c_left) {
                    SetLeftStick(outputs, 5250, 8500); // 58.22172°
                }
                if (inputs.c_up) {
                    SetLeftStick(outputs, 5875, 8000); // 53.81448°
                }
                if (inputs.c_right) {
                    SetLeftStick(outputs, 5875, 7125); // 49.40724°
                }
            }
        }
    }

    if (inputs.l) {
        if (directions.horizontal)
            SetLeftStickX(outputs, 10000);
        if (directions.vertical)
            SetLeftStickY(outputs, 10000);
        if (directions.horizontal && directions.y > 0) {
            SetLeftStick(outputs, 5375, 5375); // 45°
        }
        if (directions.horizontal && directions.y < 0) {
            SetLeftStick(outputs, 7125, 6875); // 43.97697°
        }
        if (inputs.mod_x || inputs.mod_y) {
            if (!(inputs.mod_x && inputs.mod_y)) {
                outputs.triggerLDigital = false;
                outputs.triggerRAnalog = 49;
            }

            if (directions.diagonal) {
                if (inputs.mod_x) {
                    SetLeftStick(outputs, 6375, 3750); // 30.46554°
                }
                if (inputs.mod_y) {
                    SetLeftStick(outputs, 5000, 8500); // 59.53446°
                }
            }
        }
    }

    if (inputs.r) {
        if (directions.horizontal) {
            SetLeftStickX(outputs, 6375);
        }
        if (directions.vertical) {
            SetLeftStickY(outputs, 5375);
        }
        if (directions.diagonal) {
            SetLeftStickX(outputs, 5375);
            if (inputs.mod_x) {
                SetLeftStick(outputs, 6375, 3750); // 30.46554°
            }
            if (inputs.mod_y) {
                SetLeftStick(outputs, 5000, 8500); // 59.53446°
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(outputs, 5250, 8500); // 58.29857°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (!inputs.r && horizontal_socd && !directions.vertical) {
        SetLeftStickX(outputs, 10000);
    }

    // Shut off c-stick when using dpad layer.
    if (inputs.mod_x && inputs.mod_y) {
        SetRightStick(outputs, 0000, 0000); // 0°
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
