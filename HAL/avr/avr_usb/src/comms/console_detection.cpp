#include "comms/console_detection.hpp"

#include "comms/DInputBackend.hpp"
#include "core/pinout.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

CommunicationBackendId detect_console(const Pinout &pinout) {
    delay(500);
    bool usb_connected = UDADDR & _BV(ADDEN);

    if (usb_connected) {
        return COMMS_BACKEND_DINPUT;
    }

    return COMMS_BACKEND_GAMECUBE;
}