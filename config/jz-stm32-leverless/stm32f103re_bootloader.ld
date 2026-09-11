/*
 * Linker script for STM32F103RF with 4KB DFU bootloader.
 * App starts at 0x08001000 (4KB offset from flash start).
 * RAM: 96KB, last 8 bytes reserved for bootloader reboot magic.
 */
MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 96K - 8
  rom (rx)  : ORIGIN = 0x08001000, LENGTH = 508K
}

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
