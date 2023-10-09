#include "modes/extra/ToughLoveArena.hpp"

ToughLoveArena::ToughLoveArena(const GameModeConfig &config) : KeyboardMode(config) {}

void ToughLoveArena::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_S, inputs.lf3);
    Press(HID_KEY_D, inputs.lf1);
    Press(HID_KEY_J, inputs.rf1);
    Press(HID_KEY_K, inputs.rf2);
    Press(HID_KEY_L, inputs.rf3);
}
