#include "joybus_utils.hpp"

#include <GamecubeConsole.hpp>

ConnectedConsole detect_console(uint joybus_pin) {
    // TODO: Uncomment when N64 support is added
    // if (N64Console(joybus_pin).Detect()) {
    //     return ConnectedConsole::N64;
    // }
    if (GamecubeConsole(joybus_pin).Detect()) {
        return ConnectedConsole::GAMECUBE;
    }
    return ConnectedConsole::NONE;
}