#ifndef _CORE_PINOUT_HPP
#define _CORE_PINOUT_HPP

#include "core/state.hpp"

typedef struct {
    uint8_t joybus_data;
    int mux;
    int nunchuk_detect;
    int nunchuk_sda;
    int nunchuk_scl;
} Pinout;

#endif