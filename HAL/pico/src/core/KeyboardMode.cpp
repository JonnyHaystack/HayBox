#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

#include <TUKeyboard.hpp>

KeyboardMode::KeyboardMode(GameModeConfig &config) : InputMode(config) {
    _keyboard = new TUKeyboard();
    _keyboard->begin();
}

KeyboardMode::~KeyboardMode() {
    _keyboard->releaseAll();
    _keyboard->sendState();
    delete _keyboard;
}

void KeyboardMode::SendReport(InputState &inputs) {
    InputState remapped_inputs = inputs;
    HandleRemap(inputs, remapped_inputs);
    HandleSocd(remapped_inputs);
    UpdateKeys(inputs);
    _keyboard->sendState();
}

void KeyboardMode::Press(uint8_t keycode, bool press) {
    _keyboard->setPressed(keycode, press);
}