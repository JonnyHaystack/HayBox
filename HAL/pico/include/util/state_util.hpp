#ifndef _UTIL_STATE_UTIL_HPP
#define _UTIL_STATE_UTIL_HPP

#include "stdlib.hpp"

#include <config.pb.h>

inline void set_button(uint64_t &buttons, Button button_index, bool pressed) {
    if (button_index == BTN_UNSPECIFIED) {
        return;
    }
    Button button_index_adjusted = (Button)(button_index - 1);
    buttons =
        (buttons & ~(1ULL << button_index_adjusted)) | ((uint64_t)pressed << button_index_adjusted);
}

inline bool get_button(const uint64_t &buttons, Button button_index) {
    if (button_index == BTN_UNSPECIFIED) {
        return false;
    }
    return buttons & (1ULL << (button_index - 1));
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