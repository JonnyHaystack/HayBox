#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

#include <Keyboard.h>

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {
    Keyboard.begin();
}

KeyboardMode::~KeyboardMode() {
    Keyboard.releaseAll();
    Keyboard.end();
}

void KeyboardMode::SendReport(InputState &inputs) {
    HandleSocd(inputs);
    SendKeys(inputs);
}

void KeyboardMode::Press(uint8_t key, bool press) {
    if (press)
        Keyboard.press(key);
    else
        Keyboard.release(key);
}