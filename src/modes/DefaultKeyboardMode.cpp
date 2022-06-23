#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socd_type) : KeyboardMode(socd_type) {}

void DefaultKeyboardMode::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_Q, inputs.l);
    Press(HID_KEY_A, inputs.left);
    Press(HID_KEY_S, inputs.down);
    Press(HID_KEY_D, inputs.right);
    Press(HID_KEY_W, inputs.mod_x);
    Press(HID_KEY_E, inputs.mod_y);
    Press(HID_KEY_H, inputs.start);
    Press(HID_KEY_U, inputs.r);
    Press(HID_KEY_I, inputs.y);
    Press(HID_KEY_O, inputs.lightshield);
    Press(HID_KEY_P, inputs.midshield);
    Press(HID_KEY_J, inputs.b);
    Press(HID_KEY_K, inputs.x);
    Press(HID_KEY_L, inputs.z);
    Press(HID_KEY_Y, inputs.up);
    Press(HID_KEY_V, inputs.c_up);
    Press(HID_KEY_B, inputs.c_left);
    Press(HID_KEY_N, inputs.c_right);
    Press(HID_KEY_SPACE, inputs.a);
    Press(HID_KEY_M, inputs.c_down);
}
