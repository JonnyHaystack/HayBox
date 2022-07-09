#include "modes/Melee20Button.hpp"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

Melee20Button::Melee20Button(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    horizontal_socd = false;
}

void Melee20Button::HandleSocd(InputState &inputs) {
    horizontal_socd = inputs.left && inputs.right;
    InputMode::HandleSocd(inputs);
}

void Melee20Button::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.z;
    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.l;
    }
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;

    // D-Pad layer can be activated by holding Mod X + Mod Y, or by holding the C
    // button on a nunchuk.
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

void Melee20Button::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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
    if (directions.diagonal) {
        // L, R, LS, and MS + q1/2
        SetLeftStick(outputs, 7000, 7000); // 45°

        // L, R, LS, and MS + q3/4 = Vanilla shield drop
        if (directions.y < 0 && shield_button_pressed) {
            SetLeftStick(outputs, 7000, 6875); // 44.48384°
        }
    }

    if (inputs.mod_x) {
        // MX + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(outputs, 6625);
        }
        // MX + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(outputs, 5375);
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4
            SetLeftStick(outputs, 7375, 3125); // 22.96377°
            if (shield_button_pressed) {
                SetLeftStick(outputs, 6375, 3750); // 30.46554°
            }
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
        // MY + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(outputs, 3375);
        }
        // MY + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(outputs, 7375);
        }
        if (directions.diagonal) {
            SetLeftStick(outputs, 3125, 7375); // 67.03623°
            if (shield_button_pressed) {
                SetLeftStick(outputs, 4750, 8750); // 61.50436°
                if (directions.y < 0) {
                    SetLeftStick(outputs, 5000, 8500); // 59.53446°
                }
            }
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

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(outputs, 5250, 8500); // 58.29857°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (horizontal_socd && !directions.vertical) {
        SetLeftStickX(outputs, 10000);
    }

    if (inputs.lightshield) {
        outputs.triggerRAnalog = 49;
    }
    if (inputs.midshield) {
        outputs.triggerRAnalog = 94;
    }

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 140;
    }

    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 140;
    }

    // Shut off c-stick when using dpad layer.
    if (inputs.mod_x && inputs.mod_y) {
        SetRightStick(outputs, 0000, 0000);
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
