#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/DebouncedGpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

GpioButtonMapping button_mappings[] = {
    { BTN_LF1, 2 }, // Right
    { BTN_LF2, 3 }, // Down
    { BTN_LF3, 4 }, // Left
    { BTN_LF4, 5 }, // L

    { BTN_LT1, 6 }, // ModX
    { BTN_LT2, 7 }, // ModY 

    { BTN_MB1, 0 }, // Start
    { BTN_MB2, 10 }, // Home
    { BTN_MB3, 11 }, // Select

    { BTN_RT1, 14 }, // A
    { BTN_RT2, 15 }, // C-Down
    { BTN_RT3, 13 }, // C-Left
    { BTN_RT4, 12 }, // C-Up
    { BTN_RT5, 16 }, // C-Right

    { BTN_RF1, 26 }, // B
    { BTN_RF2, 21 }, // X
    { BTN_RF3, 19 }, // Z (buttonR)
    { BTN_RF4, 17 }, // Up

    { BTN_RF5, 27 }, // R
    { BTN_RF6, 22 }, // Y
    { BTN_RF7, 20 }, // LightShield
    { BTN_RF8, 18 }, // MidShield
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

DebouncedGpioButtonInput<button_count> gpio_input(button_mappings);

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
    if (inputs.mb1) {
        reboot_bootloader();
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Attempt to load config, or write default config to flash if failed to load config.
    if (inputs.rf5 || !persistence.LoadConfig(config)) { //hold R on plugin to overwrite config
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
        gpio_input.UpdateInputs(backends[0]->GetInputs());
    }
}
