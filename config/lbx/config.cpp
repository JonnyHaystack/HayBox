#include "comms/B0XXInputViewer.hpp"
#include "comms/BrookPassthroughBackend.hpp"
#include "comms/DInputBackend.hpp"
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
bool use_brook_mode = false;

// Customise this to match your controller's pinout.
GpioButtonMapping button_mappings[] = {
    { &InputState::l,           11},
    { &InputState::left,        15},
    { &InputState::down,        16},
    { &InputState::right,       14},

    { &InputState::mod_x,       3 },
    { &InputState::mod_y,       0 },
    { &InputState::dpad_toggle, 2 },

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

GpioButtonMapping brook_button_mappings[] = {
  // These are the only buttons which aren't alos bound on brook board directly.
    { &InputState::l,           11},

    { &InputState::mod_x,       3 },
    { &InputState::mod_y,       0 },
    { &InputState::dpad_toggle, 2 },

    { &InputState::c_left,      4 },
    { &InputState::c_up,        8 },
    { &InputState::c_down,      1 },
    { &InputState::a,           12},
    { &InputState::c_right,     6 },
};
size_t brook_button_count = sizeof(brook_button_mappings) / sizeof(GpioButtonMapping);

static int BROOK_DPAD_UP_PIN = 17;
static int BROOK_L3_PIN = 30;

Pinout pinout = {
    .joybus_data = 7,
    .mux = A4,
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

    // Hold start on plugin for Brook board mode.
    pinMode(pinout.mux, OUTPUT);
    if (button_holds.start) {
        digitalWrite(pinout.mux, HIGH);
        use_brook_mode = true;
        // Create array of input sources to be used for brook.
        static InputSource *brook_input_sources[] = {
            new GpioButtonInput(brook_button_mappings, brook_button_count)
        };
        size_t brook_input_source_count = sizeof(brook_input_sources) / sizeof(InputSource *);

        pinMode(BROOK_DPAD_UP_PIN, OUTPUT);
        pinMode(BROOK_L3_PIN, OUTPUT);
        // If not DInput then only using 1 backend (no input viewer).
        backend_count = 1;
        backends = new CommunicationBackend *[backend_count] {
            new BrookPassthroughBackend(
                brook_input_sources,
                brook_input_source_count,
                BROOK_DPAD_UP_PIN,
                BROOK_L3_PIN
            )
        };
        // Brook Firmware takes control, so we can't control layout/gamemode/backend in this branch
        // IN Addition, you can force the following brook modes by holding the corresponding button on connecting.
        // If none are held, brook will auto-detect.
        // 1P/X = PS3
        // 2P/Y = PS4
        // 3P/RB = XID-PC
        // 4P/LB = Nintendo Switch
        // These listed buttons correspond to the mapping in brook mode (so can't be remapped)
        // So in the case of the default layout for lbx these correspond to R, Y, LightShield, MidShield
        // 
        return;
    }

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
        if (button_holds.a) {
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
    primary_backend->SetGameMode(new Melee20Button(socd::SOCD_2IP_NO_REAC));
}

void loop() {
    if (!use_brook_mode) {
        select_mode(backends[0]);
        if (current_kb_mode != nullptr) {
            current_kb_mode->SendReport(backends[0]->GetInputs());
        }
    }

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }
}
