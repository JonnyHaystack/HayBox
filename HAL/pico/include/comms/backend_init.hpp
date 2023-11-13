#ifndef _COMMS_BACKEND_INIT_HPP
#define _COMMS_BACKEND_INIT_HPP

#include "comms/console_detection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "core/pinout.hpp"
#include "core/state.hpp"

#include <config.pb.h>

// clang-format off

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
typedef void (*backend_config_selector_t)(
    CommunicationBackendConfig &backend_config,
    const InputState &inputs,
    const Config &config
);

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
// bool usb_backend_config_custom(CommunicationBackendConfig &backend_config, const Config &config)
//     __attribute__((weak));
typedef void (*usb_backend_getter_t)(
    CommunicationBackendConfig &backend_config,
    const Config &config
);

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
typedef void (*primary_backend_initializer_t)(
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
);

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
typedef size_t (*secondary_backend_initializer_t)(
    CommunicationBackend **&backends,
    CommunicationBackend *&primary_backend,
    CommunicationBackendId backend_id,
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    const Pinout &pinout
);

typedef CommunicationBackendId (*detect_console_t)(const Pinout &pinout);

// clang-format on

extern backend_config_selector_t get_backend_config_default;
extern usb_backend_getter_t get_usb_backend_config_default;
extern primary_backend_initializer_t init_primary_backend_default;
extern secondary_backend_initializer_t init_secondary_backends_default;

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
    const Pinout &pinout,
    backend_config_selector_t get_backend_config_custom = get_backend_config_default,
    usb_backend_getter_t get_usb_backend_config = get_usb_backend_config_default,
    detect_console_t detect_console = &detect_console,
    secondary_backend_initializer_t init_secondary_backends = init_secondary_backends_default,
    primary_backend_initializer_t init_primary_backend = init_primary_backend_default
);

#endif