#ifndef _GLYPH_OVERRIDES_HPP
#define _GLYPH_OVERRIDES_HPP

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

const Config default_config = {
    .game_mode_configs_count = 6,
    .game_mode_configs = {
        GameModeConfig {
            .mode_id = MODE_MELEE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .rgb_config = 1,
        },
        GameModeConfig {
            .mode_id = MODE_PROJECT_M,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .rgb_config = 1,
        },
        GameModeConfig {
            .mode_id = MODE_ULTIMATE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .rgb_config = 1,
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .name = "Split FGC",
            .socd_pairs_count = 2,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_LT1, .button_dir2 = BTN_RT4, .socd_type = SOCD_NEUTRAL },
            },
            .rgb_config = 1,
            .layout_plate = LAYOUT_PLATE_SPLIT_FGC,
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .socd_pairs_count = 2,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_LT1, .button_dir2 = BTN_RT4, .socd_type = SOCD_NEUTRAL },
            },
            .button_remapping_count = 28,
            .button_remapping = {
                // Right hand bottom row
                ButtonRemap { .physical_button = BTN_RF9,  .activates = BTN_RF1 },
                ButtonRemap { .physical_button = BTN_RF10, .activates = BTN_RF2 },
                ButtonRemap { .physical_button = BTN_RF11, .activates = BTN_RF3 },
                ButtonRemap { .physical_button = BTN_RF1,  .activates = BTN_RF4 },
                // Right hand top row
                ButtonRemap { .physical_button = BTN_RF12, .activates = BTN_RF5 },
                ButtonRemap { .physical_button = BTN_RF13, .activates = BTN_RF6 },
                ButtonRemap { .physical_button = BTN_RF14, .activates = BTN_RF7 },
                ButtonRemap { .physical_button = BTN_RF5,  .activates = BTN_RF8 },
                // Left hand row
                ButtonRemap { .physical_button = BTN_LF8,  .activates = BTN_LF1 },
                ButtonRemap { .physical_button = BTN_LF7,  .activates = BTN_LF2 },
                ButtonRemap { .physical_button = BTN_LF6,  .activates = BTN_LF3 },
                // Up button
                ButtonRemap { .physical_button = BTN_LT6,  .activates = BTN_LT1 },
                // Menu buttons
                ButtonRemap { .physical_button = BTN_MB3,  .activates = BTN_RT3 },
                ButtonRemap { .physical_button = BTN_MB4,  .activates = BTN_RT2 },
                ButtonRemap { .physical_button = BTN_MB2,  .activates = BTN_MB1 },
                
                // Unmap the old buttons
                ButtonRemap { .physical_button = BTN_RF2,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RF3,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RF4,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RF6,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RF7,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RF8,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_LF1,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_LF2,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_LF3,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_LT1,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RT2,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_RT3,  .activates = BTN_UNSPECIFIED },
                ButtonRemap { .physical_button = BTN_MB1,  .activates = BTN_UNSPECIFIED },
            },
            .activation_binding_count = 3,
            .rgb_config = 2,
            .layout_plate = LAYOUT_PLATE_FGC,
        },
        GameModeConfig {
            .mode_id = MODE_KEYBOARD,
            .socd_pairs_count = 2,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_LT1, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .keyboard_mode_config = 1,
        },
    },
    .communication_backend_configs_count = 8,
    .communication_backend_configs = {
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_XINPUT,
            .default_mode_config = 1,
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_DINPUT,
            .default_mode_config = 1,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RF3 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_NINTENDO_SWITCH,
            .default_mode_config = 3,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RF2 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_GAMECUBE,
            .default_mode_config = 1,
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_N64,
            .default_mode_config = 1,
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_NES,
            .default_mode_config = 1,
            .activation_binding_count = 1,
            .activation_binding = { BTN_LT1 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_SNES,
            .default_mode_config = 1,
            .activation_binding_count = 1,
            .activation_binding = { BTN_LT2 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_CONFIGURATOR,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RT2 },
        }
    },
    .keyboard_modes_count = 1,
    .keyboard_modes = {
        KeyboardModeConfig {
            0,
            22,
            {
                { BTN_LF4, HID_KEY_A },
                { BTN_LF3, HID_KEY_B },
                { BTN_LF2, HID_KEY_C },
                { BTN_LF1, HID_KEY_D },
                { BTN_LT1, HID_KEY_E },
                { BTN_LT2, HID_KEY_F },
                { BTN_MB3, HID_KEY_G },
                { BTN_MB1, HID_KEY_H },
                { BTN_MB2, HID_KEY_I },
                { BTN_RF5, HID_KEY_J },
                { BTN_RF6, HID_KEY_K },
                { BTN_RF7, HID_KEY_L },
                { BTN_RF8, HID_KEY_M },
                { BTN_RF1, HID_KEY_N },
                { BTN_RF2, HID_KEY_O },
                { BTN_RF3, HID_KEY_P },
                { BTN_RF4, HID_KEY_Q },
                { BTN_RT4, HID_KEY_R },
                { BTN_RT3, HID_KEY_S },
                { BTN_RT5, HID_KEY_T },
                { BTN_RT1, HID_KEY_U },
                { BTN_RT2, HID_KEY_V },
            },
        },
    },
    .rgb_configs_count = 2,
    .rgb_configs = {
        RgbConfig {
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
        },
        RgbConfig {
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
        },
    },
    .default_backend_config = 1,
    .default_usb_backend_config = 1,
    .rgb_brightness = 255,
};

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

    // Assign layout plates and applicable backends for default gamemode configs.
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        switch (mode_config.mode_id) {
            case MODE_FGC:
                mode_config.applicable_backends[0] = COMMS_BACKEND_XINPUT;
                mode_config.applicable_backends[1] = COMMS_BACKEND_DINPUT;
                mode_config.applicable_backends[2] = COMMS_BACKEND_NINTENDO_SWITCH;
                mode_config.applicable_backends_count = 3;
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
            case MODE_KEYBOARD:
                mode_config.applicable_backends[0] = COMMS_BACKEND_DINPUT;
                mode_config.applicable_backends_count = 1;
                mode_config.rgb_config = 1;
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
    CommunicationBackend **new_backends = new CommunicationBackend *[backend_count + 1];
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