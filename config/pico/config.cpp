#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
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
    {&InputState::l,            5 },
    { &InputState::left,        4 },
    { &InputState::down,        3 },
    { &InputState::right,       2 },

    { &InputState::mod_x,       6 },
    { &InputState::mod_y,       7 },

    { &InputState::start,       0 },

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
    .nunchuk_sda = 8,
    .nunchuk_scl = 9,
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
    CommunicationBackend *primary_backend;
    if (console == ConnectedConsole::NONE) {
        if (button_holds.x) {
            // If no console detected and X is held on plugin then use Switch USB backend.
            NintendoSwitchBackend::RegisterDescriptor();
            backend_count = 1;
            primary_backend = new NintendoSwitchBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            primary_backend->SetGameMode(new Ultimate(socd::SOCD_2IP));
        } else {
            // Default to DInput mode if no console detected.
            // Input viewer only used when connected to PC i.e. when using DInput mode.
            TUGamepad::registerDescriptor();
            TUKeyboard::registerDescriptor();
            backend_count = 2;
            primary_backend = new DInputBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
        }
    } else {
        if (console == ConnectedConsole::GAMECUBE) {
            primary_backend =
                new GamecubeBackend(input_sources, input_source_count, pinout.joybus_data);
        } else if (console == ConnectedConsole::N64) {
            primary_backend = new N64Backend(input_sources, input_source_count, pinout.joybus_data);
        }

        // If not DInput then only using 1 backend (no input viewer).
        backend_count = 1;
        backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    // Default to Melee mode.
    primary_backend->SetGameMode(new Melee20Button(socd::SOCD_2IP_NO_REAC));
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
