#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {
    _keyboard->begin();
}

KeyboardMode::~KeyboardMode() {
    _keyboard->releaseAll();
}

void KeyboardMode::SendReport(InputState &inputs) {
    HandleSocd(inputs);
    SendKeys(inputs);
    _keyboard->sendState();
}

void KeyboardMode::Press(uint8_t key, bool press) {
    _keyboard->setPressed(key, press);
}