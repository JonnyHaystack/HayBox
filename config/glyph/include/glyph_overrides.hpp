#include "comms/B0XXInputViewer.hpp"
#include "comms/NeoPixelBackend.hpp"
#include "core/config_utils.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

#define LED_PIN 7
#define LED_COUNT 72

// clang-format off

const Button pixel_to_button_mappings[LED_COUNT] = {
    BTN_MB1, BTN_MB1,
    BTN_LF4, BTN_LF4,
    BTN_LF3, BTN_LF3,
    BTN_LF5, BTN_LF5,
    BTN_LF2, BTN_LF2,
    BTN_LF1, BTN_LF1,
    BTN_LF6, BTN_LF6,
    BTN_LF7, BTN_LF7,
    BTN_LF8, BTN_LF8,
    BTN_RF12, BTN_RF12,
    BTN_RF13, BTN_RF13,
    BTN_RF14, BTN_RF14,
    BTN_RF5, BTN_RF5,
    BTN_RF6, BTN_RF6,
    BTN_RF7, BTN_RF7,
    BTN_RF8, BTN_RF8,
    BTN_RF4, BTN_RF4,
    BTN_RF3, BTN_RF3,
    BTN_RF2, BTN_RF2,
    BTN_RF1, BTN_RF1,
    BTN_RF11, BTN_RF11,
    BTN_RF10, BTN_RF10,
    BTN_RF9, BTN_RF9,
    BTN_RT3, BTN_RT3,
    BTN_RT4, BTN_RT4,
    BTN_RT5, BTN_RT5,
    BTN_RT1, BTN_RT1,
    BTN_RT1, BTN_RT1,
    BTN_RT2, BTN_RT2,
    BTN_LT6, BTN_LT6,
    BTN_LT3, BTN_LT3,
    BTN_LT4, BTN_LT4,
    BTN_LT5, BTN_LT5,
    BTN_LT1, BTN_LT1,
    BTN_LT1, BTN_LT1,
    BTN_LT2, BTN_LT2,
};

// clang-format on

void usb_backend_from_4pos_switch(
    CommunicationBackendConfig &backend_config,
    const Config &config
) {
    const uint pin2 = 8;
    const uint pin3 = 9;
    const uint pin4 = 10;

    pinMode(pin2, INPUT_PULLUP);
    pinMode(pin3, INPUT_PULLUP);
    pinMode(pin4, INPUT_PULLUP);

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

size_t init_secondary_backends_glyph(
    CommunicationBackend **&backends,
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
) {
    size_t backend_count = init_secondary_backends_default(
        backends,
        primary_backend,
        backend_id,
        inputs,
        input_sources,
        input_source_count,
        config,
        pinout
    );

    // Create new array containing all old backends but with length increased by 1 to make space for
    // NeoPixel backend.
    CommunicationBackend **new_backends = new CommunicationBackend *[backend_count + 1];
    for (size_t i = 0; i < backend_count; i++) {
        new_backends[i] = backends[i];
    }

    // Add new backend to array and increase backend count to reflect this.
    new_backends[backend_count++] = new NeoPixelBackend(
        inputs,
        input_sources,
        input_source_count,
        pixel_to_button_mappings,
        config.rgb_configs,
        config.rgb_configs_count,
        LED_PIN,
        LED_COUNT
    );

    // Delete the old backends array and reassign it.
    delete[] backends;
    backends = new_backends;

    return backend_count;
}