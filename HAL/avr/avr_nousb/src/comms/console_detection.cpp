#include "comms/console_detection.hpp"

CommunicationBackendId detect_console(const Pinout &pinout) {
    return COMMS_BACKEND_GAMECUBE;
}
