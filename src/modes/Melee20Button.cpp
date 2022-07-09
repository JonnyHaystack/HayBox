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

void SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value) {
    *axis = 128 + (direction * (uint8_t)(value / 125));
}

void SetLeftStickX(OutputState &outputs, const StickDirections &directions, const uint16_t &value) {
    SetAxis(&outputs.leftStickX, directions.x, value);
}

void SetLeftStickY(OutputState &outputs, const StickDirections &directions, const uint16_t &value) {
    SetAxis(&outputs.leftStickY, directions.y, value);
}

void SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue) {
    SetAxis(xAxis, xDirection, xValue);
    SetAxis(yAxis, yDirection, yValue);
}

void SetLeftStick(OutputState &outputs, const StickDirections &directions, const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&outputs.leftStickX, &outputs.leftStickY, directions.x, directions.y, xValue, yValue);
}

void SetRightStick(OutputState &outputs, const StickDirections &directions, const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&outputs.rightStickX, &outputs.leftStickY, directions.x, directions.y, xValue, yValue);
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
        SetLeftStick(outputs, directions, 7000, 7000); // 45°

        // L, R, LS, and MS + q3/4 = Vanilla shield drop
        if (directions.y < 0 && shield_button_pressed) {
            SetLeftStick(outputs, directions, 7000, 6875); // 44.48384°
        }
    }

    if (inputs.mod_x) {
        // MX + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(outputs, directions, 6625);
        }
        // MX + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(outputs, directions, 5375);
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4
            SetLeftStick(outputs, directions, 7375, 3125); // 22.96377°
            if (shield_button_pressed) {
                // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetStick(&outputs.rightStickX, &outputs.rightStickY, directions.cx, directions.y, 8500, 5250); // 31.70143°
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(outputs, directions, 7375, 3125); // 22.9638°
            if (inputs.c_down) {
                SetLeftStick(outputs, directions, 7000, 3625); // 27.37104°
            }
            if (inputs.c_left) {
                SetLeftStick(outputs, directions, 7875, 4875); // 31.77828°
            }
            if (inputs.c_up) {
                SetLeftStick(outputs, directions, 7000, 5125); // 36.18552°
            }
            if (inputs.c_right) {
                SetLeftStick(outputs, directions, 6125, 5250); // 40.59276°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(outputs, directions, 9125, 3875); // 22.9638°
                if (inputs.c_down) {
                    SetLeftStick(outputs, directions, 8750, 4500); // 27.37104°
                }
                if (inputs.c_left) {
                    SetLeftStick(outputs, directions, 8500, 5250); // 31.77828°
                }
                if (inputs.c_up) {
                    SetLeftStick(outputs, directions, 7375, 5375); // 36.18552°
                }
                if (inputs.c_right) {
                    SetLeftStick(outputs, directions, 6375, 5375); // 40.59276°
                }
            }
        }
    }

    if (inputs.mod_y) {
        // MY + Horizontal (even if shield is held)
        if (directions.horizontal) {
            SetLeftStickX(outputs, directions, 3375);
        }
        // MY + Vertical (even if shield is held)
        if (directions.vertical) {
            SetLeftStickY(outputs, directions, 7375);
        }
        if (directions.diagonal) {
            SetLeftStick(outputs, directions, 3125, 7375); // 67.03623°
            if (shield_button_pressed) {
                SetLeftStick(outputs, directions, 4750, 8750); // 61.50436°
                if (directions.y < 0) {
                    SetLeftStick(outputs, directions, 5000, 8500); // 59.53446°
                }
            }
        }

        // Turnaround neutral B nerf
        if (inputs.b) {
            SetLeftStickX(outputs, directions, 10000);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(outputs, directions, 3125, 7375); // 67.03623°
            if (inputs.c_down) {
                SetLeftStick(outputs, directions, 3625, 7000); // 62.62896°
            }
            if (inputs.c_left) {
                SetLeftStick(outputs, directions, 4875, 7875); // 58.22172°
            }
            if (inputs.c_up) {
                SetLeftStick(outputs, directions, 5125, 7000); // 53.81448°
            }
            if (inputs.c_right) {
                SetLeftStick(outputs, directions, 6375, 7625); // 49.40724°
            }

            /* Extended Up B Angles */
            if (inputs.b) {
                SetLeftStick(outputs, directions, 3875, 9125); // 67.0362°
                if (inputs.c_down) {
                    SetLeftStick(outputs, directions, 4500, 8750); // 62.62896°
                }
                if (inputs.c_left) {
                    SetLeftStick(outputs, directions, 5250, 8500); // 58.22172°
                }
                if (inputs.c_up) {
                    SetLeftStick(outputs, directions, 5875, 8000); // 53.81448°
                }
                if (inputs.c_right) {
                    SetLeftStick(outputs, directions, 5875, 7125); // 49.40724°
                }
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    if (directions.cx != 0 && directions.cy != 0) {
        SetRightStick(outputs, directions, 5250, 8500); // 58.29857°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (horizontal_socd && !directions.vertical) {
        SetLeftStickX(outputs, directions, 10000);
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
        SetRightStick(outputs, directions, 0000, 0000);
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
