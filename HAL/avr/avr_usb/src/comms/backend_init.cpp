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
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count,
    const GameModeConfig *game_mode_configs,
    size_t game_mode_configs_count,
    const Pinout &pinout
) {
    CommunicationBackend *primary_backend;

    CommunicationBackendConfig backend_config =
        backend_config_from_buttons(inputs, backend_configs, backend_configs_count);
    if (backend_config.backend_id == COMMS_BACKEND_UNSPECIFIED) {
        primary_backend = new DInputBackend(inputs, input_sources, input_source_count);
        delay(500);
        CommunicationBackendId detected_backend_id = detect_console(pinout);
        backend_config =
            backend_config_from_id(detected_backend_id, backend_configs, backend_configs_count);
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

    GameModeId mode_id = backend_config.default_mode;
    if (backend_config.default_mode == MODE_UNSPECIFIED) {
        mode_id = MODE_MELEE;
    }

    set_mode(primary_backend, mode_id, game_mode_configs, game_mode_configs_count);

    return backend_count;
}