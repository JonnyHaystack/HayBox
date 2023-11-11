#include "comms/backend_init.hpp"

#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/console_detection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/config_utils.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"

#include <config.pb.h>

size_t initialize_backends(
    CommunicationBackend **&backends,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const Config &config,
    const Pinout &pinout
) {
    CommunicationBackend *primary_backend = nullptr;

    CommunicationBackendConfig backend_config = backend_config_from_buttons(
        inputs,
        config.communication_backend_configs,
        config.communication_backend_configs_count
    );
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        primary_backend = new DInputBackend(inputs, input_sources, input_source_count);
        CommunicationBackendId detected_backend_id = detect_console(pinout);
        backend_config = backend_config_from_id(
            detected_backend_id,
            config.communication_backend_configs,
            config.communication_backend_configs_count
        );
    }
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED &&
        config.default_backend_config > 0) {
        backend_config = config.communication_backend_configs[config.default_backend_config - 1];
    }

    size_t backend_count;

    switch (backend_config.backend_id) {
        case COMMS_BACKEND_GAMECUBE:
            delete primary_backend;
            primary_backend = new GamecubeBackend(
                inputs,
                input_sources,
                input_source_count,
                inputs.rt1 ? 0 : 125,
                pinout.joybus_data
            );
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_N64:
            delete primary_backend;
            primary_backend =
                new N64Backend(inputs, input_sources, input_source_count, 60, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_DINPUT:
        default:
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(inputs, input_sources, input_source_count)
            };
            break;
    }

    if (backend_config.default_mode_config > 0) {
        const GameModeConfig &mode_config =
            config.game_mode_configs[backend_config.default_mode_config - 1];
        set_mode(primary_backend, mode_config, config.keyboard_modes, config.keyboard_modes_count);
    }

    return backend_count;
}