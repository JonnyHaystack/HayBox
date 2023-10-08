#include "reboot.hpp"

#include "stdlib.hpp"

void (*resetFunc)(void) = 0;

void reboot_firmware() {
    resetFunc();
}

void reboot_bootloader() {
    while (true) {
        delay(1);
    }
}