#include "core/ControllerMode.hpp"

ControllerMode::ControllerMode(socd::SocdType socd_type, uint8_t analog_stick_length) : InputMode(socd_type) {
    _analog_stick_length = analog_stick_length;

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
    uint8_t analogStickMin = ANALOG_STICK_NEUTRAL - _analog_stick_length;
    uint8_t analogStickMax = ANALOG_STICK_NEUTRAL + _analog_stick_length;
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
