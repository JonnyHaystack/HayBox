#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/KeyboardMode.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "modes/Melee20Button.hpp"
#include "stdlib.hpp"

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            47},
    { &InputState::left,        24},
    { &InputState::down,        23},
    { &InputState::right,       25},

    { &InputState::mod_x,       28},
    { &InputState::mod_y,       29},
    { &InputState::select,      30},
    { &InputState::home,        31},

    { &InputState::start,       50},

    { &InputState::c_left,      36},
    { &InputState::c_up,        34},
    { &InputState::c_down,      46},
    { &InputState::a,           35},
    { &InputState::c_right,     37},

    { &InputState::b,           44},
    { &InputState::x,           42},
    { &InputState::z,           7 },
    { &InputState::up,          45},

    { &InputState::r,           41},
    { &InputState::y,           43},
    { &InputState::lightshield, 40},
    { &InputState::midshield,   12},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

Pinout pinout = {
    .joybus_data = 52,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void setup() {
    // Create Nunchuk input source - must be done before GPIO input source otherwise it would
    // disable the pullups on the i2c pins.
    NunchukInput *nunchuk = new NunchukInput();

    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput *gpio_input = new GpioButtonInput(button_mappings, button_count);

    InputState button_holds;
    gpio_input->UpdateInputs(button_holds);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { gpio_input, nunchuk };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    CommunicationBackend *primary_backend = nullptr;
    if (button_holds.a) {
        // Hold A on plugin for GameCube adapter.
        primary_backend =
            new GamecubeBackend(input_sources, input_source_count, 0, pinout.joybus_data);
    } else {
        // Default to GameCube/Wii.
        primary_backend =
            new GamecubeBackend(input_sources, input_source_count, 125, pinout.joybus_data);
    }

    backend_count = 1;
    backends = new CommunicationBackend *[backend_count] { primary_backend };

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
}
