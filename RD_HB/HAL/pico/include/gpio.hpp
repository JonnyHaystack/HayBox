#ifndef _GPIO_HPP
#define _GPIO_HPP

#include "stdlib.hpp"

namespace gpio {
    enum class GpioMode {
        GPIO_OUTPUT,
        GPIO_INPUT,
        GPIO_INPUT_PULLUP,
        GPIO_INPUT_PULLDOWN,
    };

    void init_pin(uint pin, GpioMode mode);

    inline bool read_digital(uint pin) {
        return gpio_get(pin);
    }

    inline void write_digital(uint pin, bool value) {
        gpio_put(pin, value);
    }
}

#endif