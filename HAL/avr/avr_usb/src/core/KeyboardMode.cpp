#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

#include <ArduinoKeyboard.hpp>

KeyboardMode::KeyboardMode(GameModeConfig &config) : InputMode(config) {}

KeyboardMode::~KeyboardMode() {
    _keyboard.releaseAll();
    _keyboard.sendReport();
}

void KeyboardMode::SendReport(const InputState &inputs) {
    InputState remapped_inputs = inputs;
    HandleRemap(inputs, remapped_inputs);
    HandleSocd(remapped_inputs);
    UpdateKeys(inputs);
    _keyboard.sendReport();
}

void KeyboardMode::Press(uint8_t keycode, bool press) {
    _keyboard.setPressed(keycode, press);
}