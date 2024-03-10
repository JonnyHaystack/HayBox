#include "comms/backend_init.hpp"

#include "comms/B0XXInputViewer.hpp"
#include "comms/ConfiguratorBackend.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NesBackend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/SnesBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/config_utils.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"

#include <TUGamepad.hpp>
#include <TUKeyboard.hpp>
#include <config.pb.h>

size_t initialize_backends(
    CommunicationBackend **&backends,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout,
    backend_config_selector_t get_backend_config,
    usb_backend_getter_t get_usb_backend_config,
    detect_console_t detect_console,
    secondary_backend_initializer_t init_secondary_backends,
    primary_backend_initializer_t init_primary_backend
) {
    // Make sure required function pointers are not null.
    if (get_backend_config == nullptr || get_usb_backend_config == nullptr ||
        init_primary_backend == nullptr || detect_console == nullptr) {
        return 0;
    }

    CommunicationBackendConfig backend_config = CommunicationBackendConfig_init_zero;
    get_backend_config(backend_config, inputs, config);

    CommunicationBackend *primary_backend = nullptr;

    /* If no match found for button hold, use console/USB detection to select backend instead. */
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        /* Must check default USB backend here and initialize it before console detection, so that
         * we can respond correctly to device descriptor requests from host. */
        CommunicationBackendConfig usb_backend_config;
        get_usb_backend_config(usb_backend_config, config);
        init_primary_backend(
            primary_backend,
            usb_backend_config.backend_id,
            inputs,
            input_sources,
            input_source_count,
            config,
            pinout
        );
        CommunicationBackendId detected_backend_id = COMMS_BACKEND_UNSPECIFIED;
        detected_backend_id = detect_console(pinout);
        if (detected_backend_id == COMMS_BACKEND_XINPUT) {
            backend_config = usb_backend_config;
        } else {
            backend_config = backend_config_from_id(
                detected_backend_id,
                config.communication_backend_configs,
                config.communication_backend_configs_count
            );
        }
    }
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED &&
        config.default_backend_config > 0) {
        backend_config = config.communication_backend_configs[config.default_backend_config - 1];
    }

    init_primary_backend(
        primary_backend,
        backend_config.backend_id,
        inputs,
        input_sources,
        input_source_count,
        config,
        pinout
    );

    size_t backend_count = 1;
    if (init_secondary_backends != nullptr) {
        backend_count = init_secondary_backends(
            backends,
            primary_backend,
            backend_config.backend_id,
            inputs,
            input_sources,
            input_source_count,
            config,
            pinout
        );
    }

    if (backend_config.default_mode_config > 0) {
        GameModeConfig &mode_config =
            config.game_mode_configs[backend_config.default_mode_config - 1];
        for (size_t i = 0; i < backend_count; i++) {
            set_mode(backends[i], mode_config, config);
        }
    }

    return backend_count;
}

void init_primary_backend(
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
) {
    switch (backend_id) {
        case COMMS_BACKEND_DINPUT:
            if (primary_backend == nullptr) {
                TUGamepad::registerDescriptor();
                TUKeyboard::registerDescriptor();
                primary_backend = new DInputBackend(inputs, input_sources, input_source_count);
            }
            break;
        case COMMS_BACKEND_NINTENDO_SWITCH:
            if (primary_backend == nullptr) {
                NintendoSwitchBackend::RegisterDescriptor();
                primary_backend =
                    new NintendoSwitchBackend(inputs, input_sources, input_source_count);
            }
            break;
        case COMMS_BACKEND_XINPUT:
            if (primary_backend == nullptr) {
                primary_backend = new XInputBackend(inputs, input_sources, input_source_count);
            }
            break;
        case COMMS_BACKEND_GAMECUBE:
            delete primary_backend;
            primary_backend =
                new GamecubeBackend(inputs, input_sources, input_source_count, pinout.joybus_data);
            break;
        case COMMS_BACKEND_N64:
            delete primary_backend;
            primary_backend =
                new N64Backend(inputs, input_sources, input_source_count, pinout.joybus_data);
            break;
        case COMMS_BACKEND_NES:
            delete primary_backend;
            primary_backend = new NesBackend(
                inputs,
                input_sources,
                input_source_count,
                pinout.nes_data,
                pinout.nes_clock,
                pinout.nes_latch
            );
            break;
        case COMMS_BACKEND_SNES:
            delete primary_backend;
            primary_backend = new SnesBackend(
                inputs,
                input_sources,
                input_source_count,
                pinout.nes_data,
                pinout.nes_clock,
                pinout.nes_latch
            );
            break;
        case COMMS_BACKEND_UNSPECIFIED: // Fall back to configurator if invalid backend selected.
        case COMMS_BACKEND_CONFIGURATOR:
        default:
            delete primary_backend;
            primary_backend =
                new ConfiguratorBackend(inputs, input_sources, input_source_count, config);
    }
}

size_t init_secondary_backends(
    CommunicationBackend **&backends,
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
) {
    size_t backend_count = 0;

    switch (backend_id) {
        case COMMS_BACKEND_DINPUT:
        case COMMS_BACKEND_XINPUT:
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(inputs, input_sources, input_source_count)
            };
            break;
        default:
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    return backend_count;
}

// clang-format off

/* Default is to first check button holds for a matching comms backend config. */
backend_config_selector_t get_backend_config_default = [](
    CommunicationBackendConfig &backend_config,
    const InputState &inputs,
    const Config &config
) {
    backend_config = backend_config_from_buttons(
        inputs,
        config.communication_backend_configs,
        config.communication_backend_configs_count
    );
};

/* Default is to get default USB backend from config. */
usb_backend_getter_t get_usb_backend_config_default = [](
    CommunicationBackendConfig &backend_config,
    const Config &config
) {
    if (config.default_usb_backend_config > 0 &&
        config.default_usb_backend_config <= config.communication_backend_configs_count) {
        backend_config =
            config.communication_backend_configs[config.default_usb_backend_config - 1];
    }
};

// clang-format on

primary_backend_initializer_t init_primary_backend_default = &init_primary_backend;
secondary_backend_initializer_t init_secondary_backends_default = &init_secondary_backends;