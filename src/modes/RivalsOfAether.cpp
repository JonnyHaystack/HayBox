#include "modes/RivalsOfAether.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

RivalsOfAether::RivalsOfAether(socd::SocdType socd_type) : ControllerMode(socd_type) {
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
    outputs.b = inputs.b || inputs.y;
    outputs.x = inputs.x;
    outputs.y = inputs.midshield;
    outputs.buttonR = inputs.z;
    outputs.start = inputs.start;
    outputs.select = inputs.select;
    outputs.home = inputs.home;
    outputs.leftStickClick = inputs.lightshield;
    outputs.rightStickClick = inputs.midshield;

    // Turn on D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_down;
        outputs.dpadDown = inputs.c_up;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void RivalsOfAether::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {    
    // set a tilt modifer
    auto set_analog_stick = [&](int x_percent, int y_percent) {
        outputs.leftStickX = 128 + directions.x * x_percent;
        outputs.leftStickY = 128 + directions.y * y_percent;
    };

    // set the left stick to a forced value
    auto force_analog_stick = [&](int x_value, int y_value) {
        outputs.leftStickX = x_value;
        outputs.leftStickY = y_value;
    };

    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_up, // swapped up and down
        inputs.c_down,
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
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 44);
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
                outputs.leftStickY = 128 + (directions.y * 90);
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

    // light sheild modifers
    if (inputs.lightshield) {
        set_analog_stick(28, 100); // force jolt
    }

    // Shut off A-stick when using D-Pad layer.
    if (inputs.mod_x && inputs.mod_y) force_analog_stick(128, 128);

}
