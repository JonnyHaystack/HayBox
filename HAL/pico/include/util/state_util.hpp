#ifndef _UTIL_STATE_UTIL_HPP
#define _UTIL_STATE_UTIL_HPP

#include "stdlib.hpp"

#include <config.pb.h>

/* InputState utils */

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

inline bool any_button_held(const uint64_t &buttons, uint64_t button_mask) {
    return button_mask != 0 && (buttons & button_mask);
}

/* OutputState utils */

inline void set_output(uint32_t &buttons, DigitalOutput output_index, bool pressed) {
    if (output_index == GP_UNSPECIFIED) {
        return;
    }
    DigitalOutput output_index_adjusted = (DigitalOutput)(output_index - 1);
    buttons =
        (buttons & ~(1UL << output_index_adjusted)) | ((uint32_t)pressed << output_index_adjusted);
}

inline uint8_t OutputState::*axis_pointer(AnalogAxis axis) {
    switch (axis) {
        case AXIS_LSTICK_X:
            return &OutputState::leftStickX;
        case AXIS_LSTICK_Y:
            return &OutputState::leftStickY;
        case AXIS_RSTICK_X:
            return &OutputState::rightStickX;
        case AXIS_RSTICK_Y:
            return &OutputState::rightStickY;
        case AXIS_LTRIGGER:
            return &OutputState::triggerLAnalog;
        case AXIS_RTRIGGER:
            return &OutputState::triggerRAnalog;
        default:
            return nullptr;
    }
}

constexpr const char *digital_output_name(DigitalOutput output) {
    switch (output) {
        case GP_A:
            return "A";
        case GP_B:
            return "B";
        case GP_X:
            return "X";
        case GP_Y:
            return "Y";
        case GP_LB:
            return "L1";
        case GP_RB:
            return "R1";
        case GP_LT:
            return "L2";
        case GP_RT:
            return "R2";
        case GP_START:
            return "Start";
        case GP_SELECT:
            return "Select";
        case GP_HOME:
            return "Home";
        case GP_CAPTURE:
            return "Capture";
        case GP_DPAD_UP:
            return "D-Pad Up";
        case GP_DPAD_DOWN:
            return "D-Pad Down";
        case GP_DPAD_LEFT:
            return "D-Pad Left";
        case GP_DPAD_RIGHT:
            return "D-Pad Right";
        case GP_LSTICK_CLICK:
            return "L3";
        case GP_RSTICK_CLICK:
            return "R3";
        default:
            return "Unknown";
    }
}

#endif