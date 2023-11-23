#include "core/config_utils.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

void usb_backend_from_4pos_switch(
    CommunicationBackendConfig &backend_config,
    const Config &config
) {
    const uint pin2 = 8;
    const uint pin3 = 9;
    const uint pin4 = 10;
    const uint pin5 = 11;

    pinMode(pin2, INPUT_PULLUP);
    pinMode(pin3, INPUT_PULLUP);
    pinMode(pin4, INPUT_PULLUP);
    pinMode(pin5, INPUT_PULLUP);

    uint toggle_switch_state = 0;
    if (!digitalRead(pin3)) {
        toggle_switch_state = 1;
    } else if (!digitalRead(pin4)) {
        toggle_switch_state = 4;
    } else {
        pinMode(pin2, OUTPUT);
        digitalWrite(pin2, LOW);

        if (!digitalRead(pin4)) {
            toggle_switch_state = 2;
        } else {
            toggle_switch_state = 3;
        }
    }

    CommunicationBackendId backend_id = COMMS_BACKEND_UNSPECIFIED;

    switch (toggle_switch_state) {
        case 1:
            backend_id = COMMS_BACKEND_XINPUT;
            break;
        case 2:
            backend_id = COMMS_BACKEND_DINPUT;
            break;
        case 3:
            backend_id = COMMS_BACKEND_NINTENDO_SWITCH;
            break;
        case 4:
            break;
    }

    backend_config = backend_config_from_id(
        backend_id,
        config.communication_backend_configs,
        config.communication_backend_configs_count
    );
}
