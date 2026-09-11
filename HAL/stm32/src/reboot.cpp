#include "reboot.hpp"

#include "stdlib.hpp"

#include <libmaple/nvic.h>

void reboot_firmware() {
    nvic_sys_reset();
}

void reboot_bootloader() {
    // Write magic to top of RAM to signal DFU bootloader entry on reset.
    // Linker reserves last 8 bytes (LENGTH = 96K - 8), so __msp_init points there.
    // Compatible with davidgfnet/stm32-dfu-bootloader reboot protocol.
    extern uint32_t __msp_init;
    volatile uint64_t *magic = (volatile uint64_t *)&__msp_init;
    *magic = 0xDEADBEEFCC00FFEEULL;
    nvic_sys_reset();
}
