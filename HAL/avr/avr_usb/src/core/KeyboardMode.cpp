#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

#include <ArduinoKeyboard.hpp>

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {}

KeyboardMode::~KeyboardMode() {
    _keyboard.releaseAll();
}

void KeyboardMode::SendReport(InputState &inputs) {
    HandleSocd(inputs);
    SendKeys(inputs);
    _keyboard.sendReport();
}

void KeyboardMode::Press(uint8_t key, bool press) {
    _keyboard.setPressed(key, press);
}