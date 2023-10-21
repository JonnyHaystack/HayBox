#ifndef _CORE_MODE_SELECTION_HPP
#define _CORE_MODE_SELECTION_HPP

#include "core/CommunicationBackend.hpp"
#include "core/ControllerMode.hpp"
#include "core/KeyboardMode.hpp"

#include <config.pb.h>

extern KeyboardMode *current_kb_mode;

void set_mode(CommunicationBackend *backend, ControllerMode *mode);
void set_mode(CommunicationBackend *backend, KeyboardMode *mode);
void set_mode(
    CommunicationBackend *backend,
    const GameModeConfig &mode_config,
    const KeyboardModeConfig *keyboard_modes,
    size_t keyboard_modes_count
);
void set_mode(
    CommunicationBackend *backend,
    GameModeId mode_id,
    const GameModeConfig *mode_configs,
    size_t mode_configs_count,
    const KeyboardModeConfig *keyboard_modes,
    size_t keyboard_modes_count
);
void select_mode(
    CommunicationBackend *backend,
    const GameModeConfig *mode_configs,
    size_t mode_configs_count,
    const KeyboardModeConfig *keyboard_modes,
    size_t keyboard_modes_count
);
void setup_mode_activation_bindings(const GameModeConfig *mode_configs, size_t mode_configs_count);

#endif
