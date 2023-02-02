#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

#include <ArduinoKeyboard.hpp>

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {}

KeyboardMode::~KeyboardMode() {
    _keyboard.releaseAll();
    _keyboard.sendReport();
}

void KeyboardMode::SendReport(InputState &inputs) {
    HandleSocd(inputs);
    UpdateKeys(inputs);
    _keyboard.sendReport();
}

void KeyboardMode::Press(uint8_t keycode, bool press) {
    _keyboard.setPressed(keycode, press);
}