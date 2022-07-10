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

void RivalsOfAether::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
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

    outputs.select = inputs.select;
    outputs.home = inputs.home;
}

void RivalsOfAether::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.r;

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 66);
        }

        // Angled fsmash
        if (directions.cx != 0) {
            outputs.rightStickX = ANALOG_STICK_NEUTRAL + (directions.cx * 65);
            outputs.rightStickY = ANALOG_STICK_NEUTRAL + (directions.y * 23);
        }

        // Need to check coord system in RoA

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 59);
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 23);

            if (inputs.c_down) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 49);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 24);
            }

            if (inputs.c_left) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 52);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 31);
            }

            if (inputs.c_up) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 49);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 35);
            }

            if (inputs.c_right) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 51);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 43);
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 44);
        }

        /* Up B angles */
        if (directions.diagonal && !shield_button_pressed) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 44);
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 113);

            if (inputs.c_down) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 44);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 90);
            }

            if (inputs.c_left) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 44);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 74);
            }

            if (inputs.c_up) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 45);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 63);
            }

            if (inputs.c_right) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 47);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 57);
            }
        }
    }

    if (inputs.l) {
        if (directions.horizontal)
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 100);
        if (directions.vertical)
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 100);
        if (directions.horizontal && (directions.y == -1)) {
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 100);
            outputs.leftStickY = 28; // TODO
        }
    }

    if (inputs.r) {
        if (directions.diagonal) {
            if (inputs.mod_y) {
                outputs.leftStickX = ANALOG_STICK_NEUTRAL + (directions.x * 40);
                outputs.leftStickY = ANALOG_STICK_NEUTRAL + (directions.y * 68);
            }
        }
    }

    // Shut off c-stick when using dpad layer.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
