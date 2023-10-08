#include "core/KeyboardMode.hpp"

#include "core/InputMode.hpp"

KeyboardMode::KeyboardMode(const GameModeConfig &config) : InputMode(config) {}

KeyboardMode::~KeyboardMode() {}

void KeyboardMode::SendReport(InputState &inputs) {}

void KeyboardMode::Press(uint8_t keycode, bool press) {}