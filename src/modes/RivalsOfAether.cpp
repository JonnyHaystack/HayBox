#include "modes/RivalsOfAether.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

RivalsOfAether::RivalsOfAether(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
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
    outputs.select = inputs.select;
    outputs.home = inputs.home;
    outputs.leftStickClick = inputs.lightshield;
    outputs.rightStickClick = inputs.midshield;

    // Activate D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
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
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.r;


    // 48 total DI angles, 24 total Up b angles, 16 total airdodge angles

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 66);
            // MX Horizontal Tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 44);
            }
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 44);
            // MX Vertical Tilts
            if (inputs.a) {
                outputs.leftStickY = 128 + (directions.y * 67);
            }
        }

        /* Extra DI, Air Dodge, and Up B angles */
        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 59);
            outputs.leftStickY = 128 + (directions.y * 23);

            // Angles just for DI and Up B
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 49);
                outputs.leftStickY = 128 + (directions.y * 24);
            }

            // Angles just for DI
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 52);
                outputs.leftStickY = 128 + (directions.y * 31);
            }
      
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 49);
                outputs.leftStickY = 128 + (directions.y * 35);
            }
     
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 43);
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 44);
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 67);
        }

        /* Extra DI, Air Dodge, and Up B angles */
        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 44);
            outputs.leftStickY = 128 + (directions.y * 113);

            // Angles just for DI and Up B
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 44);
                outputs.leftStickY = 128 + (directions.y * 90);
            }

            // Angles just for DI
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 44);
                outputs.leftStickY = 128 + (directions.y * 74);
            }
      
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 45);
                outputs.leftStickY = 128 + (directions.y * 63);
            }
     
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 47);
                outputs.leftStickY = 128 + (directions.y * 57);
            }
        }
    }

    // Shut off C-stick when using D-Pad layer.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
