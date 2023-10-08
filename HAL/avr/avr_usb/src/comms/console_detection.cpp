#include "comms/console_detection.hpp"

#include "comms/DInputBackend.hpp"
#include "core/pinout.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

// extern const Pinout pinout;

/*
ConnectedConsole detect_console() {
    // InputState inputs;
    // CommunicationBackend *dinput = new DInputBackend(inputs, nullptr, 0);
    Serial.begin(115200);
    // delay(100);
    delay(500);
    bool usb_connected = UDADDR & _BV(ADDEN);
    // USBD_Connected();
    // Serial.end();
    // delete dinput;

    if (usb_connected) {
        return ConnectedConsole::NONE;
    }

    return ConnectedConsole::GAMECUBE;
}
*/

CommunicationBackendId detect_console(const Pinout &pinout) {
    bool usb_connected = UDADDR & _BV(ADDEN);

    if (usb_connected) {
        return COMMS_BACKEND_DINPUT;
    }

    return COMMS_BACKEND_GAMECUBE;
}