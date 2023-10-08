#ifndef _CORE_STATE_HPP
#define _CORE_STATE_HPP

#include "stdlib.hpp"

#include <config.pb.h>

// Button state.
// typedef union _ButtonState {
//     uint64_t buttons = 0;

// } ButtonState;

typedef struct _InputState {
    // Rectangle inputs.
    union {
        uint64_t buttons = 0;

        struct {
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