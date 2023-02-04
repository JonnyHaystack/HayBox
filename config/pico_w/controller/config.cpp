#include "comms/WiFiClientBackend.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "stdlib.hpp"

#include <pico/bootrom.h>

CommunicationBackend **backends = nullptr;
size_t backend_count;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            5 },
    { &InputState::left,        4 },
    { &InputState::down,        3 },
    { &InputState::right,       2 },

    { &InputState::mod_x,       6 },
    { &InputState::mod_y,       7 },

    { &InputState::select,      10},
    { &InputState::start,       0 },
    { &InputState::home,        11},

    { &InputState::c_left,      13},
    { &InputState::c_up,        12},
    { &InputState::c_down,      15},
    { &InputState::a,           14},
    { &InputState::c_right,     16},

    { &InputState::b,           26},
    { &InputState::x,           21},
    { &InputState::z,           19},
    { &InputState::up,          17},

    { &InputState::r,           27},
    { &InputState::y,           22},
    { &InputState::lightshield, 20},
    { &InputState::midshield,   18},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 28,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void setup() {
    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput *gpio_input = new GpioButtonInput(button_mappings, button_count);

    InputState button_holds;
    gpio_input->UpdateInputs(button_holds);

    // Bootsel button hold as early as possible for safety.
    if (button_holds.start) {
        reset_usb_boot(0, 0);
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    /* Select communication backend. */
    CommunicationBackend *primary_backend =
        new WiFiClientBackend(input_sources, input_source_count);

    backend_count = 1;
    backends = new CommunicationBackend *[backend_count] { primary_backend };
}

void loop() {
    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }
}
