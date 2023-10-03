#include "modes/extra/ToughLoveArena.hpp"

ToughLoveArena::ToughLoveArena(GameModeConfig &config) : KeyboardMode(config) {}

void ToughLoveArena::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_S, inputs.left);
    Press(HID_KEY_D, inputs.right);
    Press(HID_KEY_J, inputs.b);
    Press(HID_KEY_K, inputs.x);
    Press(HID_KEY_L, inputs.z);
}
