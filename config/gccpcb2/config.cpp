#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "modes/Melee20Button.hpp"
#include "stdlib.hpp"

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            2 },
    { &InputState::left,        10},
    { &InputState::down,        13},
    { &InputState::right,       5 },

    { &InputState::mod_x,       7 },
    { &InputState::mod_y,       3 },

    { &InputState::select,      9 },
    { &InputState::start,       6 },
    { &InputState::home,        8 },

    { &InputState::c_left,      A1},
    { &InputState::c_up,        A3},
    { &InputState::c_down,      A0},
    { &InputState::a,           A2},
    { &InputState::c_right,     A4},

    { &InputState::b,           A5},
    { &InputState::x,           14},
    { &InputState::z,           16},
    { &InputState::up,          15},

    { &InputState::r,           12},
    { &InputState::y,           4 },
    { &InputState::lightshield, 1 },
    { &InputState::midshield,   0 },
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

Pinout pinout = {
    .joybus_data = 17,
    .mux = 11,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void setup() {
    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput *gpio_input = new GpioButtonInput(button_mappings, button_count);

    InputState button_holds;
    gpio_input->UpdateInputs(button_holds);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    // Hold B on plugin for Brook board mode.
    pinMode(pinout.mux, OUTPUT);
    if (button_holds.b)
        digitalWrite(pinout.mux, HIGH);
    else
        digitalWrite(pinout.mux, LOW);

    CommunicationBackend *primary_backend = new DInputBackend(input_sources, input_source_count);
    delay(500);
    bool usb_connected = UDADDR & _BV(ADDEN);

    /* Select communication backend. */
    if (usb_connected) {
        // Default to DInput mode if USB is connected.
        // Input viewer only used when connected to PC i.e. when using DInput mode.
        backend_count = 2;
        backends = new CommunicationBackend *[backend_count] {
            primary_backend, new B0XXInputViewer(input_sources, input_source_count)
        };
    } else {
        delete primary_backend;
        if (button_holds.c_left) {
            // Hold C-Left on plugin for N64.
            primary_backend =
                new N64Backend(input_sources, input_source_count, 60, pinout.joybus_data);
        } else if (button_holds.a) {
            // Hold A on plugin for GameCube adapter.
            primary_backend =
                new GamecubeBackend(input_sources, input_source_count, 0, pinout.joybus_data);
        } else {
            // Default to GameCube/Wii.
            primary_backend =
                new GamecubeBackend(input_sources, input_source_count, 125, pinout.joybus_data);
        }

        // If not DInput then only using 1 backend (no input viewer).
        backend_count = 1;
        backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    // Default to Melee mode.
    primary_backend->SetGameMode(
        new Melee20Button(socd::SOCD_2IP_NO_REAC, { .crouch_walk_os = false })
    );
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
