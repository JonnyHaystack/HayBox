#ifndef _CORE_PINOUT_HPP
#define _CORE_PINOUT_HPP

#include "core/state.hpp"

typedef struct {
    uint8_t joybus_data;
    uint8_t nes_data;
    uint8_t nes_clock;
    uint8_t nes_latch;
    int mux;
    int nunchuk_detect;
    int nunchuk_sda;
    int nunchuk_scl;
} Pinout;

#endif