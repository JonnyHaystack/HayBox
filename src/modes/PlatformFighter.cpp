#include "modes/PlatformFighter.hpp"

PlatformFighter::PlatformFighter(socd::SocdType socd_type, uint8_t analog_stick_length)
    : ControllerMode(socd_type, analog_stick_length) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    _horizontal_socd = false;
}

void PlatformFighter::SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value) {
    *axis = ANALOG_STICK_NEUTRAL + direction * (uint8_t)(value / (10000 / _analog_stick_length));
}

void PlatformFighter::HandleSocd() {
    _horizontal_socd = _inputs->left && _inputs->right;
    InputMode::HandleSocd();
}

void PlatformFighter::SetLeftStickX(const uint16_t &value) {
    SetAxis(&_outputs->leftStickX, directions.x, value);
}

void PlatformFighter::SetLeftStickY(const uint16_t &value) {
    SetAxis(&_outputs->leftStickY, directions.y, value);
}

void PlatformFighter::SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue) {
    SetAxis(xAxis, xDirection, xValue);
    SetAxis(yAxis, yDirection, yValue);
}

void PlatformFighter::SetLeftStick(const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&_outputs->leftStickX, &_outputs->leftStickY, directions.x, directions.y, xValue, yValue);
}

void PlatformFighter::SetRightStick(const uint16_t &xValue, const uint16_t &yValue) {
    SetStick(&_outputs->rightStickX, &_outputs->leftStickY, directions.x, directions.y, xValue, yValue);
}

void PlatformFighter::SetAngledFSmash(const uint16_t &xValue, const uint16_t yValue) {
    SetStick(&_outputs->rightStickX, &_outputs->rightStickY, directions.cx, directions.y, xValue, yValue);
}