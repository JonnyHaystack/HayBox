#ifndef _DEBOUNCE_HPP
#define _DEBOUNCE_HPP

#include "stdlib.hpp"

typedef struct _DebounceState {
    absolute_time_t locked_until = 0;
    bool pressed = false;
} DebounceState;

inline bool update_debounce_state(
    DebounceState &debounce_state,
    bool current_reading,
    uint32_t debounce_period_ms
) {
    // If currently locked out, do nothing.
    if (!time_reached(debounce_state.locked_until)) {
        return false;
    }

    // If latest reading differs from the last known state, update the value and timeout.
    if (current_reading != debounce_state.pressed) {
        debounce_state.pressed = current_reading;
        debounce_state.locked_until = make_timeout_time_ms(debounce_period_ms);
        return true;
    }

    return false;
}

#endif