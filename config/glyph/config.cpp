#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "glyph_overrides.hpp"
#include "input/DebouncedSwitchMatrixInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

const size_t num_rows = 4;
const size_t num_cols = 11;
const uint row_pins[num_rows] = { 26, 25, 24, 23 };
const uint col_pins[num_cols] = { 15, 14, 13, 12, 16, 17, 21, 20, 19, 18, 22 };
// clang-format off
const Button matrix[num_rows][num_cols] = {
    {BTN_MB1,  BTN_MB2, BTN_MB3, BTN_MB4, BTN_MB5,  BTN_MB6,  BTN_MB7,  NA,      NA,      NA,      NA     },
    { BTN_LF3, BTN_LF2, BTN_LF6, BTN_LF7, BTN_RF12, BTN_RF13, BTN_RF14, BTN_RF5, BTN_RF6, BTN_RF7, BTN_RF8},
    { BTN_LF4, BTN_LF5, BTN_LF1, BTN_LF8, BTN_RF9,  BTN_RF10, BTN_RF11, BTN_RF1, BTN_RF2, BTN_RF3, BTN_RF4},
    { BTN_LT5, BTN_LT4, BTN_LT1, BTN_LT3, BTN_LT2,  BTN_LT6,  BTN_RT2,  BTN_RT3, BTN_RT1, BTN_RT4, BTN_RT5},
};
// clang-format on

DebouncedSwitchMatrixInput<num_rows, num_cols> matrix_input(
    row_pins,
    col_pins,
    matrix,
    DiodeDirection::COL2ROW
);

const Pinout pinout = {
    .joybus_data = 4,
    .nes_data = 4,
    .nes_clock = 5,
    .nes_latch = 6,
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
    matrix_input.UpdateInputs(inputs);

    // Check bootsel button hold as early as possible for safety.
    if (inputs.mb1) {
        reboot_bootloader();
    }

    // Attempt to load config, or write default config to flash if failed to load config.
    Persistence *persistence = new Persistence();
    if (!persistence->LoadConfig(config)) {
        persistence->SaveConfig(config);
    }
    delete persistence;

    // Create array of input sources to be used.
    static InputSource *input_sources[] = {};
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    backend_count = initialize_backends(
        backends,
        inputs,
        input_sources,
        input_source_count,
        config,
        pinout,
        get_backend_config_default,
        &usb_backend_from_4pos_switch,
        &detect_console,
        &init_secondary_backends_glyph
    );

    setup_mode_activation_bindings(config.game_mode_configs, config.game_mode_configs_count);
}

void loop() {
    select_mode(backends, backend_count, config);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}

/* Button inputs are read from the second core */

void setup1() {
    while (backends == nullptr) {
        tight_loop_contents();
    }
}

void loop1() {
    if (backends != nullptr) {
        matrix_input.UpdateInputs(backends[0]->GetInputs());
    }
}
