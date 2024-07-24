#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socd_type) {}

void DefaultKeyboardMode::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_A, inputs.l);
    Press(HID_KEY_B, inputs.up2);
    Press(HID_KEY_C, inputs.left);
    Press(HID_KEY_D, inputs.down);
    Press(HID_KEY_E, inputs.right);
    Press(HID_KEY_F, inputs.mod_x);
    Press(HID_KEY_G, inputs.mod_y);
    Press(HID_KEY_H, inputs.select);
    Press(HID_KEY_I, inputs.home);
    Press(HID_KEY_J, inputs.start);
    Press(HID_KEY_K, inputs.r);
    Press(HID_KEY_L, inputs.y);
    Press(HID_KEY_M, inputs.lightshield);
    Press(HID_KEY_N, inputs.midshield);
    Press(HID_KEY_O, inputs.b);
    Press(HID_KEY_P, inputs.x);
    Press(HID_KEY_Q, inputs.z);
    Press(HID_KEY_R, inputs.up);
    Press(HID_KEY_S, inputs.c_up);
    Press(HID_KEY_T, inputs.c_left);
    Press(HID_KEY_U, inputs.c_right);
    Press(HID_KEY_V, inputs.a);
    Press(HID_KEY_X, inputs.c_down);
}
