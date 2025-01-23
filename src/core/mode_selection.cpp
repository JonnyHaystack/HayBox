#include "core/mode_selection.hpp"

#include "core/state.hpp"
#include "modes/CustomControllerMode.hpp"
#include "modes/CustomKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"
#include "modes/Rivals2.hpp"
#include "modes/RivalsOfAether.hpp"
#include "modes/Ultimate.hpp"
#include "util/state_util.hpp"

#include <config.pb.h>

Melee20Button melee_mode;
ProjectM projectm_mode;
Ultimate ultimate_mode;
FgcMode fgc_mode;
RivalsOfAether rivals_mode;
Rivals2 rivals2_mode;
CustomKeyboardMode keyboard_mode;
CustomControllerMode custom_mode;

uint64_t mode_activation_masks[10];

size_t current_mode_index = SIZE_MAX;

void set_mode(CommunicationBackend *backend, ControllerMode *mode) {
    // Delete keyboard mode in case one is set, so we don't end up getting both controller and
    // keyboard inputs.
    current_kb_mode = nullptr;

    // Set new controller mode.
    backend->SetGameMode(mode);
}

void set_mode(CommunicationBackend *backend, KeyboardMode *mode) {
    // Only DInputBackend supports keyboard modes.
    if (backend->BackendId() != COMMS_BACKEND_DINPUT) {
        return;
    }

    // Delete and reassign current keyboard mode.
    current_kb_mode = mode;

    // Unset the current controller mode so backend only gives neutral inputs.
    backend->SetGameMode(mode);
}

void set_mode(CommunicationBackend *backend, GameModeConfig &mode_config, Config &config) {
    switch (mode_config.mode_id) {
        case MODE_MELEE:
            melee_mode.SetConfig(mode_config, config.melee_options);
            set_mode(backend, &melee_mode);
            break;
        case MODE_PROJECT_M:
            projectm_mode.SetConfig(mode_config, config.project_m_options);
            set_mode(backend, &projectm_mode);
            break;
        case MODE_ULTIMATE:
            ultimate_mode.SetConfig(mode_config);
            set_mode(backend, &ultimate_mode);
            break;
        case MODE_FGC:
            fgc_mode.SetConfig(mode_config);
            set_mode(backend, &fgc_mode);
            break;
        case MODE_RIVALS_OF_AETHER:
            rivals_mode.SetConfig(mode_config);
            set_mode(backend, &rivals_mode);
            break;
        case MODE_RIVALS_2:
            rivals2_mode.SetConfig(mode_config);
            set_mode(backend, &rivals2_mode);
            break;
        case MODE_KEYBOARD:
            if (backend->BackendId() != COMMS_BACKEND_DINPUT ||
                mode_config.keyboard_mode_config < 1 ||
                mode_config.keyboard_mode_config > config.keyboard_modes_count) {
                break;
            }
            keyboard_mode.SetConfig(
                mode_config,
                config.keyboard_modes[mode_config.keyboard_mode_config - 1]
            );
            set_mode(backend, &keyboard_mode);
            break;
        case MODE_CUSTOM:
            if (mode_config.custom_mode_config < 1 ||
                mode_config.custom_mode_config > config.custom_modes_count) {
                break;
            }
            custom_mode.SetConfig(
                mode_config,
                config.custom_modes[mode_config.custom_mode_config - 1]
            );
            set_mode(backend, &custom_mode);
            break;
        case MODE_UNSPECIFIED:
        default:
            break;
    }
}

// TODO: Maybe remove this overload in favour of looking up the gamemode outside of here using a
// config_utils function.
void set_mode(CommunicationBackend *backend, GameModeId mode_id, Config &config) {
    // In this overload we only know the mode id so we need to find a mode config that matches this
    // ID.
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode = config.game_mode_configs[i];
        if (mode.mode_id == mode_id) {
            set_mode(backend, mode, config);
            return;
        }
    }
}

void select_mode(CommunicationBackend **backends, size_t backends_count, Config &config) {
    // TODO: Use a counter variable to only run the contents of this function every x iterations
    // rather than on every single poll.

    InputState &inputs = backends[0]->GetInputs();

    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        if (all_buttons_held(inputs.buttons, mode_activation_masks[i]) && i != current_mode_index) {
            current_mode_index = i;
            for (size_t i = 0; i < backends_count; i++) {
                set_mode(backends[i], mode_config, config);
            }
            return;
        }
    }
}

void setup_mode_activation_bindings(const GameModeConfig *mode_configs, size_t mode_configs_count) {
    // Build bit masks for checking for matching button holds.
    for (size_t i = 0; i < mode_configs_count; i++) {
        mode_activation_masks[i] = make_button_mask(
            mode_configs[i].activation_binding,
            mode_configs[i].activation_binding_count
        );
    }
}
