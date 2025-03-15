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
    {BTN_LF1,  9 },
    { BTN_LF2, 8 },
    { BTN_LF3, 7 },
    { BTN_LF4, 6 },

    { BTN_LT1, 10},
    { BTN_LT2, 11},

    { BTN_MB1, 12},

    { BTN_RT1, 28},
    { BTN_RT2, 27},
    { BTN_RT3, 14},
    { BTN_RT4, 13},
    { BTN_RT5, 15},

    { BTN_RF1, 19},
    { BTN_RF2, 18},
    { BTN_RF3, 17},
    { BTN_RF4, 16},

    { BTN_RF5, 26},
    { BTN_RF6, 22},
    { BTN_RF7, 21},
    { BTN_RF8, 20},
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 2,
    .nes_data = -1,
    .nes_clock = -1,
    .nes_latch = -1,
    .mux = -1,
    .nunchuk_detect = 3,
    .nunchuk_sda = 4,
    .nunchuk_scl = 5,
};

DebouncedGpioButtonInput<button_count> gpio_input(button_mappings);
NunchukInput *nunchuk = nullptr;

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

void setup() {
    static InputState inputs;

    // Read button states for checking button holds.
    gpio_input.UpdateInputs(inputs);

    // Check bootsel button hold as early as possible for safety.
    if (inputs.rt2) {
        reboot_bootloader();
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Attempt to load config, or write default config to flash if failed to load config.
    if (!persistence.LoadConfig(config)) {
        persistence.SaveConfig(config);
    }

    // Create Nunchuk input source.
    nunchuk = new NunchukInput(Wire, pinout.nunchuk_detect, pinout.nunchuk_sda, pinout.nunchuk_scl);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { nunchuk };
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
