#include "modes/Rivals2.hpp"

#define ANALOG_STICK_MIN 0 
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255 


Rivals2::Rivals2(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void Rivals2::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
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
    outputs.buttonL = inputs.midshield;

    // Activate D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void Rivals2::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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

   bool shield_button_pressed = inputs.r || inputs.l;

   if (directions.diagonal && !shield_button_pressed) { //added this conditional to give joystick accurate diagonals rather than (+/- 1.2, 1.2) should be (0.87~, 0.87~)
    outputs.leftStickX = 128 + (directions.x * 100); //also allows fastfalling with down while holding forward
    outputs.leftStickY = 128 + (directions.y * 100);
   }

   if (directions.diagonal && shield_button_pressed) {
    outputs.leftStickX = 128 + (directions.x * 92); // (0.77~, 0.77~) to prevent spot dodging when pressing diagonal on the ground
    outputs.leftStickY = 128 + (directions.y * 92);  
   }

   /* coordinate notes
    Dash Start - X: 0.8 
    FastFall - Y: 0.8
    Teeter Break - X: 0.9
    Crouched Turn - X: 0.3
    Tilts - Direction: 0.3
    PlatDrop - Y: -0.8
    *Credit to KhaoKG for coords listed above

    0.3 Y value for upward/downward angled tilts

    0.24 or (directions.x/y * 47) seems to be the minimum/steepest angle available with forsburn
    zetterburn can angle a bit steeper around 0.1 
    

    When Shielding
    y value <0.28 = nothing
    y value 0.28~ through 0.79~ = shield drop on plat
    x value 0.8 thru 1.2 initiates roll
    y value 0.8 thru 1.2 initiates spot-dodge on platform
    y value 0.7 or more initiates tap jump 

    UPDATES (10-31-2024)
    Shielding on platform while holding ModX will now not allow rolls nor tap jumps nor spotdodges
    Pressing diagonal while shielding on the ground will now not spotdodge (mimic Melee behavior)
    Angled forward tilts work now

    TO-DO
    VERIFY that Forsburn's 80 and 60% up B angles are good
   
   */

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 90); //90 gives 0.77~ in-game for a max speed walk. dash begins at 0.8
            
            // MX Horizontal Tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 53); // 53 converts to 0.31~ in-game; 0.3 is min required for tilts
            }
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 48); // 48 (0.25~ in-game), this prevents shield drops while holding modX
            // MX Vertical Tilts
            if (inputs.a) {
                outputs.leftStickY = 128 + (directions.y * 53); // 53 = 0.31; tilts begin at 0.3
            }
        }

            /* 100% Magnitude UpB when holding B */
        if (!inputs.z && inputs.b && directions.diagonal && !shield_button_pressed) {
            // (x, y), (92, 47), (0.77~, 0.24~) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 92);
            outputs.leftStickY = 128 + (directions.y * 47);
            // (x, y), (89, 50), (0.75~, 0.27~) [coords, code_values, in-game values]
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 89);
                outputs.leftStickY = 128 + (directions.y * 50);
            }
            // (x, y), (85, 55), (0.7~, 0.34~) [coords, code_values, in-game values]
            if (inputs.c_left) {
                 outputs.leftStickX = 128 + (directions.x * 85);
                 outputs.leftStickY = 128 + (directions.y * 55);
            }
            // (x, y), (81, 60), (0.65~, 0.38~) [coords, code_values, in-game values]
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 81);
                outputs.leftStickY = 128 + (directions.y * 60);
            }
            // (x, y), (76, 65), (0.6~, 0.46~) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 76);
                outputs.leftStickY = 128 + (directions.y * 65);
            }
        }
        /* 80% Magnitude UpB when not holding B nor Z*/
        else if (!inputs.z && !inputs.b && directions.diagonal && !shield_button_pressed) {
            // (x, y), (74, 47), (0.56~, 0.24~) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 74);
            outputs.leftStickY = 128 + (directions.y * 47);
            // (x, y), (71, 49), (~0.53, 0.27~) [coords, code_values, in-game values]
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 71);
                outputs.leftStickY = 128 + (directions.y * 49);
            }
            // (x, y), (68, 51), (~0.49, ~0.29) [coords, code_values, in-game values]
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 68);
                outputs.leftStickY = 128 + (directions.y * 51);
            }
            // (x, y), (65, 54), (~0.45, ~0.32) [coords, code_values, in-game values]
            if (inputs.c_up) { 
                outputs.leftStickX = 128 + (directions.x * 65);
                outputs.leftStickY = 128 + (directions.y * 54);
            }
            // (x, y), (61, 57), (~0.41, ~0.36) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 61);
                outputs.leftStickY = 128 + (directions.y * 57);
            }
             // (x, y), (69, 53), (~0.506, ~0.31) [coords, code_values, in-game values]
            //for angled forward tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 69);
                outputs.leftStickY = 128 + (directions.y * 53);
            }
        }
        /* 60% Magnitude UpB when holding Z*/
        else if (inputs.z && directions.diagonal && !shield_button_pressed) { 
            // (x, y), (55, 47), (~0.34, ~0.24) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 55);
            outputs.leftStickY = 128 + (directions.y * 47);
            // (x, y), (53, 49), (~0.31, ~0.27) [coords, code_values, in-game values]
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 49);
            }
            // (x, y), (51, 51), (~0.29, 0.29~) [coords, code_values, in-game values]
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 51);
            }
            // (x, y), (48, 54), (~0.25, ~0.32) [coords, code_values, in-game values]
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 48);
                outputs.leftStickY = 128 + (directions.y * 54);
            }
            // (x, y), (46, 57), (~0.23, ~0.36) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 46);
                outputs.leftStickY = 128 + (directions.y * 57);
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 53); //53 equates to 0.318~ in-game. 0.3 is min to achieve a walk
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 96); // this should give both fast fall and tap jump; spot dodge on plat
        }
            /* 100% Magnitude UpB when holding B*/
        if (!inputs.z && inputs.b && directions.diagonal && !shield_button_pressed) {
            // (x, y), (47, 92), (~0.24, ~0.77) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 47);
            outputs.leftStickY = 128 + (directions.y * 92);
            // (x, y), (50, 89), (~0.27, ~0.74) [coords, code_values, in-game values]
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 50);
                outputs.leftStickY = 128 + (directions.y * 89);
            }
            // (x, y), (55, 85), (~0.34, 0.69~) [coords, code_values, in-game values]
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 55);
                outputs.leftStickY = 128 + (directions.y * 85);
            }
            // (x, y), (60, 81), (~0.38, 0.64~) [coords, code_values, in-game values]
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 60);
                outputs.leftStickY = 128 + (directions.y * 81);
            }
            // (x, y), (65, 76), (~0.46, 0.58~) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 65);
                outputs.leftStickY = 128 + (directions.y * 76);
            }
        }
            /* 80% Magnitude UpB when not holding B nor Z*/
        else if (!inputs.z && !inputs.b && directions.diagonal && !shield_button_pressed) {
            // (x, y), (47, 74), (~0.24, 0.56~) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 47);
            outputs.leftStickY = 128 + (directions.y * 74);
            // (x, y), (49, 71), (~0.27, ~0.52) [coords, code_values, in-game values]
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 49);
                outputs.leftStickY = 128 + (directions.y * 71);
            }
            // (x, y), (51, 68), (~0.29, ~0.49) [coords, code_values, in-game values]
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 68);
            }
            // (x, y), (54, 65), (~0.32, ~0.46) [coords, code_values, in-game values]
            if (inputs.c_up) { 
                outputs.leftStickX = 128 + (directions.x * 54);
                outputs.leftStickY = 128 + (directions.y * 65);
            }
            // (x, y), (57, 61), (~0.36, ~0.41) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 57);
                outputs.leftStickY = 128 + (directions.y * 61);
            }
        }
            /* 60% Magnitude UpB when holding Z*/
        else if (inputs.z && directions.diagonal && !shield_button_pressed) {
            // (x, y), (47, 55), (~0.24, ~0.34) [coords, code_values, in-game values]
            outputs.leftStickX = 128 + (directions.x * 47);
            outputs.leftStickY = 128 + (directions.y * 55);
            // (x, y), (49, 53), (~0.27, ~0.31) [coords, code_values, in-game values]
            if (inputs.c_down) {
                 outputs.leftStickX = 128 + (directions.x * 49);
                 outputs.leftStickY = 128 + (directions.y * 53);
            }
            // (x, y), (51, 51), (~0.29, ~0.29) [coords, code_values, in-game values]
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 51);
            }
            // (x, y), (54, 48), (~0.32, ~0.25) [coords, code_values, in-game values]
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 54);
                outputs.leftStickY = 128 + (directions.y * 48);
            }
            // (x, y), (57, 46), (~0.36, ~0.23) [coords, code_values, in-game values]
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 57);
                outputs.leftStickY = 128 + (directions.y * 46);
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
