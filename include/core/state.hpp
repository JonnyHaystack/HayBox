#ifndef _CORE_STATE_HPP
#define _CORE_STATE_HPP

#include "stdlib.hpp"

#define ANALOG_STICK_NEUTRAL 128

// Button state.
typedef struct inputstate {
    // Rectangle inputs.
    bool start = false;
    bool select = false;
    bool home = false;
    bool y = false;
    bool x = false;
    bool b = false;
    bool a = false;
    bool l = false;
    bool r = false;
    bool z = false;
    bool lightshield = false;
    bool midshield = false;
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
    bool mod_x = false;
    bool mod_y = false;
    bool c_left = false;
    bool c_right = false;
    bool c_up = false;
    bool c_down = false;

    // Nunchuk inputs.
    bool nunchuk_connected = false;
    int8_t nunchuk_x = 0;
    int8_t nunchuk_y = 0;
    bool nunchuk_c = false;
    bool nunchuk_z = false;
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
    uint8_t leftStickX = ANALOG_STICK_NEUTRAL;
    uint8_t leftStickY = ANALOG_STICK_NEUTRAL;
    uint8_t rightStickX = ANALOG_STICK_NEUTRAL;
    uint8_t rightStickY = ANALOG_STICK_NEUTRAL;
    uint8_t triggerRAnalog = 0;
    uint8_t triggerLAnalog = 0;
} OutputState;

#endif