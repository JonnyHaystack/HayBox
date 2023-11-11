#include "comms/backend_init.hpp"

#include "comms/B0XXInputViewer.hpp"
#include "comms/ConfiguratorBackend.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "comms/console_detection.hpp"
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
    const Pinout &pinout
) {
    CommunicationBackend *primary_backend = nullptr;

    /* First check button holds for a matching comms backend config. */
    CommunicationBackendConfig backend_config = backend_config_from_buttons(
        inputs,
        config.communication_backend_configs,
        config.communication_backend_configs_count
    );

    // TODO: weakly defined backend_config_custom()

    /* If no match found for button hold, use console/USB detection to select backend instead. */
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        /* Must check default USB backend here and initialize it before console detection, so that
         * we can respond correctly to device descriptor requests from host. */
        const CommunicationBackendConfig &usb_backend_config =
            config.communication_backend_configs[config.default_usb_backend_config - 1];
        // TODO: weakly defined usb_backend_config_custom()
        init_primary_backend(
            primary_backend,
            usb_backend_config.backend_id,
            inputs,
            input_sources,
            input_source_count,
            config,
            pinout
        );
        // TODO: weakly defined detect_console_custom()
        CommunicationBackendId detected_backend_id = detect_console(pinout);
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

    size_t backend_count;

    // TODO: weakly defined init_primary_backend_custom()
    init_primary_backend(
        primary_backend,
        backend_config.backend_id,
        inputs,
        input_sources,
        input_source_count,
        config,
        pinout
    );
    // TODO: weakly defined init_secondary_backends_custom()
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

    if (backend_config.default_mode_config > 0) {
        const GameModeConfig &mode_config =
            config.game_mode_configs[backend_config.default_mode_config - 1];
        set_mode(primary_backend, mode_config, config.keyboard_modes, config.keyboard_modes_count);
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
        default:
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    return backend_count;
}