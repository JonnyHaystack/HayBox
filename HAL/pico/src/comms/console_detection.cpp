#include "comms/console_detection.hpp"

#include "core/pinout.hpp"

#include <GamecubeConsole.hpp>
#include <N64Console.hpp>
#include <config.pb.h>

#define VBUS_SENSE_PIN 24

CommunicationBackendId detect_console(const Pinout &pinout) {
    gpio_init(VBUS_SENSE_PIN);
    gpio_set_dir(VBUS_SENSE_PIN, GPIO_IN);
    bool vbus_powered = gpio_get(VBUS_SENSE_PIN);

    if (GamecubeConsole(pinout.joybus_data).Detect()) {
        return COMMS_BACKEND_GAMECUBE;
    }
    // 5V is not connected when plugged into N64 so we check that first to save time.
    if (!vbus_powered && N64Console(pinout.joybus_data).Detect()) {
        return COMMS_BACKEND_N64;
    }

    return COMMS_BACKEND_XINPUT;
}