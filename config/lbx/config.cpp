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
#include "modes/MeleeLbx.hpp"
#include "stdlib.hpp"

const int brook_up_pin = 17;
const int brook_l_pin = 30;

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;
bool brook_mode = false;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            11},
    { &InputState::left,        15},
    { &InputState::down,        16},
    { &InputState::right,       14},

    { &InputState::mod_x,       3 },
    { &InputState::mod_y,       0 },
    { &InputState::select,       2 }, // Third mod button

    { &InputState::start,       A5},

    { &InputState::c_left,      4 },
    { &InputState::c_up,        8 },
    { &InputState::c_down,      1 },
    { &InputState::a,           12},
    { &InputState::c_right,     6 },

    { &InputState::b,           13},
    { &InputState::x,           5 },
    { &InputState::z,           10},
    { &InputState::up,          9 },

    { &InputState::r,           A0},
    { &InputState::y,           A1},
    { &InputState::lightshield, A2},
    { &InputState::midshield,   A3},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

Pinout pinout = { .joybus_data = 7, .mux = A4 };

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
    if (button_holds.b) {
        digitalWrite(pinout.mux, HIGH);
        brook_mode = true;
    } else {
        digitalWrite(pinout.mux, LOW);
        brook_mode = false;
    }

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
    primary_backend->SetGameMode(new MeleeLbx(socd::SOCD_2IP_NO_REAC));
}

void loop() {
    if (brook_mode) {
        bool button_l = digitalRead(button_mappings[0].pin);
        bool button_mod_x = digitalRead(button_mappings[4].pin);
        bool button_mod_y = digitalRead(button_mappings[5].pin);
        bool button_cstick_down = digitalRead(button_mappings[10].pin);
        bool button_a = digitalRead(button_mappings[11].pin);

        digitalWrite(brook_up_pin, button_mod_x && button_mod_y && button_cstick_down && button_a);
        digitalWrite(brook_l_pin, button_l);
        return;
    }

    select_mode(backends[0]);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}
