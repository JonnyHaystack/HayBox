/** TaikoDrums profile by VAMOS ( github.com/colonket )
*   Designed for "Taiko no Tatsujin: Drum 'n' Fun!"
*   on Nintendo Switch
*
*   To add this mode to your controller, edit `config/mode_selection.hpp`
*   
*   After the include statements, add:
*   #include "modes/extra/TaikoDrums.hpp"
*
*   To select this mode with ModX+Start+A : 
*   After the mode selection if statements, (reccomended after Rivals) add:
*   } else if (inputs.a) {
            set_mode(backend, new TaikoDrums(socd::SOCD_NEUTRAL));
    }
*
*   NOTICE
*   Taiko no Tatsujin uses a different button scheme in-game
*   compared to the typical Nintendo Pro Controller scheme
*   since the game's button layout were meant for Joy-Cons
*
*   TO NAVIGATE THE IN-GAME MENU
*   UP and DOWN is like FGC mode, where Down is DOWN, and Mod_X is UP
*   A and B are normal
*   LEFT and RIGHT are ZL and ZR (The farthest left and farthest right buttons )

*   ==========================================================================
*   INPUT  | OUTPUT (Normal) | OUTPUT (In-Game Menu) | DRUM 
*   ==========================================================================
*    Down  | DPAD Down       | Down                  |  LEFT RED
*    ModX  | DPAD Up         | Up                    |  LEFT BLUE
*    Left  | DPAD Left       | Left                  |  LEFT BLUE
*    Right | DPAD Right      | Left                  |  LEFT RED
*    L     | ZL              | Left                  |  LEFT BLUE
*    Up    | ZR              | Right                 |  RIGHT BLUE
*    A     | A               | A                     |  RIGHT BLUE
*    B     | B               | B                     |  RIGHT RED
*    X     | Y               | Y                     |  RIGHT RED
*    Z     | R               | R                     |  RIGHT BLUE
*    Y     | X               | Random Song           |  Right Blue (Unintended)
*    LS    | MINUS / SELECT  | Option                |  N/A
*    MS    | HOME            | HOME                  |  N/A
*
=============IN SONG================ (B)lue, (R)ed
                        X minus
                      /     home 
      B R               R B
    B     R           R     B
              start   /
          B          /B/
           /         /
=============IN MENU================
                            X minus
                          /     home 
      Left Down             Y R
    Left     Left         B     Right
                start     /
             Up          /A/
              /         /
=============SWITCH CONTROLS=========
                           X minus
                         /     home 
      Left Down            Y R
    ZL      Right        B     ZR
                start    /
            Up          /A/
             /          /
=====================================
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
    //outputs.dpadDown = inputs.mod_y;
    // RIGHT BLUE
    // INPUTS: A, Z, UP
    // OUTPUTS: ( A / X / ZR / R )
    outputs.a = inputs.a; 
    outputs.buttonR = inputs.z;
    outputs.triggerRDigital = inputs.up;
    // RIGHT RED
    // INPUTS: C_DOWN, B, X
    // OUTPUTS: ( B / Y) 
    //outputs.b = inputs.c_down; 
    outputs.b = inputs.b;
    outputs.y = inputs.x;
    // Control Row (Right Hand Top Row)
    // INPUTS: R, Y, LS, MS
    // OUTPUTS: ( A / X / SELECT / HOME )
    //outputs.a = inputs.r;
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