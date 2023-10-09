#include "modes/extra/MultiVersus.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MultiVersus::MultiVersus(const GameModeConfig &config) : ControllerMode(config) {}

void MultiVersus::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    // Bind X and Y to "jump" in-game.
    outputs.x = inputs.rf2;
    outputs.y = inputs.rf6;

    outputs.start = !inputs.lt2 && inputs.mb1;

    // Select, MS, or MY + Start for "Reset" in the Lab. Not supported by GameCube adapter.
    outputs.select = inputs.mb3 || inputs.rf8 || (inputs.lt2 && inputs.mb1);

    // Home not supported by GameCube adapter.
    outputs.home = inputs.mb2;

    // L or Nunchuk Z = LT. Bind to "dodge" in-game.
    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.lf4;
    }

    // R = RT. Can be bound to "pickup item" or left unbound.
    outputs.triggerRDigital = inputs.rf5;

    if (!inputs.lt1) {
        // Bind A to "attack" in-game.
        outputs.a = inputs.rt1;

        // Bind B to "special" in-game.
        outputs.b = inputs.rf1;

        // Z = RB. Bind to "dodge" in-game.
        outputs.buttonR = inputs.rf3;

        // LS = LB. Not supported by GameCube adapter.
        outputs.buttonL = inputs.rf7;
    }

    // MX activates a layer for "neutral" binds. Uses D-Pad buttons.
    if (inputs.lt1 && !inputs.lt2) {
        // MX + A = D-Pad Left. Bind to "neutral attack" in-game.
        outputs.dpadLeft = inputs.rt1;

        // MX + B = D-Pad Right. Bind to "neutral special" in-game.
        outputs.dpadRight = inputs.rf1;

        // MX + Z = D-Pad Down. Bind to "neutral evade" in-game.
        outputs.dpadDown = inputs.rf3;

        // MX + LS = D-Pad Up. Bind to "taunt 1" in-game.
        outputs.dpadUp = inputs.rf7;
    }

    // MY activates C-Stick to D-Pad conversion.
    if (inputs.lt2 && !inputs.lt1) {
        outputs.dpadLeft = inputs.rt3;
        outputs.dpadRight = inputs.rt5;
        outputs.dpadDown = inputs.rt2;
        outputs.dpadUp = inputs.rt4;
    }
}

void MultiVersus::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.lf3, // Left
        inputs.lf1, // Right
        inputs.lf2, // Down
        inputs.rf4, // Up
        inputs.rt3, // C-Left
        inputs.rt5, // C-Right
        inputs.rt2, // C-Down
        inputs.rt4, // C-Up
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    if (inputs.lt2 && !inputs.lt1) {
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
