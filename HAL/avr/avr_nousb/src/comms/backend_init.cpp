#include "comms/B0XXInputViewer.hpp"
#include "comms/ConfiguratorBackend.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "comms/backend_init.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/mode_selection.hpp"
#include "core/pinout.hpp"

#include <TUGamepad.hpp>
#include <TUKeyboard.hpp>
#include <config.pb.h>

size_t initialize_backends(
    CommunicationBackend **&backends,
    const CommunicationBackendConfig &backend_config,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const Pinout &pinout,
    Config &config
) {
    GameModeId mode_id = backend_config.default_mode;
    if (backend_config.default_mode == MODE_UNSPECIFIED) {
        mode_id = MODE_MELEE;
    }

    CommunicationBackend *primary_backend;
    size_t backend_count;

    switch (backend_config.backend_id) {
        case COMMS_BACKEND_DINPUT:
            TUGamepad::registerDescriptor();
            TUKeyboard::registerDescriptor();
            primary_backend = new DInputBackend(inputs, input_sources, input_source_count);
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(inputs, input_sources, input_source_count)
            };
            break;
        case COMMS_BACKEND_XINPUT:
            primary_backend = new XInputBackend(inputs, input_sources, input_source_count);
            backend_count = 2;
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(inputs, input_sources, input_source_count)
            };
            break;
        case COMMS_BACKEND_GAMECUBE:
            primary_backend =
                new GamecubeBackend(inputs, input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_N64:
            primary_backend =
                new N64Backend(inputs, input_sources, input_source_count, pinout.joybus_data);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_NINTENDO_SWITCH:
            NintendoSwitchBackend::RegisterDescriptor();
            primary_backend = new NintendoSwitchBackend(inputs, input_sources, input_source_count);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            break;
        case COMMS_BACKEND_UNSPECIFIED: // Fall back to configurator if invalid backend selected.
        case COMMS_BACKEND_CONFIGURATOR:
        default:
            primary_backend =
                new ConfiguratorBackend(inputs, input_sources, input_source_count, config);
            backend_count = 1;
            backends = new CommunicationBackend *[backend_count] { primary_backend };
    }

    set_mode(primary_backend, mode_id);

    return backend_count;
}