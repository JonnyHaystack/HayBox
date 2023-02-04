#include "joybus_utils.hpp"

#include <GamecubeConsole.hpp>
#include <N64Console.hpp>
#include <pico/cyw43_arch.h>

#ifdef ARDUINO_RASPBERRY_PI_PICO_W
#define VBUS_SENSE_PIN 34
#else
#define VBUS_SENSE_PIN 24
#endif

ConnectedConsole detect_console(uint joybus_pin) {
    pinMode(VBUS_SENSE_PIN, INPUT);
    bool vbus_powered = digitalRead(VBUS_SENSE_PIN);

    if (GamecubeConsole(joybus_pin).Detect()) {
        return ConnectedConsole::GAMECUBE;
    }
    // 5V is not connected when plugged into N64 so we check that first to save time.
    // if (!vbus_powered && N64Console(joybus_pin).Detect()) {
    //     return ConnectedConsole::N64;
    // }
}
