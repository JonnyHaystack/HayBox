#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

const GpioButtonMapping button_mappings[] = {
    { BTN_LF4, 9  },
    { BTN_LF3, 15 },
    { BTN_LF2, 16 },
    { BTN_LF1, 14 },

    { BTN_LT1, 8  },
    { BTN_LT2, 6  },

    { BTN_MB1, 12 },

    { BTN_RT3, A1 },
    { BTN_RT4, A2 },
    { BTN_RT2, 5  },
    { BTN_RT1, 13 },
    { BTN_RT5, A0 },

    { BTN_RF1, 4  },
    { BTN_RF2, A5 },
    { BTN_RF3, A4 },
    { BTN_RF4, A3 },

    { BTN_RF5, 0  },
    { BTN_RF6, 1  },
    { BTN_RF7, 10 },
    { BTN_RF8, 11 },
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 17,
    .nes_data = 0,
    .nes_clock = 0,
    .nes_latch = 0,
    .mux = 7,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

void setup() {
    static InputState inputs;

    // Create Nunchuk input source - must be done before GPIO input source otherwise it would
    // disable the pullups on the i2c pins.
    // static NunchukInput nunchuk;

    // Create GPIO input source and use it to read button states for checking button holds.
    static GpioButtonInput gpio_input(button_mappings, button_count);
    gpio_input.UpdateInputs(inputs);

    // Check bootloader button hold as early as possible for safety.
    if (inputs.rt2) {
        Serial.begin(115200);
        reboot_bootloader();
    }

    // Hold RF1 (B) on plugin for Brook board mode.
    pinMode(pinout.mux, OUTPUT);
    if (inputs.rf1)
        digitalWrite(pinout.mux, LOW);
    else
        digitalWrite(pinout.mux, HIGH);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = {
        &gpio_input,
        // &nunchuk,
    };
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
