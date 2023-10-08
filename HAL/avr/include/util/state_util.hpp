#ifndef _UTIL_STATE_UTIL_HPP
#define _UTIL_STATE_UTIL_HPP

#include "core/state.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

typedef struct _ButtonState {
    bool left : 1;
    bool right : 1;
    bool down : 1;
    bool up : 1;
    bool c_left : 1;
    bool c_right : 1;
    bool c_down : 1;
    bool c_up : 1;
    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;
    bool l : 1;
    bool r : 1;
    bool z : 1;
    bool lightshield : 1;
    bool midshield : 1;
    bool select : 1;
    bool start : 1;
    bool home : 1;
    bool mod_x : 1;
    bool mod_y : 1;
} ButtonState;

inline void set_button(uint64_t &buttons, Button button_index, bool pressed) {
    ButtonState &inputs = (ButtonState &)buttons;
    if (button_index == BTN_UNSPECIFIED) {
        return;
    }
    switch (button_index) {
        case BTN_LEFT:
            inputs.left = pressed;
            break;
        case BTN_RIGHT:
            inputs.right = pressed;
            break;
        case BTN_DOWN:
            inputs.down = pressed;
            break;
        case BTN_UP:
            inputs.up = pressed;
            break;
        case BTN_C_LEFT:
            inputs.c_left = pressed;
            break;
        case BTN_C_RIGHT:
            inputs.c_right = pressed;
            break;
        case BTN_C_DOWN:
            inputs.c_down = pressed;
            break;
        case BTN_C_UP:
            inputs.c_up = pressed;
            break;
        case BTN_A:
            inputs.a = pressed;
            break;
        case BTN_B:
            inputs.b = pressed;
            break;
        case BTN_X:
            inputs.x = pressed;
            break;
        case BTN_Y:
            inputs.y = pressed;
            break;
        case BTN_L:
            inputs.l = pressed;
            break;
        case BTN_R:
            inputs.r = pressed;
            break;
        case BTN_Z:
            inputs.z = pressed;
            break;
        case BTN_LIGHTSHIELD:
            inputs.lightshield = pressed;
            break;
        case BTN_MIDSHIELD:
            inputs.midshield = pressed;
            break;
        case BTN_SELECT:
            inputs.select = pressed;
            break;
        case BTN_START:
            inputs.start = pressed;
            break;
        case BTN_HOME:
            inputs.home = pressed;
            break;
        case BTN_MOD_X:
            inputs.mod_x = pressed;
            break;
        case BTN_MOD_Y:
            inputs.mod_y = pressed;
            break;
        default:
            break;
    }
}

inline bool get_button(const uint64_t &buttons, Button button_index) {
    ButtonState &inputs = (ButtonState &)buttons;
    switch (button_index) {
        case BTN_LEFT:
            return inputs.left;
        case BTN_RIGHT:
            return inputs.right;
        case BTN_DOWN:
            return inputs.down;
        case BTN_UP:
            return inputs.up;
        case BTN_C_LEFT:
            return inputs.c_left;
        case BTN_C_RIGHT:
            return inputs.c_right;
        case BTN_C_DOWN:
            return inputs.c_down;
        case BTN_C_UP:
            return inputs.c_up;
        case BTN_A:
            return inputs.a;
        case BTN_B:
            return inputs.b;
        case BTN_X:
            return inputs.x;
        case BTN_Y:
            return inputs.y;
        case BTN_L:
            return inputs.l;
        case BTN_R:
            return inputs.r;
        case BTN_Z:
            return inputs.z;
        case BTN_LIGHTSHIELD:
            return inputs.lightshield;
        case BTN_MIDSHIELD:
            return inputs.midshield;
        case BTN_SELECT:
            return inputs.select;
        case BTN_START:
            return inputs.start;
        case BTN_HOME:
            return inputs.home;
        case BTN_MOD_X:
            return inputs.mod_x;
        case BTN_MOD_Y:
            return inputs.mod_y;
        default:
            return false;
    }
}

inline uint64_t make_button_mask(const Button *buttons, size_t buttons_count) {
    uint64_t button_mask = 0;
    for (size_t j = 0; j < buttons_count; j++) {
        button_mask |= (1ULL << (buttons[j] - 1));
    }
    return button_mask;
}

inline bool all_buttons_held(const uint64_t &buttons, uint64_t button_mask) {
    return button_mask != 0 && (buttons & button_mask) == button_mask;
}

#endif