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
 * @param backend_configs Array of backend configs to select from
 * @param backend_configs_count Number of elements in the backend_configs array
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 * @return size_t The number of backends in the array
 */
size_t initialize_backends(
    CommunicationBackend **&backends,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count,
    const GameModeConfig *game_mode_configs,
    size_t game_mode_configs_count,
    const KeyboardModeConfig *keyboard_modes,
    size_t keyboard_modes_count,
    const Pinout &pinout
);

#endif