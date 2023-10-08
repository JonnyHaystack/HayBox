#include "comms/backend_init.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = {
    .default_backend = COMMS_BACKEND_XINPUT,
    .game_mode_configs_count = 4,
    .game_mode_configs = {
        GameModeConfig {
            .mode_id = MODE_MELEE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_L },
        },
        GameModeConfig {
            .mode_id = MODE_PROJECT_M,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_LEFT },
        },
        GameModeConfig {
            .mode_id = MODE_ULTIMATE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_DOWN },
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_MOD_X, .button_dir2 = BTN_C_UP, .socd_type = SOCD_DIR1_PRIORITY },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_MOD_X, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_NEUTRAL },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_RIGHT },
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
            .activation_binding = { BTN_Z },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_CONFIGURATOR,
            .default_mode = MODE_UNSPECIFIED,
            .activation_binding_count = 1,
            .activation_binding = { BTN_C_DOWN },
        }
    },
};

GpioButtonMapping button_mappings[] = {
    {BTN_L,            5 },
    { BTN_LEFT,        4 },
    { BTN_DOWN,        3 },
    { BTN_RIGHT,       2 },

    { BTN_MOD_X,       6 },
    { BTN_MOD_Y,       7 },

    { BTN_SELECT,      10},
    { BTN_START,       0 },
    { BTN_HOME,        11},

    { BTN_C_LEFT,      13},
    { BTN_C_UP,        12},
    { BTN_C_DOWN,      15},
    { BTN_A,           14},
    { BTN_C_RIGHT,     16},

    { BTN_B,           26},
    { BTN_X,           21},
    { BTN_Z,           19},
    { BTN_UP,          17},

    { BTN_R,           27},
    { BTN_Y,           22},
    { BTN_LIGHTSHIELD, 20},
    { BTN_MIDSHIELD,   18},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 28,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

void setup() {
    static InputState inputs;

    // Create GPIO input source and use it to read button states for checking button holds.
    static GpioButtonInput gpio_input(button_mappings, button_count);
    gpio_input.UpdateInputs(inputs);

    // Check bootsel button hold as early as possible for safety.
    if (inputs.start) {
        rp2040.rebootToBootloader();
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
    static InputSource *input_sources[] = { &gpio_input };
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

/* Nunchuk code runs on the second core */
NunchukInput *nunchuk = nullptr;

void setup1() {
    while (backends == nullptr) {
        tight_loop_contents();
    }

    // Create Nunchuk input source.
    nunchuk = new NunchukInput(Wire, pinout.nunchuk_detect, pinout.nunchuk_sda, pinout.nunchuk_scl);
}

void loop1() {
    if (backends != nullptr) {
        nunchuk->UpdateInputs(backends[0]->GetInputs());
        busy_wait_us(50);
    }
}
