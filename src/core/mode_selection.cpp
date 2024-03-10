#include "core/mode_selection.hpp"

#include "core/state.hpp"
#include "modes/CustomControllerMode.hpp"
#include "modes/CustomKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"
#include "modes/RivalsOfAether.hpp"
#include "modes/Ultimate.hpp"
#include "util/state_util.hpp"

#include <config.pb.h>

uint64_t mode_activation_masks[10];

size_t current_mode_index = SIZE_MAX;

void set_mode(CommunicationBackend *backend, ControllerMode *mode) {
    // Delete keyboard mode in case one is set, so we don't end up getting both controller and
    // keyboard inputs.
    delete current_kb_mode;
    current_kb_mode = nullptr;

    // Set new controller mode.
    backend->SetGameMode(mode);
}

void set_mode(CommunicationBackend *backend, KeyboardMode *mode) {
    // Delete and reassign current keyboard mode.
    delete current_kb_mode;
    current_kb_mode = mode;

    // Unset the current controller mode so backend only gives neutral inputs.
    backend->SetGameMode(nullptr);
}

void set_mode(CommunicationBackend *backend, GameModeConfig &mode_config, Config &config) {
    switch (mode_config.mode_id) {
        case MODE_MELEE:
            set_mode(backend, new Melee20Button(mode_config, { .crouch_walk_os = false }));
            break;
        case MODE_PROJECT_M:
            set_mode(
                backend,
                new ProjectM(
                    mode_config,
                    { .true_z_press = false, .ledgedash_max_jump_traj = true }
                )
            );
            break;
        case MODE_ULTIMATE:
            set_mode(backend, new Ultimate(mode_config));
            break;
        case MODE_FGC:
            set_mode(backend, new FgcMode(mode_config));
            break;
        case MODE_RIVALS_OF_AETHER:
            set_mode(backend, new RivalsOfAether(mode_config));
            break;
        case MODE_KEYBOARD:
            if (mode_config.keyboard_mode_config < 1 ||
                mode_config.keyboard_mode_config > config.keyboard_modes_count) {
                break;
            }
            set_mode(
                backend,
                new CustomKeyboardMode(
                    mode_config,
                    config.keyboard_modes[mode_config.keyboard_mode_config - 1]
                )
            );
            break;
        case MODE_CUSTOM:
            if (mode_config.custom_mode_config < 1 ||
                mode_config.custom_mode_config > config.custom_modes_count) {
                break;
            }
            set_mode(
                backend,
                new CustomControllerMode(
                    mode_config,
                    config.custom_modes[mode_config.custom_mode_config - 1]
                )
            );
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