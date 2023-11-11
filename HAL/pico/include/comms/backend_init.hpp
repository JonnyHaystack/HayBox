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
 * @return The number of backends in the array
 */
size_t initialize_backends(
    CommunicationBackend **&backends,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
);

/**
 * @brief Optionally defined function that allows a device config to hook into initialize_backends()
 * and use a custom method for selecting backend config before console detection
 *
 * @param backend_config The reference to the current backend config after checking for button holds
 * and connected console
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param config Reference to the global Config struct
 * @return true if the passed in backend config was altered, otherwise false
 */
bool backend_config_custom(
    CommunicationBackendConfig &backend_config,
    const InputState &inputs,
    const Config &config
) __attribute__((weak));

/**
 * @brief Optionally defined function that allows a device config to hook into initialize_backends()
 * and use a custom method for selecting the default USB communication backend to initialize prior
 * to console/USB detection
 *
 * @param backend_config The reference to the current backend config after checking for button holds
 * and connected console
 * @param config Reference to the global Config struct
 * @return true if the passed in backend config was altered, otherwise false
 */
bool usb_backend_config_custom(CommunicationBackendConfig &backend_config, const Config &config)
    __attribute__((weak));

void detect_console_custom(Pinout &pinout);

/**
 * @brief Initialize primary backend based on the passed in backend id
 *
 * @param primary_backend The reference to the primary backend pointer to initialize
 * @param backend_id The id of the backend to initialize
 * @param backends The reference to assign to the created backends array
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param config Reference to global config struct
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 */
void init_primary_backend(
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
);

/**
 * @brief Optionally defined function that allows a device config to hook into initialize_backends()
 * and override initialization of primary backend
 *
 * @param primary_backend The reference to the primary backend pointer to initialize
 * @param backend_id The id of the backend to initialize
 * @param backends The reference to assign to the created backends array
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param config Reference to global config struct
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 */
void init_primary_backend_custom(
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
) __attribute__((weak));

/**
 * @brief Initialize secondary backends and the backends array and return the number of elements in
 * the array
 *
 * @param backends The reference to assign to the created backends array
 * @param primary_backend The reference to the primary backend to insert first in the backends array
 * @param backend_id The id indicating the primary backend
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param config Reference to global config struct
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 * @return The number of backends in the array
 */
size_t init_secondary_backends(
    CommunicationBackend **&backends,
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
);

/**
 * @brief Optionally defined function that allows a device config to hook into initialize_backends()
 * and override initialization of secondary backends
 *
 * @param backends The reference to assign to the created backends array
 * @param primary_backend The reference to the primary backend to insert first in the backends array
 * @param backend_id The id indicating the primary backend
 * @param inputs Reference to the InputState struct to pass into each backend's constructor
 * @param input_sources Input sources array to pass into each backend's constructor
 * @param input_source_count Number of elements in the input_sources array
 * @param config Reference to global config struct
 * @param pinout Pinout struct used for GameCube/N64 communication backends
 * @return The number of backends in the array
 */
size_t init_secondary_backends_custom(
    CommunicationBackend **&backends,
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
) __attribute__((weak));

#endif