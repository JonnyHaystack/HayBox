#include "gpio.hpp"

#include "stdlib.hpp"

namespace gpio {
    void init_pin(uint pin, GpioMode mode) {
        gpio_init(pin);
        if (mode == GpioMode::GPIO_OUTPUT) {
            gpio_set_dir(pin, GPIO_OUT);
            return;
        }

        gpio_set_dir(pin, GPIO_IN);
        if (mode == GpioMode::GPIO_INPUT_PULLUP) {
            gpio_pull_up(pin);
        } else if (mode == GpioMode::GPIO_INPUT_PULLDOWN) {
            gpio_pull_down(pin);
        }
    }

    bool read_digital(uint pin) {
        return gpio_get(pin);
    }

    void write_digital(uint pin, bool value) {
        gpio_put(pin, value);
    }
}