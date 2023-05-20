#ifndef _UTIL_ANALOG_FILTERS_HPP
#define _UTIL_ANALOG_FILTERS_HPP

#include "stdlib.hpp"

uint8_t apply_deadzone(uint8_t value, uint8_t deadzone, bool scale);
uint8_t apply_radius(uint8_t value, int radius);

#endif