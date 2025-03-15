#include "comms/console_detection.hpp"

#include "core/pinout.hpp"

#include <config.pb.h>

CommunicationBackendId detect_console(const Pinout &pinout) {
    return COMMS_BACKEND_GAMECUBE;
}
