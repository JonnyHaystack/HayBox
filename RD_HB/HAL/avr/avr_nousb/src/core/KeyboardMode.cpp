#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {}

KeyboardMode::~KeyboardMode() {}

void KeyboardMode::SendReport(InputState &inputs) {}

void KeyboardMode::Press(uint8_t keycode, bool press) {}