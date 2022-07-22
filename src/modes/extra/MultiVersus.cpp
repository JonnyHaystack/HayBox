#include "modes/extra/MultiVersus.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MultiVersus::MultiVersus(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void MultiVersus::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Bind X and Y to "jump" in-game.
    outputs.x = inputs.x;
    outputs.y = inputs.y;

    outputs.start = inputs.start;

    // Select or MS for "Reset" in the Lab. Not supported by GameCube adapter.
    outputs.select = inputs.select || inputs.midshield;

    // Home not supported by GameCube adapter.
    outputs.home = inputs.home;

    if (!inputs.mod_x) {
        // Bind A to "attack" in-game.
        outputs.a = inputs.a;

        // Bind B to "special" in-game.
        outputs.b = inputs.b;

        // Z = RB. Bind to "dodge" in-game.
        outputs.buttonR = inputs.z;

        // LS = LB. Not supported by GameCube adapter.
        outputs.buttonL = inputs.lightshield;
    }

    if (inputs.mod_x && !inputs.mod_y) {
        // MX activates a layer for "neutral" binds. Uses D-Pad buttons.

        // MX + A = D-Pad Left. Bind to "neutral attack" in-game.
        outputs.dpadLeft = inputs.a;

        // MX + B = D-Pad Right. Bind to "neutral special" in-game.
        outputs.dpadRight = inputs.b;

        // MX + Z = D-Pad Down. Bind to "neutral evade" in-game.
        outputs.dpadDown = inputs.z;

        // MX + LS = D-Pad Up (for taunt 1)
        outputs.dpadUp = inputs.y;
    }

    if (inputs.mod_y && !inputs.mod_x) {
        // MY activates C-Stick to D-Pad conversion.
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadUp = inputs.c_up;
    }
}

void MultiVersus::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
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

    if (inputs.mod_y) {
        // MY slows down the cursor for easier menu navigation.
        // 128 ± 76 results in the slowest cursor that still actuates directional inputs in-game.
        outputs.leftStickX = ANALOG_STICK_NEUTRAL + directions.x * 76;
        outputs.leftStickY = ANALOG_STICK_NEUTRAL + directions.y * 76;

        if (directions.diagonal) {
            // Maintain a consistent cursor velocity when MY is held.
            // ⌊76 × √2/2⌋ = 53
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + directions.x * 53;
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + directions.y * 53;
            
        }

        // Also shut off C-Stick for D-Pad conversion.
        outputs.rightStickX = ANALOG_STICK_NEUTRAL;
        outputs.rightStickY = ANALOG_STICK_NEUTRAL;
    }

    // R = RT. Can be bound to "pickup item" or left unbound.
    if (inputs.r) {
        outputs.triggerRAnalog = 140;
    }

    // L or Nunchuk Z = LT. Bind to "dodge" in-game.
    if (inputs.l || (inputs.nunchuk_connected && inputs.nunchuk_z)) {
        outputs.triggerLAnalog = 140;
    }
}
