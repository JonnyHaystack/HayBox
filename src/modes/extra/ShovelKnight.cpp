#include "modes/extra/ShovelKnight.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

ShovelKnight::ShovelKnight(const GameModeConfig &config) : ControllerMode(config) {}

void ShovelKnight::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.dpadLeft = inputs.lf3;
    outputs.dpadRight = inputs.lf1;
    outputs.dpadDown = inputs.lf2;
    outputs.dpadUp = inputs.lt1;

    outputs.b = inputs.rf2; // Jump
    outputs.a = inputs.rt1; // Attack
    outputs.y = inputs.rf1; // Attack
    outputs.x = inputs.rf3; // Subweapon
    outputs.buttonL = inputs.rf5; // Subweapon prev
    outputs.buttonR = inputs.rf6; // Subweapon next

    outputs.select = inputs.rf7; // Inventory
    outputs.start = inputs.mb1; // Pause
}

void ShovelKnight::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
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
