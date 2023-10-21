#include "comms/console_detection.hpp"

#include "core/pinout.hpp"

#include <GamecubeConsole.hpp>
#include <N64Console.hpp>
#include <config.pb.h>
#include <hardware/structs/usb.h>

CommunicationBackendId detect_console(const Pinout &pinout) {
    delay(500);
    bool usb_connected = usb_hw->sie_status & USB_SIE_STATUS_CONNECTED_BITS;

    if (usb_connected) {
        return COMMS_BACKEND_XINPUT;
    }
    if (GamecubeConsole(pinout.joybus_data).Detect()) {
        return COMMS_BACKEND_GAMECUBE;
    }
    if (N64Console(pinout.joybus_data).Detect()) {
        return COMMS_BACKEND_N64;
    }
    return COMMS_BACKEND_UNSPECIFIED;
}