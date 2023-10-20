#include "comms/backend_init.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/SwitchMatrixInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

Config config = {
    .default_backend = COMMS_BACKEND_XINPUT,
    .game_mode_configs_count = 4,
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
            .activation_binding = { BTN_LT1, BTN_MB1, BTN_LF4 },
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
            .activation_binding = { BTN_LT1, BTN_MB1, BTN_LF3 },
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
            .activation_binding = { BTN_LT1, BTN_MB1, BTN_LF2 },
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .socd_pairs_count = 2,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LF3, .button_dir2 = BTN_LF1, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_LT1, .button_dir2 = BTN_RT4, .socd_type = SOCD_NEUTRAL },
            },
            .button_remapping_count = 1,
            .button_remapping = {
                ButtonRemap { .physical_button = BTN_RT4, .activates = BTN_LT1 },
            },
            .activation_binding_count = 3,
            .activation_binding = { BTN_LT1, BTN_MB1, BTN_LF1 },
        },
    },
    .communication_backend_configs_count = 3,
    .communication_backend_configs = {
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_XINPUT,
            .default_mode = MODE_MELEE,
            .activation_binding_count = 0,
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_DINPUT,
            .default_mode = MODE_MELEE,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RF3 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_CONFIGURATOR,
            .default_mode = MODE_UNSPECIFIED,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RT2 },
        }
    },
};

CommunicationBackend **backends;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

const size_t num_rows = 5;
const size_t num_cols = 13;
const uint row_pins[num_rows] = { 20, 19, 18, 17, 16 };
const uint col_pins[num_cols] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
// clang-format off
const Button matrix[num_rows][num_cols] = {
    {BTN_LF8,   BTN_LF7,  BTN_LF6,  BTN_LF5, NA, BTN_MB3,  BTN_MB1,  BTN_MB2,  NA, BTN_RF5, BTN_RF6,  BTN_RF7,  BTN_RF8 },
    { BTN_LF4,  BTN_LF3,  BTN_LF2,  BTN_LF1, NA, BTN_MB4,  BTN_MB5,  BTN_MB6,  NA, BTN_RF1, BTN_RF2,  BTN_RF3,  BTN_RF4 },
    { BTN_LF12, BTN_LF11, BTN_LF10, BTN_LF9, NA, BTN_MB7,  BTN_MB8,  BTN_MB9,  NA, BTN_RF9, BTN_RF10, BTN_RF11, BTN_RF12},
    { NA,       BTN_LT5,  BTN_LT4,  BTN_LT3, NA, BTN_MB10, BTN_MB11, BTN_MB12, NA, BTN_RT3, BTN_RT4,  BTN_RT5,  NA      },
    { NA,       NA,       BTN_LT1,  BTN_LT2, NA, BTN_LT6,  BTN_RT7,  BTN_RT6,  NA, BTN_RT2, BTN_RT1,  NA,       NA      },
};
// clang-format on
DiodeDirection diode_direction = DiodeDirection::COL2ROW;

const Pinout pinout = {
    .joybus_data = 22,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void setup() {
    static InputState inputs;

    // Create switch matrix input source and use it to read button states for checking button holds.
    static SwitchMatrixInput<num_rows, num_cols>
        matrix_input(row_pins, col_pins, matrix, diode_direction);
    matrix_input.UpdateInputs(inputs);

    // Bootsel button hold as early as possible for safety.
    if (inputs.mb1) {
        reboot_bootloader();
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Attempt to load config, or write default config to flash if failed to load config.
    Persistence *persistence = new Persistence();
    if (!persistence->LoadConfig(config)) {
        persistence->SaveConfig(config);
    }
    delete persistence;

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { &matrix_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    backend_count =
        initialize_backends(backends, inputs, input_sources, input_source_count, config, pinout);

    setup_mode_activation_bindings(config.game_mode_configs, config.game_mode_configs_count);
}

void loop() {
    select_mode(backends[0], config.game_mode_configs, config.game_mode_configs_count);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}
