#include "comms/B0XXInputViewer.hpp"
#include "comms/ConfiguratorBackend.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/KeyboardMode.hpp"
#include "core/persistence.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "joybus_utils.hpp"
#include "modes/Melee20Button.hpp"
#include "stdlib.hpp"

#include <EEPROM.h>
#include <config.pb.h>
#include <pico/bootrom.h>

Config config = {
    .default_backend = COMMS_BACKEND_XINPUT,
    .game_mode_configs_count = 4,
    .game_mode_configs = {
        GameModeConfig {
            .mode_id = MODE_MELEE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_L },
        },
        GameModeConfig {
            .mode_id = MODE_PROJECT_M,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP_NO_REAC },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP_NO_REAC },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_LEFT },
        },
        GameModeConfig {
            .mode_id = MODE_ULTIMATE,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_UP, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_C_LEFT, .button_dir2 = BTN_C_RIGHT, .socd_type = SOCD_2IP },
                SocdPair { .button_dir1 = BTN_C_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_2IP },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_DOWN },
        },
        GameModeConfig {
            .mode_id = MODE_FGC,
            .socd_pairs_count = 4,
            .socd_pairs = {
                SocdPair { .button_dir1 = BTN_LEFT, .button_dir2 = BTN_RIGHT, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_MOD_X, .button_dir2 = BTN_C_UP, .socd_type = SOCD_DIR1_PRIORITY },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_MOD_X, .socd_type = SOCD_NEUTRAL },
                SocdPair { .button_dir1 = BTN_DOWN, .button_dir2 = BTN_C_UP, .socd_type = SOCD_NEUTRAL },
            },
            .button_remapping_count = 0,
            .activation_binding_count = 3,
            .activation_binding = { BTN_MOD_X, BTN_START, BTN_RIGHT },
        },
    },
    .communication_backend_configs_count = 3,
    .communication_backend_configs = {
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_XINPUT,
            .default_mode = MODE_MELEE,
            .activation_binding_count = 0,
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_DINPUT,
            .default_mode = MODE_MELEE,
            .activation_binding_count = 1,
            .activation_binding = { BTN_Z },
        },
        CommunicationBackendConfig {
            .backend_id = COMMS_BACKEND_CONFIGURATOR,
            .default_mode = MODE_UNSPECIFIED,
            .activation_binding_count = 1,
            .activation_binding = { BTN_C_DOWN },
        }
    },
};
CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GpioButtonMapping button_mappings[] = {
    {BTN_L,            5 },
    { BTN_LEFT,        4 },
    { BTN_DOWN,        3 },
    { BTN_RIGHT,       2 },

    { BTN_MOD_X,       6 },
    { BTN_MOD_Y,       7 },

    { BTN_SELECT,      10},
    { BTN_START,       0 },
    { BTN_HOME,        11},

    { BTN_C_LEFT,      13},
    { BTN_C_UP,        12},
    { BTN_C_DOWN,      15},
    { BTN_A,           14},
    { BTN_C_RIGHT,     16},

    { BTN_B,           26},
    { BTN_X,           21},
    { BTN_Z,           19},
    { BTN_UP,          17},

    { BTN_R,           27},
    { BTN_Y,           22},
    { BTN_LIGHTSHIELD, 20},
    { BTN_MIDSHIELD,   18},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 28,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

void set_comms_backend(
    CommunicationBackendId backend_id,
    GameModeId mode_id,
    InputSource **input_sources,
    size_t input_source_count
);

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

    EEPROM.begin(EEPROM_SIZE);
    if (!persistence::load_config(config)) {
        persistence::save_config(config);
    }

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        CommunicationBackendConfig &backend_config = config.communication_backend_configs[i];
        // Build bit mask for checking for matching button hold.
        uint64_t button_hold_mask = make_button_mask(
            backend_config.activation_binding,
            backend_config.activation_binding_count
        );

        // Check if button hold matches.
        if (all_buttons_held(button_holds.buttons, button_hold_mask)) {
            set_comms_backend(
                backend_config.backend_id,
                backend_config.default_mode,
                input_sources,
                input_source_count
            );
            return;
        }
    }

    // If no backend selected using button holds, run auto detection.
    ConnectedConsole console = detect_console(pinout.joybus_data);
    CommunicationBackendId backend_id = COMMS_BACKEND_UNSPECIFIED;
    switch (console) {
        case ConnectedConsole::GAMECUBE:
            backend_id = COMMS_BACKEND_GAMECUBE;
            break;
        case ConnectedConsole::N64:
            backend_id = COMMS_BACKEND_N64;
            break;
        case ConnectedConsole::NONE:
        default:
            backend_id = config.default_backend;
    }

    // No button hold was matched to a comms backend so we need to find a backend config that
    // matches the detected console so we can check what the configured default gamemode is.
    // Fall back to Melee mode if no other match found.
    GameModeId mode_id = MODE_MELEE;
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        CommunicationBackendConfig &backend_config = config.communication_backend_configs[i];
        if (backend_config.backend_id == backend_id) {
            mode_id = backend_config.default_mode;
        }
    }

    set_comms_backend(backend_id, mode_id, input_sources, input_source_count);
}

void set_comms_backend(
    CommunicationBackendId backend_id,
    GameModeId mode_id,
    InputSource **input_sources,
    size_t input_source_count
) {
    if (mode_id == MODE_UNSPECIFIED) {
        mode_id = MODE_MELEE;
    }

    CommunicationBackend *primary_backend;

    switch (backend_id) {
        case COMMS_BACKEND_DINPUT:
            TUGamepad::registerDescriptor();
            TUKeyboard::registerDescriptor();
            primary_backend = new DInputBackend(input_sources, input_source_count);
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
            break;
        case COMMS_BACKEND_XINPUT:
            primary_backend = new XInputBackend(input_sources, input_source_count);
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
            break;
        case COMMS_BACKEND_GAMECUBE:
            primary_backend =
                new GamecubeBackend(input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_N64:
            primary_backend = new N64Backend(input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_NINTENDO_SWITCH:
            NintendoSwitchBackend::RegisterDescriptor();
            primary_backend = new NintendoSwitchBackend(input_sources, input_source_count);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_UNSPECIFIED: // Fall back to configurator if invalid backend selected.
        case COMMS_BACKEND_CONFIGURATOR:
        default:
            primary_backend = new ConfiguratorBackend(input_sources, input_source_count);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    set_mode(primary_backend, mode_id);
}

#ifdef DEFINITELY_NOT_DEFINED
void old_main_stuff() {
    /* Select communication backend. */
    if (console == ConnectedConsole::NONE) {
        if (button_holds.x) {
            // If no console detected and X is held on plugin then use Switch USB backend.
            NintendoSwitchBackend::RegisterDescriptor();
            backend_count = 1;
            primary_backend = new NintendoSwitchBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] { primary_backend };

            // Default to Ultimate mode on Switch.
            set_mode(primary_backend, new Ultimate(socd::SOCD_2IP));
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
    set_mode(
        primary_backend,
        new Melee20Button(socd::SOCD_2IP_NO_REAC, { .crouch_walk_os = false })
    );
}
#endif

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
