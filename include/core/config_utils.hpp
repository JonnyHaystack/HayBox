#ifndef _CORE_CONFIG_UTILS_HPP
#define _CORE_CONFIG_UTILS_HPP

#include "core/state.hpp"

#include <config.pb.h>

CommunicationBackendConfig backend_config_from_buttons(
    const InputState &inputs,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
);

CommunicationBackendConfig backend_config_from_id(
    CommunicationBackendId backend_id,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
);

#endif