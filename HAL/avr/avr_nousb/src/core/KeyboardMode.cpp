#include "core/NoopKeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode(socd::SocdType socd_type) : InputMode(socd_type) {}

KeyboardMode::~KeyboardMode() {}

void KeyboardMode::SendReport() {}

void KeyboardMode::Press(char key, bool press) {}