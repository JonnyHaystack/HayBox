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
    bool read_digital(uint pin);
    void write_digital(uint pin, bool value);
}

#endif