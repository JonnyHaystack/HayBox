#include "gpio.hpp"

#include "stdlib.hpp"

// TODO: Might be better to replace with port manipulation
namespace gpio {
    void init_pin(uint pin, GpioMode mode) {
        if (mode == GpioMode::GPIO_OUTPUT) {
            pinMode(pin, OUTPUT);
        } else if (mode == GpioMode::GPIO_INPUT_PULLUP) {
            pinMode(pin, INPUT_PULLUP);
        } else if (mode == GpioMode::GPIO_INPUT) {
            pinMode(pin, INPUT);
        }
    }
}