#include <Keyboard.h>

#include "InputMode.h"
#include "KeyboardMode.h"

KeyboardMode::KeyboardMode(socd::SocdType socdType,
                           state::InputState &rInputState)
    : InputMode(socdType, rInputState) {
  Keyboard.begin();
}

KeyboardMode::~KeyboardMode() {
    Keyboard.releaseAll();
    Keyboard.end();
}

void KeyboardMode::UpdateOutputs() {
  HandleSocd();
  SendKeys();
}

void KeyboardMode::Press(char key, bool press) {
    if (press)
        Keyboard.press(key);
    else
        Keyboard.release(key);
}
