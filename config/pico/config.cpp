#include "comms/backend_init.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "reboot.hpp"
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
            .activation_binding = { BTN_RT3 },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_CONFIGURATOR,
            .default_mode = MODE_UNSPECIFIED,
            .activation_binding_count = 1,
            .activation_binding = { BTN_RT2 },
        }
    },
};

GpioButtonMapping button_mappings[] = {
    {BTN_LF1,  2 },
    { BTN_LF2, 3 },
    { BTN_LF3, 4 },
    { BTN_LF4, 5 },

    { BTN_LT1, 6 },
    { BTN_LT2, 7 },

    { BTN_MB1, 0 },
    { BTN_MB2, 10},
    { BTN_MB3, 11},

    { BTN_RT1, 14},
    { BTN_RT2, 15},
    { BTN_RT3, 13},
    { BTN_RT4, 12},
    { BTN_RT5, 16},

    { BTN_RF1, 26},
    { BTN_RF2, 21},
    { BTN_RF3, 19},
    { BTN_RF4, 17},

    { BTN_RT5, 27},
    { BTN_RT6, 22},
    { BTN_RT7, 20},
    { BTN_RT8, 18},
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
