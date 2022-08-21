#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socd_type) : KeyboardMode(socd_type) {}

void DefaultKeyboardMode::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_B, inputs.l);
    Press(HID_KEY_A, inputs.left);
    Press(HID_KEY_S, inputs.down);
    Press(HID_KEY_D, inputs.right);
    Press(HID_KEY_W, inputs.mod_x);
    Press(HID_KEY_I, inputs.mod_y);
    Press(HID_KEY_G, inputs.select);
    Press(HID_KEY_ENTER, inputs.start);
    Press(HID_KEY_F, inputs.home);
    Press(HID_KEY_Y, inputs.r);
    Press(HID_KEY_K, inputs.y);
    Press(HID_KEY_L, inputs.lightshield);
    Press(HID_KEY_M, inputs.midshield);
    Press(HID_KEY_U, inputs.b);
    Press(HID_KEY_O, inputs.x);
    Press(HID_KEY_P, inputs.z);
    Press(HID_KEY_Q, inputs.up);
    Press(HID_KEY_R, inputs.c_up);
    Press(HID_KEY_Z, inputs.c_left);
    Press(HID_KEY_T, inputs.c_right);
    Press(HID_KEY_N, inputs.a);
    Press(HID_KEY_V, inputs.c_down);
    Press(HID_KEY_C, inputs.nunchuk_c);
}
