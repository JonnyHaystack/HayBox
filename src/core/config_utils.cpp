#include "core/config_utils.hpp"

#include "core/state.hpp"

#include <config.pb.h>

CommunicationBackendConfig backend_config_from_buttons(
    const InputState &inputs,
    const Config &config
) {
    const CommunicationBackendConfig *backend_configs = config.communication_backend_configs;
    size_t backend_configs_count = config.communication_backend_configs_count;
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
        .default_mode = MODE_UNSPECIFIED,
    };
}

CommunicationBackendConfig backend_config_from_id(
    CommunicationBackendId backend_id,
    const Config &config
) {
    const CommunicationBackendConfig *backend_configs = config.communication_backend_configs;
    size_t backend_configs_count = config.communication_backend_configs_count;
    // Find the first backend config that matches the detected console so we can check what the
    // configured default gamemode is.
    for (size_t i = 0; i < backend_configs_count; i++) {
        const CommunicationBackendConfig &backend_config = backend_configs[i];
        if (backend_config.backend_id == backend_id) {
            return backend_config;
        }
    }

    return CommunicationBackendConfig{
        .backend_id = backend_id,
        .default_mode = MODE_UNSPECIFIED,
    };
}