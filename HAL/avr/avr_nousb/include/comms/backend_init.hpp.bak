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
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param config Reference to global config struct
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 * @return size_t The number of backends in the array
 */
size_t initialize_backends(
    CommunicationBackend **&backends,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const Config &config,
    const Pinout &pinout
);

bool initialize_backends_custom(
    CommunicationBackend **&backends,
    InputSource **input_sources,
    size_t input_source_count
) __attribute__((weak));

void select_backend_custom(CommunicationBackendConfig &backend_config) __attribute__((weak));

#endif