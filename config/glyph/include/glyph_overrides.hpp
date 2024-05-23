#ifndef _GLYPH_OVERRIDES_HPP
#define _GLYPH_OVERRIDES_HPP

#include "comms/B0XXInputViewer.hpp"
#include "comms/IntegratedDisplay.hpp"
#include "comms/NeoPixelBackend.hpp"
#include "config_defaults.hpp"
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

Config glyph_default_config() {
    Config config = default_config;
    config.rgb_configs_count = 2;
    config.rgb_configs[0] = {
        .button_colors_count = 19,
        .button_colors = {
            { BTN_LF1, 0x0000ff },
            { BTN_LF2, 0x0000ff },
            { BTN_LF3, 0x0000ff },
            { BTN_LF4, 0x0000ff },
            { BTN_LT1, 0x0000ff },
            { BTN_LT2, 0x0000ff },
            { BTN_RF1, 0x0000ff },
            { BTN_RF2, 0x0000ff },
            { BTN_RF3, 0x0000ff },
            { BTN_RF4, 0x0000ff },
            { BTN_RF5, 0x0000ff },
            { BTN_RF6, 0x0000ff },
            { BTN_RF7, 0x0000ff },
            { BTN_RF8, 0x0000ff },
            { BTN_RT1, 0x0000ff },
            { BTN_RT2, 0x0000ff },
            { BTN_RT3, 0x0000ff },
            { BTN_RT4, 0x0000ff },
            { BTN_RT5, 0x0000ff },
        },
    };
    config.rgb_configs[1] = {
        .button_colors_count = 12,
        .button_colors = {
            { BTN_LF6,  0xff0000 },
            { BTN_LF7,  0xff0000 },
            { BTN_LF8,  0xff0000 },
            { BTN_LT6,  0xff0000 },
            { BTN_RF9,  0xff0000 },
            { BTN_RF10, 0xff0000 },
            { BTN_RF11, 0xff0000 },
            { BTN_RF1,  0xff0000 },
            { BTN_RF12, 0xff0000 },
            { BTN_RF13, 0xff0000 },
            { BTN_RF14, 0xff0000 },
            { BTN_RF5,  0xff0000 },
        },
    };
    config.rgb_brightness = 255;

    // Assign layout plates and applicable backends for default gamemode configs.
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        switch (mode_config.mode_id) {
            case MODE_FGC:
                mode_config.rgb_config = 2;
                if (mode_config.button_remapping_count > 0) {
                    mode_config.layout_plate = LAYOUT_PLATE_FGC;
                } else {
                    mode_config.layout_plate = LAYOUT_PLATE_SPLIT_FGC;
                }
                mode_config.applicable_backends[0] = COMMS_BACKEND_XINPUT;
                mode_config.applicable_backends[1] = COMMS_BACKEND_DINPUT;
                mode_config.applicable_backends[2] = COMMS_BACKEND_NINTENDO_SWITCH;
                mode_config.applicable_backends_count = 3;

                // Right hand bottom row
                mode_config.button_remapping[0] = { BTN_RF9, BTN_RF1 };
                mode_config.button_remapping[1] = { BTN_RF10, BTN_RF2 };
                mode_config.button_remapping[2] = { BTN_RF11, BTN_RF3 };
                mode_config.button_remapping[3] = { BTN_RF1, BTN_RF4 };
                // Right hand top row
                mode_config.button_remapping[4] = { BTN_RF12, BTN_RF5 };
                mode_config.button_remapping[5] = { BTN_RF13, BTN_RF6 };
                mode_config.button_remapping[6] = { BTN_RF14, BTN_RF7 };
                mode_config.button_remapping[7] = { BTN_RF5, BTN_RF8 };
                // Left hand row
                mode_config.button_remapping[8] = { BTN_LF8, BTN_LF1 };
                mode_config.button_remapping[9] = { BTN_LF7, BTN_LF2 };
                mode_config.button_remapping[10] = { BTN_LF6, BTN_LF3 };
                // Up button
                mode_config.button_remapping[11] = { BTN_LT6, BTN_LT1 };
                // Menu buttons
                mode_config.button_remapping[12] = { BTN_MB3, BTN_RT3 };
                mode_config.button_remapping[13] = { BTN_MB4, BTN_RT2 };
                mode_config.button_remapping[14] = { BTN_MB2, BTN_MB1 };

                // Unmap the old buttons
                mode_config.button_remapping[15] = { BTN_RF2, BTN_UNSPECIFIED };
                mode_config.button_remapping[16] = { BTN_RF3, BTN_UNSPECIFIED };
                mode_config.button_remapping[17] = { BTN_RF4, BTN_UNSPECIFIED };
                mode_config.button_remapping[18] = { BTN_RF6, BTN_UNSPECIFIED };
                mode_config.button_remapping[19] = { BTN_RF7, BTN_UNSPECIFIED };
                mode_config.button_remapping[20] = { BTN_RF8, BTN_UNSPECIFIED };
                mode_config.button_remapping[21] = { BTN_LF1, BTN_UNSPECIFIED };
                mode_config.button_remapping[22] = { BTN_LF2, BTN_UNSPECIFIED };
                mode_config.button_remapping[23] = { BTN_LF3, BTN_UNSPECIFIED };
                mode_config.button_remapping[24] = { BTN_LT1, BTN_UNSPECIFIED };
                mode_config.button_remapping[25] = { BTN_RT2, BTN_UNSPECIFIED };
                mode_config.button_remapping[26] = { BTN_RT3, BTN_UNSPECIFIED };
                mode_config.button_remapping[27] = { BTN_MB1, BTN_UNSPECIFIED };

                mode_config.button_remapping_count = 28;
                break;
            case MODE_MELEE:
            case MODE_PROJECT_M:
            case MODE_ULTIMATE:
            case MODE_RIVALS_OF_AETHER:
                mode_config.rgb_config = 1;
                mode_config.layout_plate = LAYOUT_PLATE_PLATFORM_FIGHTER;
                mode_config.applicable_backends[0] = COMMS_BACKEND_XINPUT;
                mode_config.applicable_backends[1] = COMMS_BACKEND_DINPUT;
                mode_config.applicable_backends[2] = COMMS_BACKEND_NINTENDO_SWITCH;
                mode_config.applicable_backends[3] = COMMS_BACKEND_GAMECUBE;
                mode_config.applicable_backends_count = 4;
                break;
            default:
                mode_config.layout_plate = LAYOUT_PLATE_EVERYTHING;
        }
    }

    return config;
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
        config.rgb_configs_count,
        config.rgb_brightness
    );

    // Delete the old backends array and reassign it.
    delete[] backends;
    backends = new_backends;

    return backend_count;
}

#endif