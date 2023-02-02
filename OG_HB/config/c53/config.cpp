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
#include "input/SwitchMatrixInput.hpp"
#include "joybus_utils.hpp"
#include "modes/Melee20Button.hpp"
#include "stdlib.hpp"

#include <pico/bootrom.h>

CommunicationBackend **backends;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

const size_t num_rows = 5;
const size_t num_cols = 13;
uint row_pins[num_rows] = { 20, 19, 18, 17, 16 };
uint col_pins[num_cols] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
// clang-format off
SwitchMatrixElement matrix[num_rows][num_cols] = {
    {NA,      NA,        NA,         NA,         NA, BTN(select), BTN(start), BTN(home), NA, BTN(r),      BTN(y),    BTN(lightshield), BTN(midshield)},
    { BTN(l), BTN(left), BTN(down),  BTN(right), NA, NA,          NA,         NA,        NA, BTN(b),      BTN(x),    BTN(z),           BTN(up)       },
    { NA,     NA,        NA,         NA,         NA, NA,          NA,         NA,        NA, NA,          NA,        NA,               NA            },
    { NA,     NA,        NA,         NA,         NA, NA,          NA,         NA,        NA, BTN(c_left), BTN(c_up), BTN(c_right),     NA            },
    { NA,     NA,        BTN(mod_x), BTN(mod_y), NA, NA,          NA,         NA,        NA, BTN(c_down), BTN(a),    NA,               NA            },
};
// clang-format on
DiodeDirection diode_direction = DiodeDirection::COL2ROW;

const Pinout pinout = {
    .joybus_data = 22,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void setup() {
    // Create switch matrix input source and use it to read button states for checking button holds.
    SwitchMatrixInput<num_rows, num_cols> *matrix_input =
        new SwitchMatrixInput<num_rows, num_cols>(row_pins, col_pins, matrix, diode_direction);

    InputState button_holds;
    matrix_input->UpdateInputs(button_holds);

    // Bootsel button hold as early as possible for safety.
    if (button_holds.start) {
        reset_usb_boot(0, 0);
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { matrix_input };
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

            // Default to Ultimate mode on Switch.
            primary_backend->SetGameMode(new Ultimate(socd::SOCD_2IP));
            return;
        } else if (button_holds.z) {
            // If no console detected and Z is held on plugin then use DInput backend.
            TUGamepad::registerDescriptor();
            TUKeyboard::registerDescriptor();
            backend_count = 2;
            primary_backend = new DInputBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
        } else {
            // Default to XInput mode if no console detected and no other mode forced.
            backend_count = 2;
            primary_backend = new XInputBackend(input_sources, input_source_count);
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

        // If console then only using 1 backend (no input viewer).
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
