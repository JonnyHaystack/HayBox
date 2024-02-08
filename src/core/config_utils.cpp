#include "core/config_utils.hpp"

#include "core/state.hpp"
#include "util/state_util.hpp"

#include <config.pb.h>

CommunicationBackendConfig backend_config_from_buttons(
    const InputState &inputs,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
) {
    for (size_t i = 0; i < backend_configs_count; i++) {
        const CommunicationBackendConfig &backend_config = backend_configs[i];
        // Build bit mask for checking for matching button hold.
        uint64_t button_hold_mask = make_button_mask(
            backend_config.activation_binding,
            backend_config.activation_binding_count
        );

        // Check if button hold matches.
        if (all_buttons_held(inputs.buttons, button_hold_mask)) {
            return backend_config;
        }
    }

    return CommunicationBackendConfig{
        .backend_id = COMMS_BACKEND_UNSPECIFIED,
        .default_mode_config = 0,
    };
}

CommunicationBackendConfig backend_config_from_id(
    CommunicationBackendId backend_id,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
) {
    for (size_t i = 0; i < backend_configs_count; i++) {
        const CommunicationBackendConfig &backend_config = backend_configs[i];
        if (backend_config.backend_id == backend_id) {
            return backend_config;
        }
    }

    return CommunicationBackendConfig{
        .backend_id = backend_id,
        .default_mode_config = 0,
    };
}

uint8_t backend_config_id_from_backend_id(
    CommunicationBackendId backend_id,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
) {
    for (size_t i = 0; i < backend_configs_count; i++) {
        const CommunicationBackendConfig &backend_config = backend_configs[i];
        if (backend_config.backend_id == backend_id) {
            return i + 1;
        }
    }

    return 0;
}

uint8_t mode_config_id_from_mode_id(
    GameModeId mode_id,
    const GameModeConfig *mode_configs,
    size_t mode_configs_count
) {
    for (size_t i = 0; i < mode_configs_count; i++) {
        const GameModeConfig &mode = mode_configs[i];
        if (mode.mode_id == mode_id) {
            return i + 1;
        }
    }

    return 0;
}