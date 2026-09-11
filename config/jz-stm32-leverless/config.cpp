#include "comms/backend_init.hpp"
#include "config_defaults.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/KeyboardMode.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "reboot.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

Config config = default_config;

// Button-to-pin mapping for jz-stm32-leverless (FGC mode).
// Pin assignments verified via stm32pintester 2026-03-12.
//
// Physical layout:
//   Left hand:                          Right hand:
//   B1(Start) B2(Sel)  B3(Home)         B18(X)  B19(Y)  B20(RB) B21(LB)
//   B4  B5  B6                          B22(A)  B23(B)  B24(RT) B25(LT)
//   B7(L3) B8(Left) B9(Down) B10(Right) B26(Up) B27(R3)
//
// Shared pins: B5/B12→PA7 (26 unique pins, 27 buttons)

const GpioButtonMapping button_mappings[] = {
    // Top row — Start / Select / Home
    { BTN_MB1,  PC0  },  // B1  - Start
    { BTN_RT3,  PC2  },  // B2  - Select
    { BTN_RT2,  PA4  },  // B3  - Home (also bootloader entry)
    { BTN_MB1,  PC8  },  // B14 - Start (same as B1)
    { BTN_RT3,  PC9  },  // B15 - Select (same as B2)
    { BTN_RT2,  PC10 },  // B16 - Home (same as B3)

    // Left hand — movement
    { BTN_LT1,  PA7  },  // B5  - DPad Up (also PA7/B12)
    { BTN_LT2,  PB9  },  // B7  - L3
    { BTN_LT2,  PB15 },  // B11 - L3 (same as B7)
    { BTN_LF3,  PB11 },  // B8  - DPad Left
    { BTN_LF2,  PB1  },  // B9  - DPad Down
    { BTN_LF1,  PC5  },  // B10 - DPad Right

    // Right hand — attack buttons
    { BTN_RF5,  PA5  },  // B18 - X
    { BTN_RF6,  PA3  },  // B19 - Y
    { BTN_RF7,  PA1  },  // B20 - RB
    { BTN_RF8,  PC3  },  // B21 - LB
    { BTN_RF1,  PC12 },  // B22 - A
    { BTN_RF2,  PB13 },  // B23 - B
    { BTN_RF3,  PC6  },  // B24 - RT
    { BTN_RF4,  PB14 },  // B25 - LT

    // Bottom row
    { BTN_LT1,  PC7  },  // B26 - DPad Up
    { BTN_RT1,  PA0  },  // B27 - R3
    { BTN_RT1,  PB12 },  // B13 - R3 (same as B27)
};
const size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = {
    .joybus_data = 0,
    .nes_data = -1,
    .nes_clock = -1,
    .nes_latch = -1,
    .mux = -1,
    .nunchuk_detect = -1,
    .nunchuk_sda = -1,
    .nunchuk_scl = -1,
};

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GameModeConfig *find_fgc_config(Config &cfg);

void setup() {
    // Free PA15, PB3, PB4 from JTAG for GPIO use (keep SWD on PA13/PA14).
    afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);

    // PA8 is the matrix row driver — drive LOW to activate the diode-to-ground matrix.
    pinMode(PA8, OUTPUT);
    digitalWrite(PA8, LOW);

    static InputState inputs;

    static GpioButtonInput gpio_input(button_mappings, button_count);
    gpio_input.UpdateInputs(inputs);

    // Check bootloader button hold as early as possible.
    if (inputs.rt2) {
        reboot_bootloader();
    }

    static InputSource *input_sources[] = { &gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    // Default FGC vertical SOCD to Up priority (Dir1 = Up wins over Down).
    GameModeConfig *fgc = find_fgc_config(config);
    if (fgc && fgc->socd_pairs_count >= 2) {
        fgc->socd_pairs[1] = { BTN_LT1, BTN_LF2, SOCD_DIR1_PRIORITY };
    }

    backend_count =
        initialize_backends(backends, inputs, input_sources, input_source_count, config, pinout);

    setup_mode_activation_bindings(config.game_mode_configs, config.game_mode_configs_count);
}

GameModeConfig *find_fgc_config(Config &cfg) {
    for (size_t i = 0; i < cfg.game_mode_configs_count; i++) {
        if (cfg.game_mode_configs[i].mode_id == MODE_FGC) {
            return &cfg.game_mode_configs[i];
        }
    }
    return nullptr;
}

static SocdType next_socd(SocdType current) {
    switch (current) {
        case SOCD_DIR1_PRIORITY: return SOCD_NEUTRAL;
        case SOCD_NEUTRAL:       return SOCD_2IP;
        case SOCD_2IP:           return SOCD_2IP_NO_REAC;
        case SOCD_2IP_NO_REAC:   return SOCD_DIR1_PRIORITY;
        default:                 return SOCD_DIR1_PRIORITY;
    }
}

void loop() {
    select_mode(backends, backend_count, config);

    // SOCD toggle: Start (BTN_MB1) + Select (BTN_RT3) + Up (BTN_LT1)
    static bool combo_was_held = false;
    InputState &inputs = backends[0]->GetInputs();
    bool combo_held = inputs.mb1 && inputs.rt3 && inputs.lt1;

    if (combo_held && !combo_was_held) {
        GameModeConfig *fgc_cfg = find_fgc_config(config);
        if (fgc_cfg != nullptr && fgc_cfg->socd_pairs_count >= 2) {
            fgc_cfg->socd_pairs[1].socd_type = next_socd(fgc_cfg->socd_pairs[1].socd_type);
        }
    }
    combo_was_held = combo_held;

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}
