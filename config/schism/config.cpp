#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/DebouncedGpioButtonInput.hpp"
#include "input/Pca9671Input.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

GpioButtonMapping button_mappings[] = {
    {BTN_LF1,  27},
    { BTN_LF2, 26},
    { BTN_LF3, 19},
    { BTN_LF4, 18},

    { BTN_LT1, 3 },
    { BTN_LT2, 2 },
    { BTN_LT3, 0 },

    { BTN_MB1, 1 },
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

Pca9671ButtonMapping expander_button_mappings[] = {
    {BTN_RT1,  14},
    { BTN_RT2, 15},
    { BTN_RT3, 13},
    { BTN_RT4, 11},
    { BTN_RT5, 12},

    { BTN_RF1, 7 },
    { BTN_RF2, 3 },
    { BTN_RF3, 4 },
    { BTN_RF4, 6 },

    { BTN_RF5, 0 },
    { BTN_RF6, 1 },
    { BTN_RF7, 2 },
    { BTN_RF8, 5 },
};
const size_t expander_button_count =
    sizeof(expander_button_mappings) / sizeof(Pca9671ButtonMapping);

DebouncedGpioButtonInput<button_count> gpio_input(button_mappings);
Pca9671Input expander_input(expander_button_mappings, expander_button_count, Wire, 20, 21);

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
    gpio_input.UpdateInputs(inputs);

    // Check bootsel button hold as early as possible for safety.
    if (inputs.rt2) {
        reboot_bootloader();
    }

    // Also scan I/O expander for inputs, but after the bootloader check in case it causes a crash.
    expander_input.UpdateInputs(inputs);

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Attempt to load config, or write default config to flash if failed to load config.
    if (!persistence.LoadConfig(config)) {
        persistence.SaveConfig(config);
    }

    // Create array of input sources to be used.
    static InputSource *input_sources[] = {};
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    backend_count =
        initialize_backends(backends, inputs, input_sources, input_source_count, config, pinout);

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
        InputState &inputs = backends[0]->GetInputs();
        gpio_input.UpdateInputs(inputs);
        expander_input.UpdateInputs(inputs);
    }
}
