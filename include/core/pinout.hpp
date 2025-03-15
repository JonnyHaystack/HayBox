#ifndef _CORE_PINOUT_HPP
#define _CORE_PINOUT_HPP

#include "core/state.hpp"

typedef struct {
    uint8_t joybus_data;
    int nes_data;
    int nes_clock;
    int nes_latch;
    int mux;
    int nunchuk_detect;
    int nunchuk_sda;
    int nunchuk_scl;
} Pinout;

#endif