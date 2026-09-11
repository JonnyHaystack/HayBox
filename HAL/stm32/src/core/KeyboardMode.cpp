#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode() : InputMode() {}

KeyboardMode::~KeyboardMode() {}

void KeyboardMode::SendReport(const InputState &inputs) {
    // Keyboard mode is not yet supported on STM32.
    // TODO: Implement using USBComposite HID keyboard.
}

void KeyboardMode::Press(uint8_t keycode, bool press) {
    // Stub — no keyboard support yet.
}
