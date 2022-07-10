#include "core/ControllerMode.hpp"

ControllerMode::ControllerMode(socd::SocdType socd_type, uint8_t analog_stick_length) : InputMode(socd_type) {
    this->analog_stick_length = analog_stick_length;

    // Set up initial state.
    ResetDirections();
}

void ControllerMode::UpdateOutputs(InputState &inputs, OutputState &outputs) {
    HandleSocd(inputs);
    UpdateDigitalOutputs(inputs, outputs);
    UpdateAnalogOutputs(inputs, outputs);
}

void ControllerMode::ResetDirections() {
    directions = {
        .horizontal = false,
        .vertical = false,
        .diagonal = false,
        .x = 0,
        .y = 0,
        .cx = 0,
        .cy = 0,
    };
}

void ControllerMode::UpdateDirections(
    bool lsLeft,
    bool lsRight,
    bool lsDown,
    bool lsUp,
    bool rsLeft,
    bool rsRight,
    bool rsDown,
    bool rsUp,
    OutputState &outputs
) {
    uint8_t analogStickMin = ANALOG_STICK_NEUTRAL - analog_stick_length;
    uint8_t analogStickMax = ANALOG_STICK_NEUTRAL + analog_stick_length;
    ResetDirections();

    outputs.leftStickX = ANALOG_STICK_NEUTRAL;
    outputs.leftStickY = ANALOG_STICK_NEUTRAL;
    outputs.rightStickX = ANALOG_STICK_NEUTRAL;
    outputs.rightStickY = ANALOG_STICK_NEUTRAL;

    // Coordinate calculations to make modifier handling simpler.
    if (lsLeft || lsRight) {
        directions.horizontal = true;
        if (lsLeft) {
            directions.x = -1;
            outputs.leftStickX = analogStickMin;
        } else {
            directions.x = 1;
            outputs.leftStickX = analogStickMax;
        }
    }
    if (lsDown || lsUp) {
        directions.vertical = true;
        if (lsDown) {
            directions.y = -1;
            outputs.leftStickY = analogStickMin;
        } else {
            directions.y = 1;
            outputs.leftStickY = analogStickMax;
        }
    }
    if (directions.horizontal && directions.vertical)
        directions.diagonal = true;

    if (rsLeft || rsRight) {
        if (rsLeft) {
            directions.cx = -1;
            outputs.rightStickX = analogStickMin;
        } else {
            directions.cx = 1;
            outputs.rightStickX = analogStickMax;
        }
    }
    if (rsDown || rsUp) {
        if (rsDown) {
            directions.cy = -1;
            outputs.rightStickY = analogStickMin;
        } else {
            directions.cy = 1;
            outputs.rightStickY = analogStickMax;
        }
    }
}

void ControllerMode::SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value) {
    *axis = ANALOG_STICK_NEUTRAL + direction * (uint8_t)(value / (10000 / analog_stick_length));
}

void ControllerMode::SetLeftStickX(OutputState &outputs, const uint16_t &value) {
    SetAxis(&outputs.leftStickX, directions.x, value);
}

void ControllerMode::SetLeftStickY(OutputState &outputs, const uint16_t &value) {
    SetAxis(&outputs.leftStickY, directions.y, value);
}

void ControllerMode::SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue) {
    SetAxis(xAxis, xDirection, xValue);
    SetAxis(yAxis, yDirection, yValue);
}

void ControllerMode::SetLeftStick(OutputState &outputs, const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&outputs.leftStickX, &outputs.leftStickY, directions.x, directions.y, xValue, yValue);
}

void ControllerMode::SetRightStick(OutputState &outputs, const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&outputs.rightStickX, &outputs.leftStickY, directions.x, directions.y, xValue, yValue);
}