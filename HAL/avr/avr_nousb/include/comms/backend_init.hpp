#ifndef _COMMS_BACKEND_INIT_HPP
#define _COMMS_BACKEND_INIT_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"

#include <config.pb.h>

/**
 * @brief Initialize the backends array and return the number of elements in the array
 *
 * @param backends The reference to assign to the created backends array
 * @param backend_config The selected backend config that we want to map to an array of
 * communication backends
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 * @param config Reference to global config struct, to be used by ConfiguratorBackend
 * @return size_t The number of backends in the array
 */
size_t initialize_backends(
    CommunicationBackend **&backends,
    const CommunicationBackendConfig &backend_config,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const Pinout &pinout
);

#endif