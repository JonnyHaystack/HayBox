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
    { BTN_LF4, 11 },
    { BTN_LF3, 15 },
    { BTN_LF2, 16 },
    { BTN_LF1, 14 },

    { BTN_LT1, 3  },
    { BTN_LT2, 0  },
    // { &InputState::nunchuk_c,   2  }, // Dpad Toggle button

    { BTN_MB1, A5 },

    { BTN_RT3, 4  },
    { BTN_RT4, 8  },
    { BTN_RT2, 1  },
    { BTN_RT1, 12 },
    { BTN_RT5, 6  },

    { BTN_RF1, 13 },
    { BTN_RF2, 5  },
    { BTN_RF3, 10 },
    { BTN_RF4, 9  },

    { BTN_RF5, A0 },
    { BTN_RF6, A1 },
    { BTN_RF7, A2 },
    { BTN_RF8, A3 },
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const GpioButtonMapping brook_button_mappings[] = {
    // These are the only buttons which aren't also bound on brook board directly.
    // And so the only buttons which can be bound to dpad_up and l3 on brook
    // WARNING: Bind as few of these as you need, since it increases latency
    { BTN_LF4, 11 },

    { BTN_LT1, 3  },
    { BTN_LT2, 0  },
    // { &InputState::nunchuk_c, 2  },

    { BTN_RT3, 4  },
    { BTN_RT4, 8  },
    { BTN_RT2, 1  },
    { BTN_RT1, 12 },
    { BTN_RT5, 6  },
};

Pinout pinout = {
    .joybus_data = 7,
    .nes_data = 0,
    .nes_clock = 0,
    .nes_latch = 0,
    .mux = A4,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

const int brook_up_pin = 17;
const int brook_l_pin = 30;

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;
bool brook_mode = false;

void setup() {
    static InputState inputs;

    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput gpio_input(button_mappings, button_count);
    gpio_input.UpdateInputs(inputs);

    // Check bootloader button hold as early as possible for safety.
    if (inputs.rt2) {
        Serial.begin(115200);
        reboot_bootloader();
    }

    // Hold RF1 (B) on plugin for Brook board mode.
    pinMode(pinout.mux, OUTPUT);
    if (inputs.rf1) {
        digitalWrite(pinout.mux, HIGH);
        brook_mode = true;
        return;
        // Remaining code is no-op if brook is enabled.
        // Brook Firmware takes control, so we can't control layout/gamemode/backend in this branch
        // In addition, you can force the following brook modes by holding the corresponding button
        // on connecting. If none are held, brook will auto-detect. 1P/X = PS3 2P/Y = PS4 3P/RB =
        // XID-PC 4P/LB = Nintendo Switch These listed buttons correspond to the mapping in brook
        // mode (so can't be remapped) So in the case of the default layout for lbx these correspond
        // to R, Y, LightShield, MidShield
    }
    digitalWrite(pinout.mux, LOW);
    brook_mode = false;

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { &gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    backend_count =
        initialize_backends(backends, inputs, input_sources, input_source_count, config, pinout);

    setup_mode_activation_bindings(config.game_mode_configs, config.game_mode_configs_count);
}

void loop() {
    if (brook_mode) {
        bool button_l = digitalRead(brook_button_mappings[0].pin);
        bool button_mod_x = digitalRead(brook_button_mappings[1].pin);
        bool button_mod_y = digitalRead(brook_button_mappings[2].pin);
        bool button_cstick_down = digitalRead(brook_button_mappings[6].pin);
        bool button_a = digitalRead(brook_button_mappings[7].pin);

        digitalWrite(brook_up_pin, button_mod_x && button_mod_y && button_cstick_down && button_a);
        digitalWrite(brook_l_pin, button_l);
        return;
    }

    select_mode(backends, backend_count, config);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}
