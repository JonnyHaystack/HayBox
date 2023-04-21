#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/KeyboardMode.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "joybus_utils.hpp"
#include "modes/Melee20Button.hpp"
#include "stdlib.hpp"

#include <pico/bootrom.h>

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GpioButtonMapping button_mappings[] = {
    { &InputState::l,           6 },
    { &InputState::left,        7 },
    { &InputState::down,        8 },
    { &InputState::right,       9 },

    { &InputState::mod_x,       10},
    { &InputState::mod_y,       11},

    { &InputState::start,       12},

    { &InputState::c_left,      14},
    { &InputState::c_up,        13},
    { &InputState::c_down,      27},
    { &InputState::a,           28},
    { &InputState::c_right,     15},

    { &InputState::b,           19},
    { &InputState::x,           18},
    { &InputState::z,           17},
    { &InputState::up,          16},

    { &InputState::r,           26},
    { &InputState::y,           22},
    { &InputState::lightshield, 21},
    { &InputState::midshield,   20},
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

    ConnectedConsole console = detect_console(pinout.joybus_data);

    /* Select communication backend. */
    /* CommunicationBackend *primary_backend; */
    NintendoSwitchBackend::RegisterDescriptor();
    backend_count = 1;
    CommunicationBackend *primary_backend = new NintendoSwitchBackend(input_sources, input_source_count);
    backends = new CommunicationBackend *[backend_count] { primary_backend };

    // Default to Ultimate mode.
    primary_backend->SetGameMode(new UltimateR4(socd::SOCD_2IP));
}

void loop() {
    select_mode(backends[0]);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}

