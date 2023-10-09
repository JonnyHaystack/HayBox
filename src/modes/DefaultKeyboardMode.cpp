#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(const GameModeConfig &config) : KeyboardMode(config) {}

void DefaultKeyboardMode::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_A, inputs.lf4);
    Press(HID_KEY_B, inputs.lf3);
    Press(HID_KEY_C, inputs.lf2);
    Press(HID_KEY_D, inputs.lf1);
    Press(HID_KEY_E, inputs.lt1);
    Press(HID_KEY_F, inputs.lt2);
    Press(HID_KEY_G, inputs.mb3);
    Press(HID_KEY_H, inputs.mb1);
    Press(HID_KEY_I, inputs.mb2);
    Press(HID_KEY_J, inputs.rf5);
    Press(HID_KEY_K, inputs.rf6);
    Press(HID_KEY_L, inputs.rf7);
    Press(HID_KEY_M, inputs.rf8);
    Press(HID_KEY_N, inputs.rf1);
    Press(HID_KEY_O, inputs.rf2);
    Press(HID_KEY_P, inputs.rf3);
    Press(HID_KEY_Q, inputs.rf4);
    Press(HID_KEY_R, inputs.rt4);
    Press(HID_KEY_S, inputs.rt3);
    Press(HID_KEY_T, inputs.rt5);
    Press(HID_KEY_U, inputs.rt1);
    Press(HID_KEY_V, inputs.rt2);
}
