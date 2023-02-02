#include "joybus_utils.hpp"

#include <GamecubeConsole.hpp>
#include <N64Console.hpp>

#define VBUS_SENSE_PIN 24

ConnectedConsole detect_console(uint joybus_pin) {
    gpio_init(VBUS_SENSE_PIN);
    gpio_set_dir(VBUS_SENSE_PIN, GPIO_IN);
    bool vbus_powered = gpio_get(VBUS_SENSE_PIN);

    if (GamecubeConsole(joybus_pin).Detect()) {
        return ConnectedConsole::GAMECUBE;
    }
    // 5V is not connected when plugged into N64 so we check that first to save time.
    if (!vbus_powered && N64Console(joybus_pin).Detect()) {
        return ConnectedConsole::N64;
    }

    return ConnectedConsole::NONE;
}