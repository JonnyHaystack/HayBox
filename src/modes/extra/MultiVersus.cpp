#include "modes/extra/MultiVersus.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MultiVersus::MultiVersus(socd::SocdType socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void MultiVersus::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Bind X and Y to "jump" in-game.
    outputs.x = inputs.x;
    outputs.y = inputs.y;

    outputs.start = !inputs.mod_y && inputs.start;

    // Select, MS, or MY + Start for "Reset" in the Lab. Not supported by GameCube adapter.
    outputs.select = inputs.select || inputs.midshield || (inputs.mod_y && inputs.start);

    // Home not supported by GameCube adapter.
    outputs.home = inputs.home;

    // L or Nunchuk Z = LT. Bind to "dodge" in-game.
    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.l;
    }

    // R = RT. Can be bound to "pickup item" or left unbound.
    outputs.triggerRDigital = inputs.r;

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

    // MX activates a layer for "neutral" binds. Uses D-Pad buttons.
    if (inputs.mod_x && !inputs.mod_y) {
        // MX + A = D-Pad Left. Bind to "neutral attack" in-game.
        outputs.dpadLeft = inputs.a;

        // MX + B = D-Pad Right. Bind to "neutral special" in-game.
        outputs.dpadRight = inputs.b;

        // MX + Z = D-Pad Down. Bind to "neutral evade" in-game.
        outputs.dpadDown = inputs.z;

        // MX + LS = D-Pad Up. Bind to "taunt 1" in-game.
        outputs.dpadUp = inputs.lightshield;
    }

    // MY activates C-Stick to D-Pad conversion.
    if (inputs.mod_y && !inputs.mod_x) {
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

    if (inputs.mod_y && !inputs.mod_x) {
        // MY slows down the cursor for easier menu navigation.
        // Menu cursor speed can also be turned down in-game under "Interface" settings.
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

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 140;
    }

    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 140;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
