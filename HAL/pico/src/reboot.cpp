#include "reboot.hpp"

#include "stdlib.hpp"

void reboot_firmware() {
    rp2040.reboot();
}

void reboot_bootloader() {
    rp2040.rebootToBootloader();
}