#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode() : InputMode() {}

KeyboardMode::~KeyboardMode() {}

void KeyboardMode::SendReport(const InputState &inputs) {}

void KeyboardMode::Press(uint8_t keycode, bool press) {}