#include "modes/extra/HollowKnight.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

HollowKnight::HollowKnight(const GameModeConfig &config) : ControllerMode(config) {}

void HollowKnight::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.rt1; // Attack
    outputs.b = inputs.rf1; // Dash
    outputs.x = inputs.rf2; // Jump
    outputs.y = inputs.lt2; // Quick Cast
    outputs.triggerLDigital = inputs.rf5; // Focus / Cast
    outputs.triggerRDigital = inputs.rf3; // C-Dash
    outputs.buttonR = inputs.rf4; // Dream Nail

    outputs.buttonL = inputs.rf7; // Map
    outputs.select = inputs.rf8; // Inventory
    outputs.start = inputs.mb1; // Pause
}

void HollowKnight::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
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
