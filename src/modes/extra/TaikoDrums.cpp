/** TaikoDrums profile by VAMOS ( github.com/colonket )
*   Designed for "Taiko no Tatsujin: Drum 'n' Fun!"
*   on Nintendo Switch
**/
#include "modes/extra/TaikoDrums.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

TaikoDrums::TaikoDrums(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 2;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
    };
}

void TaikoDrums::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // LEFT BLUE
    // INPUTS: L, LEFT, MOD_X
    // OUTPUTS: ( DPAD LEFT / DPAD UP / ZL / L )
    outputs.triggerLDigital = inputs.l;
    outputs.dpadLeft = inputs.left;
    outputs.dpadUp = inputs.mod_x;
    // LEFT RED
    // INPUTS: DOWN, RIGHT, MOD_Y
    // OUTPUTS: ( DPAD DOWN / DPAD RIGHT ) 
    outputs.dpadDown = inputs.down;
    outputs.dpadRight = inputs.right;
    outputs.dpadDown = inputs.mod_y;
    // RIGHT BLUE
    // INPUTS: A, Z, UP
    // OUTPUTS: ( A / X / ZR / R )
    outputs.a = inputs.a;
    outputs.buttonR = inputs.z;
    outputs.triggerRDigital = inputs.up;
    // RIGHT RED
    // INPUTS: C_DOWN, B, X
    // OUTPUTS: ( B / Y) 
    outputs.b = inputs.c_down;
    outputs.b = inputs.b;
    outputs.y = inputs.x;
    // Control Row (Right Hand Top Row)
    // INPUTS: R, Y, LS, MS
    // OUTPUTS: ( A / X / SELECT / HOME )
    outputs.a = inputs.r;
    outputs.x = inputs.y;
    outputs.select = inputs.lightshield;
    outputs.home = inputs.midshield;
}

void TaikoDrums::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.mod_x,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );
}