#ifndef _JOYBUS_UTILS_HPP
#define _JOYBUS_UTILS_HPP

#include <stdlib.hpp>

enum class ConnectedConsole {
    GAMECUBE,
    N64,
    SWITCH,
    NONE,
};

ConnectedConsole detect_console(uint joybus_pin);

#endif
