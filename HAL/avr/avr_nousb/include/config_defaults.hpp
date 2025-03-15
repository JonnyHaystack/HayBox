#ifndef _CONFIG_DEFAULTS_HPP
#define _CONFIG_DEFAULTS_HPP

#include <config.pb.h>

// clang-format off

const Config default_config = {
    .game_mode_configs_count = 4,
    .game_mode_configs = new GameModeConfig[4] {
        GameModeConfig {
            .mode_id = MODE_MELEE,
            .name = {},
            .socd_pairs_count = 4,
            .socd_pairs = new SocdPair[4] {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .button_remapping = {},
            .activation_binding_count = 3,
            .activation_binding = new Button[3] { BTN_LT1, BTN_MB1, BTN_LF4 },
            .custom_mode_config = 0,
            .keyboard_mode_config = 0,
            .rgb_config = 0,
        },
        GameModeConfig {
            .mode_id = MODE_PROJECT_M,
            .name = {},
            .socd_pairs_count = 4,
            .socd_pairs = new SocdPair[4] {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .button_remapping = {},
            .activation_binding_count = 3,
            .activation_binding = new Button[3] { BTN_LT1, BTN_MB1, BTN_LF3 },
            .custom_mode_config = 0,
            .keyboard_mode_config = 0,
            .rgb_config = 0,
        },
        GameModeConfig {
            .mode_id = MODE_ULTIMATE,
            .name = {},
            .socd_pairs_count = 4,
            .socd_pairs = new SocdPair[4] {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_LF2, .button_dir2 = BTN_RF4, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_RT3, .button_dir2 = BTN_RT5, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_RT2, .button_dir2 = BTN_RT4, .socd_type = SOCD_2IP },
            },
            .button_remapping_count = 0,
            .button_remapping = {},
            .activation_binding_count = 3,
            .activation_binding = new Button[3] { BTN_LT1, BTN_MB1, BTN_LF2 },
            .custom_mode_config = 0,
            .keyboard_mode_config = 0,
            .rgb_config = 0,
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .name = {},
            .socd_pairs_count = 2,
            .socd_pairs = new SocdPair[2] {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_LT1, .button_dir2 = BTN_RT4, .socd_type = SOCD_NEUTRAL },
            },
            .button_remapping_count = 1,
            .button_remapping = new ButtonRemap[1] {
                ButtonRemap { .physical_button = BTN_RT4, .activates = BTN_LT1 },
            },
            .activation_binding_count = 3,
            .activation_binding = new Button[3] { BTN_LT1, BTN_MB1, BTN_LF1 },
            .custom_mode_config = 0,
            .keyboard_mode_config = 0,
            .rgb_config = 0,
        },
    },
    .communication_backend_configs_count = 2,
    .communication_backend_configs = new CommunicationBackendConfig[2] {
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_GAMECUBE,
            .default_mode_config = 1,
            .activation_binding_count = 1,
            .activation_binding = new Button[1] { BTN_RT1 },
            .secondary_backends = {},
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_N64,
            .default_mode_config = 1,
            .activation_binding_count = 1,
            .activation_binding = new Button[1] { BTN_RT3 },
            .secondary_backends = {},
        },
    },
    .custom_modes_count = 0,
    .custom_modes = {},
    .keyboard_modes_count = 0,
    .keyboard_modes = {},
    .rgb_configs_count = 0,
    .rgb_configs = {},
    .default_backend_config = 1,
    .default_usb_backend_config = 1,
    .rgb_brightness = 0,
    .has_melee_options = true,
    .melee_options = {
        .crouch_walk_os = false,
        .disable_ledgedash_socd_override = false,
        .has_custom_airdodge = false,
        .custom_airdodge = { .x = 0, .y = 0 },
    },
    .has_project_m_options = true,
    .project_m_options = {
        .true_z_press = true,
        .disable_ledgedash_socd_override = false,
        .has_custom_airdodge = false,
        .custom_airdodge = { .x = 0, .y = 0 },
    },
};

// clang-format on

#endif
