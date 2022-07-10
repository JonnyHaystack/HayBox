#include "modes/ProjectM.hpp"

ProjectM::ProjectM(socd::SocdType socd_type, bool ledgedash_max_jump_traj, bool true_z_press)
    : ControllerMode(socd_type, 100) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    this->ledgedash_max_jump_traj = ledgedash_max_jump_traj;
    this->true_z_press = true_z_press;
    horizontal_socd = false;
}

void ProjectM::HandleSocd(InputState &inputs) {
    horizontal_socd = inputs.left && inputs.right;
    InputMode::HandleSocd(inputs);
}

void ProjectM::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    // True Z press vs macro lightshield + A.
    if (true_z_press || inputs.mod_x) {
        outputs.buttonR = inputs.z;
    } else {
        outputs.a = inputs.a || inputs.z;
    }
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

    // Don't override dpad up if it's already pressed using the MX + MY dpad
    // layer.
    outputs.dpadUp = outputs.dpadUp || inputs.midshield;

    if (inputs.select)
        outputs.dpadLeft = true;
    if (inputs.home)
        outputs.dpadRight = true;
}

void ProjectM::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.lightshield || inputs.midshield;

    if (directions.diagonal) {
        if (directions.y > 0) {
            SetLeftStick(outputs, 8300, 9300); // 48.25195°
        }
    }

    if (inputs.mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(outputs, 7000);
        }
        if (directions.vertical) {
            SetLeftStickY(outputs, 6000);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetStick(&outputs.rightStickX, &outputs.rightStickY, directions.cx, directions.y, 6500, 2300); // 19.48613°
        }

        if (directions.diagonal) {
            SetLeftStick(outputs, 7000, 3400); // 25.90651°

            if (inputs.b) {
                SetLeftStick(outputs, 8500, 3100); // 20.03721°
            }

            if (inputs.r) {
                SetLeftStick(outputs, 8200, 3500); // 23.11421°
            }

            if (inputs.c_up) {
                SetLeftStick(outputs, 7700, 5500); // 35.53768°
            }

            if (inputs.c_down) {
                SetLeftStick(outputs, 8200, 3600); // 23.70265°
            }

            if (inputs.c_left) {
                SetLeftStick(outputs, 8400, 5000); // 30.76272°
            }

            if (inputs.c_right) {
                SetLeftStick(outputs, 7200, 6100); // 40.27201°
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 35);
        }
        if (directions.vertical) {
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 70);
        }

        if (directions.diagonal) {
            SetLeftStick(outputs, 2800, 5800); // 64.23067°

            if (inputs.b) {
                SetLeftStick(outputs, 2800, 8500); // 71.76751°
            }

            if (inputs.r) {
                SetLeftStick(outputs, 5100, 8200); // 58.1204°
            }

            if (inputs.c_up) {
                SetLeftStick(outputs, 5500, 7700); // 54.46232°
            }

            if (inputs.c_down) {
                SetLeftStick(outputs, 3400, 8200); // 67.47943°
            }

            if (inputs.c_left) {
                SetLeftStick(outputs, 4000, 8400); // 64.53665°
            }

            if (inputs.c_right) {
                SetLeftStick(outputs, 6200, 7200); // 49.26789°
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    // We don't apply this for c-up + c-left/c-right in case we want to implement
    // C-stick nair somehow.
    if (directions.cx != 0 && directions.cy < 0) {
        SetLeftStick(outputs, 3500, 9800); // 70.34618°
    }

    // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
    // trajectory).
    if (ledgedash_max_jump_traj && horizontal_socd && !directions.vertical &&
        !shield_button_pressed) {
        SetLeftStickX(outputs, 10000);
    }

    if (inputs.lightshield) {
        outputs.triggerRAnalog = 49;
    }
    if (inputs.midshield) {
        outputs.triggerRAnalog = 94;
    }

    // Send lightshield input if we are using Z = lightshield + A macro.
    if (inputs.z && !(inputs.mod_x || true_z_press)) {
        outputs.triggerRAnalog = 49;
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
