#include "comms/backend_init.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "display/ConfigMenu.hpp"
#include "display/DisplayMode.hpp"
#include "display/InputDisplay.hpp"
#include "display/RgbBrightnessMenu.hpp"
#include "glyph_overrides.hpp"
#include "input/DebouncedSwitchMatrixInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <Adafruit_SSD1306.h>
#include <config.pb.h>

Config config = glyph_default_config();

const size_t num_rows = 4;
const size_t num_cols = 11;
const uint row_pins[num_rows] = { 26, 25, 24, 23 };
const uint col_pins[num_cols] = { 15, 14, 13, 12, 16, 17, 21, 20, 19, 18, 22 };
// clang-format off
const Button matrix[num_rows][num_cols] = {
    {BTN_MB1,  BTN_MB2, BTN_MB3, BTN_MB4, BTN_MB5,  BTN_MB6,  BTN_MB7,  NA,      NA,      NA,      NA     },
    { BTN_LF3, BTN_LF2, BTN_LF6, BTN_LF7, BTN_RF12, BTN_RF13, BTN_RF14, BTN_RF5, BTN_RF6, BTN_RF7, BTN_RF8},
    { BTN_LF4, BTN_LF5, BTN_LF1, BTN_LF8, BTN_RF9,  BTN_RF10, BTN_RF11, BTN_RF1, BTN_RF2, BTN_RF3, BTN_RF4},
    { BTN_LT5, BTN_LT4, BTN_LT1, BTN_LT3, BTN_LT2,  BTN_LT6,  BTN_RT2,  BTN_RT3, BTN_RT1, BTN_RT4, BTN_RT5},
};
// clang-format on

DebouncedSwitchMatrixInput<num_rows, num_cols> matrix_input(
    row_pins,
    col_pins,
    matrix,
    DiodeDirection::COL2ROW
);

const Pinout pinout = {
    .joybus_data = 4,
    .nes_data = 4,
    .nes_clock = 5,
    .nes_latch = 6,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

InputState inputs;

InputSource *input_sources[] = { &matrix_input };
size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

void setup() {
    // Create GPIO input source and use it to read button states for checking button holds.
    matrix_input.UpdateInputs(inputs);

    // Check bootsel button hold as early as possible for safety.
    if (inputs.mb1) {
        reboot_bootloader();
    }

    // Attempt to load config, or write default config to flash if failed to load config.
    if (!persistence.LoadConfig(config)) {
        persistence.SaveConfig(config);
    }

    // Create array of input sources to be used.
    backend_count = initialize_backends(
        backends,
        inputs,
        input_sources,
        input_source_count,
        config,
        pinout,
        get_backend_config_default,
        get_usb_backend_config_default,
        &detect_console,
        &init_secondary_backends_glyph
    );

    setup_mode_activation_bindings(config.game_mode_configs, config.game_mode_configs_count);
}

void loop() {
    select_mode(backends, backend_count, config);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}

#define MENU_BUTTON_RADIUS 2
#define NORMAL_BUTTON_RADIUS 4
#define LARGE_BUTTON_RADIUS 5

InputViewerButton input_viewer_buttons[] = {
  // {BTN_MB1,  2,   3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB2, 8,   3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB3, 14,  3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB4, 20,  3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB5, 26,  3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB6, 32,  3,  MENU_BUTTON_RADIUS  },
  // { BTN_MB7, 38,  3,  MENU_BUTTON_RADIUS  },

    {BTN_LF4,  6,   29, NORMAL_BUTTON_RADIUS},
    { BTN_LF3, 15,  23, NORMAL_BUTTON_RADIUS},
    { BTN_LF2, 25,  22, NORMAL_BUTTON_RADIUS},
    { BTN_LF1, 35,  27, NORMAL_BUTTON_RADIUS},

    { BTN_RF1, 93,  27, NORMAL_BUTTON_RADIUS},
    { BTN_RF2, 102, 23, NORMAL_BUTTON_RADIUS},
    { BTN_RF3, 112, 24, NORMAL_BUTTON_RADIUS},
    { BTN_RF4, 122, 29, NORMAL_BUTTON_RADIUS},

    { BTN_RF5, 93,  17, NORMAL_BUTTON_RADIUS},
    { BTN_RF6, 102, 13, NORMAL_BUTTON_RADIUS},
    { BTN_RF7, 112, 14, NORMAL_BUTTON_RADIUS},
    { BTN_RF8, 122, 19, NORMAL_BUTTON_RADIUS},

    { BTN_LT1, 38,  52, NORMAL_BUTTON_RADIUS},
    { BTN_LT2, 46,  58, NORMAL_BUTTON_RADIUS},

    { BTN_RT1, 90,  52, NORMAL_BUTTON_RADIUS},
    { BTN_RT2, 82,  58, NORMAL_BUTTON_RADIUS},
    { BTN_RT3, 82,  46, NORMAL_BUTTON_RADIUS},
    { BTN_RT4, 90,  40, NORMAL_BUTTON_RADIUS},
    { BTN_RT5, 98,  46, NORMAL_BUTTON_RADIUS},
};
size_t input_viewer_buttons_count = count_of(input_viewer_buttons);

/* Second core handles OLED display */
Adafruit_SSD1306 display(128, 64, &Wire1);
IntegratedDisplay *display_backend = nullptr;

RgbBrightnessMenu rgb_brightness_menu(config);

void setup1() {
    while (!backend_count || backends == nullptr) {
        delay(1);
    }

    // These have to be initialized after backends.
    CommunicationBackendId primary_backend_id = backends[0]->BackendId();
    static InputDisplay input_display(
        input_viewer_buttons,
        input_viewer_buttons_count,
        primary_backend_id
    );
    static ConfigMenu config_menu(config, backends, backend_count);

    static DisplayMode *display_modes[] = { &input_display, &config_menu, &rgb_brightness_menu };
    size_t display_modes_count = count_of(display_modes);

    Wire1.setSDA(2);
    Wire1.setSCL(3);
    Wire1.setClock(1'000'000UL);
    Wire1.begin();
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
        // clang-format off
        display_backend = new IntegratedDisplay(
            inputs,
            display,
            []() { display.clearDisplay(); },
            []() { display.display(); },
            DisplayControls{ .back = BTN_MB4, .down = BTN_MB5, .up = BTN_MB6, .enter = BTN_MB7 },
            display_modes,
            display_modes_count
        );
        // clang-format on
    }
}

void loop1() {
    if (display_backend != nullptr) {
        if (display_backend->CurrentGameMode() != backends[0]->CurrentGameMode()) {
            display_backend->SetGameMode(backends[0]->CurrentGameMode());
        }
        display_backend->SendReport();
    }
}
