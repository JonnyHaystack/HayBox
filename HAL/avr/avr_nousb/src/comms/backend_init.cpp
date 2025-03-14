#include "comms/backend_init.hpp"

#include "comms/B0XXInputViewer.hpp"
#include "comms/ConfiguratorBackend.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
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
    const Pinout &pinout
) {
    CommunicationBackend *primary_backend;

    CommunicationBackendConfig backend_config = backend_config_from_buttons(
        inputs,
        config.communication_backend_configs,
        config.communication_backend_configs_count
    );
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        backend_config = backend_config_from_id(
            COMMS_BACKEND_GAMECUBE,
            config.communication_backend_configs,
            config.communication_backend_configs_count
        );
    }
    // if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {}
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        backend_config = config.communication_backend_configs[config.default_backend_config];
    }

    size_t backend_count;

    switch (backend_config.backend_id) {
        case COMMS_BACKEND_N64:
            primary_backend =
                new N64Backend(inputs, input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_GAMECUBE:
        default:
            primary_backend =
                new GamecubeBackend(inputs, input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
    }

    const GameModeConfig &mode_config =
        config.game_mode_configs[backend_config.default_mode_config];

    set_mode(primary_backend, mode_config, config.keyboard_modes, config.keyboard_modes_count);

    return backend_count;
}