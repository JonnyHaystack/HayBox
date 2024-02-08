#include "comms/B0XXInputViewer.hpp"
#include "comms/IntegratedDisplay.hpp"
#include "comms/NeoPixelBackend.hpp"
#include "core/config_utils.hpp"
#include "stdlib.hpp"

#include <Wire.h>
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

Adafruit_SSD1306 display(128, 64, &Wire1);

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
    // CommunicationBackend **new_backends = new CommunicationBackend *[backend_count + 1];
    CommunicationBackend **new_backends = new CommunicationBackend *[backend_count + 2];
    for (size_t i = 0; i < backend_count; i++) {
        new_backends[i] = backends[i];
    }

    // Add new backend to array and increase backend count to reflect this.
    new_backends[backend_count++] = new NeoPixelBackend<LED_PIN, LED_COUNT>(
        inputs,
        input_sources,
        input_source_count,
        pixel_to_button_mappings,
        config.rgb_configs,
        config.rgb_configs_count
    );

    Wire1.setSDA(2);
    Wire1.setSCL(3);
    Wire1.begin();
    Wire1.setClock(1'000'000UL);
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
        backend_count++;
        new_backends[backend_count - 1] = new IntegratedDisplay(
            inputs,
            input_sources,
            input_source_count,
            128,
            64,
            Wire1,
            config,
            backend_id,
            new_backends,
            backend_count
        );
    }

    // Delete the old backends array and reassign it.
    delete[] backends;
    backends = new_backends;

    return backend_count;
}