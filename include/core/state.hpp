#ifndef _CORE_STATE_HPP
#define _CORE_STATE_HPP

#include "stdlib.hpp"

#include <config.pb.h>

typedef struct _InputState {
    // Rectangle inputs.
    union {
        uint64_t buttons = 0;

        struct {
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
        };
    };

    // Nunchuk inputs.
    union {
        uint8_t nunchuk_buttons = 0;

        struct {
            bool nunchuk_connected : 1;
            bool nunchuk_c : 1;
            bool nunchuk_z : 1;
        };
    };

    int8_t nunchuk_x = 0;
    int8_t nunchuk_y = 0;
} InputState;

// State describing stick direction at the quadrant level.
typedef struct {
    bool horizontal;
    bool vertical;
    bool diagonal;
    int8_t x;
    int8_t y;
    int8_t cx;
    int8_t cy;
} StickDirections;

// Output state.
typedef struct outputstate {
    // Digital outputs.
    bool a = false;
    bool b = false;
    bool x = false;
    bool y = false;
    bool buttonL = false;
    bool buttonR = false;
    bool triggerLDigital = false;
    bool triggerRDigital = false;
    bool start = false;
    bool select = false;
    bool home = false;
    bool dpadUp = false;
    bool dpadDown = false;
    bool dpadLeft = false;
    bool dpadRight = false;
    bool leftStickClick = false;
    bool rightStickClick = false;

    // Analog outputs.
    uint8_t leftStickX = 128;
    uint8_t leftStickY = 128;
    uint8_t rightStickX = 128;
    uint8_t rightStickY = 128;
    uint8_t triggerRAnalog = 0;
    uint8_t triggerLAnalog = 0;
} OutputState;

#endif