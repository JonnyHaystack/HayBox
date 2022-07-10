#include "core/ControllerMode.hpp"

ControllerMode::ControllerMode(socd::SocdType socd_type, uint8_t analog_stick_length) : InputMode(socd_type) {
    this->analog_stick_length = analog_stick_length;

    // Set up initial state.
    ResetDirections();
}

void ControllerMode::UpdateOutputs(InputState* inputs, OutputState* outputs) {
    _inputs = inputs;
    _outputs = outputs;

    if (inputs == nullptr || outputs == nullptr)
        return;

    HandleSocd();
    UpdateDigitalOutputs();
    UpdateAnalogOutputs();
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
    bool rsUp
) {
    uint8_t analogStickMin = ANALOG_STICK_NEUTRAL - analog_stick_length;
    uint8_t analogStickMax = ANALOG_STICK_NEUTRAL + analog_stick_length;
    ResetDirections();

    _outputs->leftStickX = ANALOG_STICK_NEUTRAL;
    _outputs->leftStickY = ANALOG_STICK_NEUTRAL;
    _outputs->rightStickX = ANALOG_STICK_NEUTRAL;
    _outputs->rightStickY = ANALOG_STICK_NEUTRAL;

    // Coordinate calculations to make modifier handling simpler.
    if (lsLeft || lsRight) {
        directions.horizontal = true;
        if (lsLeft) {
            directions.x = -1;
            _outputs->leftStickX = analogStickMin;
        } else {
            directions.x = 1;
            _outputs->leftStickX = analogStickMax;
        }
    }
    if (lsDown || lsUp) {
        directions.vertical = true;
        if (lsDown) {
            directions.y = -1;
            _outputs->leftStickY = analogStickMin;
        } else {
            directions.y = 1;
            _outputs->leftStickY = analogStickMax;
        }
    }
    if (directions.horizontal && directions.vertical)
        directions.diagonal = true;

    if (rsLeft || rsRight) {
        if (rsLeft) {
            directions.cx = -1;
            _outputs->rightStickX = analogStickMin;
        } else {
            directions.cx = 1;
            _outputs->rightStickX = analogStickMax;
        }
    }
    if (rsDown || rsUp) {
        if (rsDown) {
            directions.cy = -1;
            _outputs->rightStickY = analogStickMin;
        } else {
            directions.cy = 1;
            _outputs->rightStickY = analogStickMax;
        }
    }
}

void ControllerMode::SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value) {
    *axis = ANALOG_STICK_NEUTRAL + direction * (uint8_t)(value / (10000 / analog_stick_length));
}

void ControllerMode::SetLeftStickX(const uint16_t &value) {
    SetAxis(&_outputs->leftStickX, directions.x, value);
}

void ControllerMode::SetLeftStickY(const uint16_t &value) {
    SetAxis(&_outputs->leftStickY, directions.y, value);
}

void ControllerMode::SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue) {
    SetAxis(xAxis, xDirection, xValue);
    SetAxis(yAxis, yDirection, yValue);
}

void ControllerMode::SetLeftStick(const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&_outputs->leftStickX, &_outputs->leftStickY, directions.x, directions.y, xValue, yValue);
}

void ControllerMode::SetRightStick(const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&_outputs->rightStickX, &_outputs->leftStickY, directions.x, directions.y, xValue, yValue);
}