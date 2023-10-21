#ifndef _CONFIG_DEFAULTS_HPP
#define _CONFIG_DEFAULTS_HPP

#include <config.pb.h>
#include <keycodes.h>

// clang-format off

const Config default_config = {
    .default_backend = COMMS_BACKEND_DINPUT,
    .game_mode_configs_count = 4,
    .game_mode_configs = new GameModeConfig[4] {
        GameModeConfig {
            .mode_id = MODE_MELEE,
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
        },
        GameModeConfig {
            .mode_id = MODE_PROJECT_M,
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
        },
        GameModeConfig {
            .mode_id = MODE_ULTIMATE,
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
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
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
        },
    },
    .communication_backend_configs_count = 1,
    .communication_backend_configs = new CommunicationBackendConfig[1] {
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_GAMECUBE,
            .default_mode = MODE_MELEE,
            .activation_binding_count = 0,
            .activation_binding = {},
        },
    },
    .keyboard_modes_count = 0,
    .keyboard_modes = {},
};

// clang-format on

#endif