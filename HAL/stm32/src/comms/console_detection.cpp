#include "comms/console_detection.hpp"

#include "core/pinout.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

CommunicationBackendId detect_console(const Pinout &pinout) {
    // STM32 is always USB — return XInput.
    return COMMS_BACKEND_XINPUT;
}
