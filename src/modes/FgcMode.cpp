#include "modes/FgcMode.hpp"

FgcMode::FgcMode(GameModeConfig &config) : ControllerMode(config) {}

void FgcMode::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    // Directions
    outputs.dpadLeft = inputs.lf3;
    outputs.dpadRight = inputs.lf1;
    outputs.dpadDown = inputs.lf2;
    outputs.dpadUp = inputs.lt1;

    // Menu keys
    outputs.start = inputs.mb1;
    outputs.select = inputs.rt3;
    outputs.home = inputs.rt2;

    // Right hand bottom row
    outputs.a = inputs.rf1;
    outputs.b = inputs.rf2;
    outputs.triggerRDigital = inputs.rf3;
    outputs.triggerLDigital = inputs.rf4;

    // Right hand top row
    outputs.x = inputs.rf5;
    outputs.y = inputs.rf6;
    outputs.buttonR = inputs.rf7;
    outputs.buttonL = inputs.rf8;
}

void FgcMode::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.leftStickX = 128;
    outputs.leftStickY = 128;
    outputs.rightStickX = 128;
    outputs.rightStickY = 128;
    outputs.triggerLAnalog = outputs.triggerLDigital ? 255 : 0;
    outputs.triggerRAnalog = outputs.triggerRDigital ? 255 : 0;
}
