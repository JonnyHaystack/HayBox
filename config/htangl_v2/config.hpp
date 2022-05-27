#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include "stdlib.hpp"

#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "modes/Melee20Button.hpp"

CommunicationBackend **backends;
size_t backend_count;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            12},
    { &InputState::left,        7 },
    { &InputState::down,        11},
    { &InputState::right,       5 },
    { &InputState::mod_x,       9 },
    { &InputState::mod_y,       A1},

    { &InputState::select,      2 },
    { &InputState::start,       1 },
    { &InputState::home,        3 },

    { &InputState::c_left,      A3},
    { &InputState::c_up,        A4},
    { &InputState::c_down,      A0},
    { &InputState::a,           10},
    { &InputState::c_right,     A2},

    { &InputState::b,           0 },
    { &InputState::x,           4 },
    { &InputState::z,           6 },
    { &InputState::up,          8 },

    { &InputState::r,           16},
    { &InputState::y,           15},
    { &InputState::lightshield, A5},
    { &InputState::midshield,   14},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

Pinout pinout = {
    .joybus_data = 13,
    .mux = -1,
    .nunchuk_sda = 2,
    .nunchuk_scl = 3,
    .nunchuk_detect = -1,
};

void initialise() {
    // Create Nunchuk input source - must be done before GPIO input source otherwise it would
    // disable the pullups on the i2c pins.
    NunchukInput *nunchuk = new NunchukInput();

    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput *gpio_input = new GpioButtonInput(button_mappings, button_count);

    InputState button_holds;
    gpio_input->UpdateInputs(button_holds);

    // Create array of input sources to be used.
    InputSource *input_sources[] = { gpio_input, nunchuk };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    CommunicationBackend *primary_backend = new DInputBackend(input_sources, input_source_count);
    delay(500);
    bool usb_connected = UDADDR & _BV(ADDEN);

    /* Select communication backend. */
    if (usb_connected) {
        // Default to DInput mode if USB is connected.
        // Input viewer only used when connected to PC i.e. when using DInput mode.
        backends = new CommunicationBackend *[2] {
            primary_backend, new B0XXInputViewer(input_sources, input_source_count)
        };
        backend_count = 2;
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
        backends = new CommunicationBackend *[1] { primary_backend };
        backend_count = 1;
    }

    // Default to Melee mode.
    primary_backend->SetGameMode(new Melee20Button(socd::SOCD_2IP_NO_REAC));
}

#endif
