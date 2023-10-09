#ifndef _UTIL_STATE_UTIL_HPP
#define _UTIL_STATE_UTIL_HPP

#include "core/state.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

typedef struct _ButtonState {
    bool lf1 : 1;
    bool lf2 : 1;
    bool lf3 : 1;
    bool lf4 : 1;
    bool lf5 : 1;
    bool lf6 : 1;
    bool lf7 : 1;
    bool lf8 : 1;
    bool lf9 : 1;
    bool lf10 : 1;
    bool lf11 : 1;
    bool lf12 : 1;
    bool lf13 : 1;
    bool lf14 : 1;
    bool lf15 : 1;
    bool lf16 : 1;
    bool rf1 : 1;
    bool rf2 : 1;
    bool rf3 : 1;
    bool rf4 : 1;
    bool rf5 : 1;
    bool rf6 : 1;
    bool rf7 : 1;
    bool rf8 : 1;
    bool rf9 : 1;
    bool rf10 : 1;
    bool rf11 : 1;
    bool rf12 : 1;
    bool rf13 : 1;
    bool rf14 : 1;
    bool rf15 : 1;
    bool rf16 : 1;
    bool lt1 : 1;
    bool lt2 : 1;
    bool lt3 : 1;
    bool lt4 : 1;
    bool lt5 : 1;
    bool lt6 : 1;
    bool lt7 : 1;
    bool lt8 : 1;
    bool rt1 : 1;
    bool rt2 : 1;
    bool rt3 : 1;
    bool rt4 : 1;
    bool rt5 : 1;
    bool rt6 : 1;
    bool rt7 : 1;
    bool rt8 : 1;
    bool mb1 : 1;
    bool mb2 : 1;
    bool mb3 : 1;
    bool mb4 : 1;
    bool mb5 : 1;
    bool mb6 : 1;
    bool mb7 : 1;
    bool mb8 : 1;
    bool mb9 : 1;
    bool mb10 : 1;
    bool mb11 : 1;
    bool mb12 : 1;
} ButtonState;

inline void set_button(uint64_t &buttons, Button button_index, bool pressed) {
    ButtonState &inputs = (ButtonState &)buttons;
    if (button_index == BTN_UNSPECIFIED) {
        return;
    }
    switch (button_index) {
        case BTN_LF1:
            inputs.lf1 = pressed;
            break;
        case BTN_LF2:
            inputs.lf2 = pressed;
            break;
        case BTN_LF3:
            inputs.lf3 = pressed;
            break;
        case BTN_LF4:
            inputs.lf4 = pressed;
            break;
        case BTN_LF5:
            inputs.lf5 = pressed;
            break;
        case BTN_LF6:
            inputs.lf6 = pressed;
            break;
        case BTN_LF7:
            inputs.lf7 = pressed;
            break;
        case BTN_LF8:
            inputs.lf8 = pressed;
            break;
        case BTN_LF9:
            inputs.lf9 = pressed;
            break;
        case BTN_LF10:
            inputs.lf10 = pressed;
            break;
        case BTN_LF11:
            inputs.lf11 = pressed;
            break;
        case BTN_LF12:
            inputs.lf12 = pressed;
            break;
        case BTN_LF13:
            inputs.lf13 = pressed;
            break;
        case BTN_LF14:
            inputs.lf14 = pressed;
            break;
        case BTN_LF15:
            inputs.lf15 = pressed;
            break;
        case BTN_LF16:
            inputs.lf16 = pressed;
            break;
        case BTN_RF1:
            inputs.rf1 = pressed;
            break;
        case BTN_RF2:
            inputs.rf2 = pressed;
            break;
        case BTN_RF3:
            inputs.rf3 = pressed;
            break;
        case BTN_RF4:
            inputs.rf4 = pressed;
            break;
        case BTN_RF5:
            inputs.rf5 = pressed;
            break;
        case BTN_RF6:
            inputs.rf6 = pressed;
            break;
        case BTN_RF7:
            inputs.rf7 = pressed;
            break;
        case BTN_RF8:
            inputs.rf8 = pressed;
            break;
        case BTN_RF9:
            inputs.rf9 = pressed;
            break;
        case BTN_RF10:
            inputs.rf10 = pressed;
            break;
        case BTN_RF11:
            inputs.rf11 = pressed;
            break;
        case BTN_RF12:
            inputs.rf12 = pressed;
            break;
        case BTN_RF13:
            inputs.rf13 = pressed;
            break;
        case BTN_RF14:
            inputs.rf14 = pressed;
            break;
        case BTN_RF15:
            inputs.rf15 = pressed;
            break;
        case BTN_RF16:
            inputs.rf16 = pressed;
            break;
        case BTN_LT1:
            inputs.lt1 = pressed;
            break;
        case BTN_LT2:
            inputs.lt2 = pressed;
            break;
        case BTN_LT3:
            inputs.lt3 = pressed;
            break;
        case BTN_LT4:
            inputs.lt4 = pressed;
            break;
        case BTN_LT5:
            inputs.lt5 = pressed;
            break;
        case BTN_LT6:
            inputs.lt6 = pressed;
            break;
        case BTN_LT7:
            inputs.lt7 = pressed;
            break;
        case BTN_LT8:
            inputs.lt8 = pressed;
            break;
        case BTN_RT1:
            inputs.rt1 = pressed;
            break;
        case BTN_RT2:
            inputs.rt2 = pressed;
            break;
        case BTN_RT3:
            inputs.rt3 = pressed;
            break;
        case BTN_RT4:
            inputs.rt4 = pressed;
            break;
        case BTN_RT5:
            inputs.rt5 = pressed;
            break;
        case BTN_RT6:
            inputs.rt6 = pressed;
            break;
        case BTN_RT7:
            inputs.rt7 = pressed;
            break;
        case BTN_RT8:
            inputs.rt8 = pressed;
            break;
        case BTN_MB1:
            inputs.mb1 = pressed;
            break;
        case BTN_MB2:
            inputs.mb2 = pressed;
            break;
        case BTN_MB3:
            inputs.mb3 = pressed;
            break;
        case BTN_MB4:
            inputs.mb4 = pressed;
            break;
        case BTN_MB5:
            inputs.mb5 = pressed;
            break;
        case BTN_MB6:
            inputs.mb6 = pressed;
            break;
        case BTN_MB7:
            inputs.mb7 = pressed;
            break;
        case BTN_MB8:
            inputs.mb8 = pressed;
            break;
        case BTN_MB9:
            inputs.mb9 = pressed;
            break;
        case BTN_MB10:
            inputs.mb10 = pressed;
            break;
        case BTN_MB11:
            inputs.mb11 = pressed;
            break;
        case BTN_MB12:
            inputs.mb12 = pressed;
            break;
        default:
            break;
    }
}

inline bool get_button(const uint64_t &buttons, Button button_index) {
    ButtonState &inputs = (ButtonState &)buttons;
    switch (button_index) {
        case BTN_LF1:
            return inputs.lf1;
        case BTN_LF2:
            return inputs.lf2;
        case BTN_LF3:
            return inputs.lf3;
        case BTN_LF4:
            return inputs.lf4;
        case BTN_LF5:
            return inputs.lf5;
        case BTN_LF6:
            return inputs.lf6;
        case BTN_LF7:
            return inputs.lf7;
        case BTN_LF8:
            return inputs.lf8;
        case BTN_LF9:
            return inputs.lf9;
        case BTN_LF10:
            return inputs.lf10;
        case BTN_LF11:
            return inputs.lf11;
        case BTN_LF12:
            return inputs.lf12;
        case BTN_LF13:
            return inputs.lf13;
        case BTN_LF14:
            return inputs.lf14;
        case BTN_LF15:
            return inputs.lf15;
        case BTN_LF16:
            return inputs.lf16;
        case BTN_RF1:
            return inputs.rf1;
        case BTN_RF2:
            return inputs.rf2;
        case BTN_RF3:
            return inputs.rf3;
        case BTN_RF4:
            return inputs.rf4;
        case BTN_RF5:
            return inputs.rf5;
        case BTN_RF6:
            return inputs.rf6;
        case BTN_RF7:
            return inputs.rf7;
        case BTN_RF8:
            return inputs.rf8;
        case BTN_RF9:
            return inputs.rf9;
        case BTN_RF10:
            return inputs.rf10;
        case BTN_RF11:
            return inputs.rf11;
        case BTN_RF12:
            return inputs.rf12;
        case BTN_RF13:
            return inputs.rf13;
        case BTN_RF14:
            return inputs.rf14;
        case BTN_RF15:
            return inputs.rf15;
        case BTN_RF16:
            return inputs.rf16;
        case BTN_LT1:
            return inputs.lt1;
        case BTN_LT2:
            return inputs.lt2;
        case BTN_LT3:
            return inputs.lt3;
        case BTN_LT4:
            return inputs.lt4;
        case BTN_LT5:
            return inputs.lt5;
        case BTN_LT6:
            return inputs.lt6;
        case BTN_LT7:
            return inputs.lt7;
        case BTN_LT8:
            return inputs.lt8;
        case BTN_RT1:
            return inputs.rt1;
        case BTN_RT2:
            return inputs.rt2;
        case BTN_RT3:
            return inputs.rt3;
        case BTN_RT4:
            return inputs.rt4;
        case BTN_RT5:
            return inputs.rt5;
        case BTN_RT6:
            return inputs.rt6;
        case BTN_RT7:
            return inputs.rt7;
        case BTN_RT8:
            return inputs.rt8;
        case BTN_MB1:
            return inputs.mb1;
        case BTN_MB2:
            return inputs.mb2;
        case BTN_MB3:
            return inputs.mb3;
        case BTN_MB4:
            return inputs.mb4;
        case BTN_MB5:
            return inputs.mb5;
        case BTN_MB6:
            return inputs.mb6;
        case BTN_MB7:
            return inputs.mb7;
        case BTN_MB8:
            return inputs.mb8;
        case BTN_MB9:
            return inputs.mb9;
        case BTN_MB10:
            return inputs.mb10;
        case BTN_MB11:
            return inputs.mb11;
        case BTN_MB12:
            return inputs.mb12;
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