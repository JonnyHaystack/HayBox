#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

const GpioButtonMapping button_mappings[] = {
    { BTN_LF4, 2  },
    { BTN_LF3, 10 },
    { BTN_LF2, 13 },
    { BTN_LF1, 5  },

    { BTN_LT1, 7  },
    { BTN_LT2, 3  },

    { BTN_MB3, 9  },
    { BTN_MB1, 6  },
    { BTN_MB2, 8  },

    { BTN_RT3, A1 },
    { BTN_RT4, A3 },
    { BTN_RT2, A0 },
    { BTN_RT1, A2 },
    { BTN_RT5, A4 },

    { BTN_RF1, A5 },
    { BTN_RF2, 14 },
    { BTN_RF3, 16 },
    { BTN_RF4, 15 },

    { BTN_RF5, 12 },
    { BTN_RF6, 4  },
    { BTN_RF7, 1  },
    { BTN_RF8, 0  },
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 17,
    .nes_data = 0,
    .nes_clock = 0,
    .nes_latch = 0,
    .mux = 11,
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
    GpioButtonInput gpio_input(button_mappings, button_count);
    gpio_input.UpdateInputs(inputs);

    // Check bootloader button hold as early as possible for safety.
    if (inputs.mb1) {
        Serial.begin(115200);
        reboot_bootloader();
    }

    // Hold RF1 (B) on plugin for Brook board mode.
    pinMode(pinout.mux, OUTPUT);
    if (inputs.rf1)
        digitalWrite(pinout.mux, HIGH);
    else
        digitalWrite(pinout.mux, LOW);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { &gpio_input };
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
