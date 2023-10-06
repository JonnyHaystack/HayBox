#ifndef _CORE_CONFIG_UTILS_HPP
#define _CORE_CONFIG_UTILS_HPP

#include "core/state.hpp"

#include <config.pb.h>

CommunicationBackendConfig backend_config_from_buttons(
    const InputState &inputs,
    const Config &config
);

CommunicationBackendConfig backend_config_from_id(
    CommunicationBackendId backend_id,
    const Config &config
);

#endif