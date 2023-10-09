#include "modes/extra/SaltAndSanctuary.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

SaltAndSanctuary::SaltAndSanctuary(const GameModeConfig &config) : ControllerMode(config) {}

void SaltAndSanctuary::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.dpadRight = inputs.lf4; // Block
    outputs.b = inputs.rf1; // Roll
    outputs.a = inputs.rt1; // Attack
    outputs.y = inputs.rf3; // Strong
    outputs.dpadDown = inputs.lt2; // Use
    outputs.x = inputs.rf2; // Jump

    outputs.buttonL = inputs.rf5; // Previous item
    outputs.buttonR = inputs.rf6; // Next item
    outputs.triggerLDigital = inputs.rf7; // Use item

    outputs.triggerRDigital = inputs.rf8; // Use torch

    outputs.dpadLeft = inputs.rf4; // Switch loadout

    outputs.start = inputs.mb1; // Inventory
}

void SaltAndSanctuary::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.lf3,
        inputs.lf1,
        inputs.lf2,
        inputs.lt1,
        inputs.rt3,
        inputs.rt5,
        inputs.rt2,
        inputs.rt4,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );
}
