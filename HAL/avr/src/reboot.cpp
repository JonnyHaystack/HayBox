#include "reboot.hpp"

#include "stdlib.hpp"

#include <avr/wdt.h>

void reboot_firmware() {
    wdt_enable(WDTO_15MS);
    while (true) {
        delay(1);
    }
}

void reboot_bootloader() {
    uint16_t *magic_key_pos = (uint16_t *)MAGIC_KEY_POS;
    *magic_key_pos = MAGIC_KEY;
    wdt_enable(WDTO_15MS);
    while (true) {
        delay(1);
    }
}