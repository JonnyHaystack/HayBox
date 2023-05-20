#include "util/analog_filters.hpp"

#include <math.h>

#define SIGNUM(x) ((x > 0) - (x < 0))

uint8_t apply_deadzone(uint8_t value, uint8_t deadzone, bool scale) {
    int8_t value_signed = value - 128;
    if (abs(value_signed) > deadzone) {
        // If outside deadzone, must subtract deadzone from result so that axis values start from 1
        // instead of having lower values cut off.
        int8_t post_deadzone = value_signed - deadzone * SIGNUM(value_signed);
        // If a radius value is passed in, scale up the values linearly so that the same effective
        // value is given on the rim.
        if (scale) {
            int8_t sign = SIGNUM(post_deadzone);
            int8_t post_scaling = min(127, abs(post_deadzone) * 128.0 / (128 - deadzone)) * sign;
            return post_scaling + 128;
        }
        return post_deadzone + 128;
    }
    return 128;
}

uint8_t apply_radius(uint8_t value, int radius) {
    int8_t value_signed = value - 128;
    int8_t sign = SIGNUM(value_signed);
    return min(127, (int)(abs(value_signed) * radius / 128.0)) * sign + 128;
}