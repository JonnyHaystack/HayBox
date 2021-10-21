#ifndef NOUSB
#include <Keyboard.h>
#endif

#include "InputMode.h"
#include "KeyboardMode.h"

KeyboardMode::KeyboardMode(socd::SocdType socdType,
                           state::InputState &rInputState)
    : InputMode(socdType, rInputState) {
#ifndef NOUSB
  Keyboard.begin();
#endif
}

KeyboardMode::~KeyboardMode() {
#ifndef NOUSB
    Keyboard.releaseAll();
    Keyboard.end();
#endif
}

void KeyboardMode::UpdateOutputs() {
  HandleSocd();
  SendKeys();
}

void KeyboardMode::Press(char key, bool press) {
#ifndef NOUSB
    if (press)
        Keyboard.press(key);
    else
        Keyboard.release(key);
#endif
}