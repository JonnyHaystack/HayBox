#include "modes/RivalsOfAether.hpp"

RivalsOfAether::RivalsOfAether(socd::SocdType socd_type) : ControllerMode(socd_type, 100) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void RivalsOfAether::UpdateDigitalOutputs(InputState &inputs) {
    outputs->a = inputs.a;
    outputs->b = inputs.b;
    outputs->x = inputs.x;
    outputs->y = inputs.y;
    outputs->buttonR = inputs.z;
    if (inputs.nunchuk_connected) {
        // Lightshield with C button.
        if (inputs.nunchuk_c) {
            outputs->triggerLAnalog = 49;
        }
        outputs->triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs->triggerLDigital = inputs.l;
    }
    outputs->triggerRDigital = inputs.r;
    outputs->start = inputs.start;

    /********* DPAD *********/
    if (inputs.mod_x && inputs.mod_y) {
        outputs->dpadUp = inputs.c_up;
        outputs->dpadDown = inputs.c_down;
        outputs->dpadLeft = inputs.c_left;
        outputs->dpadRight = inputs.c_right;
    }

    outputs->select = inputs.select;
    outputs->home = inputs.home;
}

void RivalsOfAether::UpdateAnalogOutputs(InputState &inputs) {
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

    bool shield_button_pressed = inputs.l || inputs.r;

    if (inputs.mod_x) {
        if (directions.horizontal) {
            SetLeftStickX(6600);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            SetStick(&outputs->rightStickX, &outputs->rightStickY, directions.cx, directions.y, 6500, 2300); // 19.48613°
        }

        // Need to check coord system in RoA

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(5900, 2300); // 21.29735°

            if (inputs.c_down) {
                SetLeftStick(4900, 2400); // 26.09542°
            }

            if (inputs.c_left) {
                SetLeftStick(5200, 3100); // 30.80145°
            }

            if (inputs.c_up) {
                SetLeftStick(4900, 3500); // 35.53768°
            }

            if (inputs.c_right) {
                SetLeftStick(5100, 4300); // 40.13549°
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            SetLeftStickX(4400);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            SetLeftStick(4400, 11300); // 68.72503°

            if (inputs.c_down) {
                SetLeftStick(4400, 9000); // 63.9465°
            }

            if (inputs.c_left) {
                SetLeftStick(4400, 7400); // 59.26451°
            }

            if (inputs.c_up) {
                SetLeftStick(4500, 6300); // 54.46232°
            }

            if (inputs.c_right) {
                SetLeftStick(4700, 5700); // 50.49232°
            }
        }
    }

    if (inputs.l) {
        if (directions.horizontal)
            SetLeftStickX(10000);
        if (directions.vertical)
            SetLeftStickY(10000);
        if (directions.horizontal && directions.y < 0) {
            SetLeftStick(10000, 2800); // 15.64225°
        }
    }

    if (inputs.r) {
        if (directions.diagonal) {
            if (inputs.mod_y) {
                SetLeftStick(4000, 6800); // 59.53446°
            }
        }
    }

    // Shut off c-stick when using dpad layer.
    if (inputs.mod_x && inputs.mod_y) {
        SetLeftStick(0000, 0000); // 0°
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs->leftStickX = inputs.nunchuk_x;
        outputs->leftStickY = inputs.nunchuk_y;
    }
}
