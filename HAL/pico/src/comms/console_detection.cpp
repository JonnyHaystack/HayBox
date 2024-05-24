#include "comms/console_detection.hpp"

#include "core/pinout.hpp"

#include <GamecubeConsole.hpp>
#include <N64Console.hpp>
#include <config.pb.h>
#include <hardware/structs/usb.h>

uint latch_pulses = 0;
uint clock_pulses = 0;

void latch_irq_handler() {
    latch_pulses++;
}

void clock_irq_handler() {
    clock_pulses++;
}

CommunicationBackendId detect_console(const Pinout &pinout) {
    if (pinout.nes_latch > -1 && pinout.nes_clock > -1) {
        attachInterrupt(pinout.nes_latch, &latch_irq_handler, PinStatus::RISING);
        attachInterrupt(pinout.nes_clock, &clock_irq_handler, PinStatus::FALLING);
    }

    delay(500);
    bool usb_connected = usb_hw->sie_status & USB_SIE_STATUS_CONNECTED_BITS;

    if (pinout.nes_latch > -1 && pinout.nes_clock > -1) {
        detachInterrupt(pinout.nes_latch);
        detachInterrupt(pinout.nes_clock);
    }

    CommunicationBackendId result = COMMS_BACKEND_UNSPECIFIED;

    if (usb_connected) {
        result = COMMS_BACKEND_XINPUT;
    } else if (latch_pulses && (clock_pulses / latch_pulses) > 8) {
        result = COMMS_BACKEND_SNES;
    } else if (latch_pulses && (clock_pulses / latch_pulses) > 1) {
        result = COMMS_BACKEND_NES;
    } else if (GamecubeConsole(pinout.joybus_data).Detect()) {
        result = COMMS_BACKEND_GAMECUBE;
    } else if (N64Console(pinout.joybus_data).Detect()) {
        result = COMMS_BACKEND_N64;
    }

    return result;
}