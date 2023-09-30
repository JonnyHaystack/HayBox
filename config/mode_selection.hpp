#ifndef _CONFIG_MODE_SELECTION_HPP
#define _CONFIG_MODE_SELECTION_HPP

#include "core/state.hpp"
#include "modes/DefaultKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"
#include "modes/RivalsOfAether.hpp"
#include "modes/Ultimate.hpp"

#include <config.pb.h>

extern Config config;
extern KeyboardMode *current_kb_mode;

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

void set_mode(CommunicationBackend *backend, GameModeConfig &mode_config) {
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
        case MODE_UNSPECIFIED:
        default:
            break;
    }
}

void set_mode(CommunicationBackend *backend, GameModeId mode_id) {
    // In this overload we only know the mode id so we need to find a mode config that matches this
    // ID.
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode = config.game_mode_configs[i];
        if (mode.mode_id == mode_id) {
            set_mode(backend, mode);
            return;
        }
    }
}

void select_mode(CommunicationBackend *backend) {
    // TODO: Use a counter variable to only run the contents of this function every x iterations
    // rather than on every single poll.

    InputState &inputs = backend->GetInputs();

    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        // TODO: Cache gamemode activation binding masks globally so they aren't rebuilt in main
        // loop.
        // Build bit mask for checking for matching button hold.
        uint64_t activation_binding_mask =
            make_button_mask(mode_config.activation_binding, mode_config.activation_binding_count);

        if (all_buttons_held(inputs.buttons, activation_binding_mask)) {
            set_mode(backend, mode_config);
            return;
        }
    }

    // if (inputs.mod_x && !inputs.mod_y && inputs.start) {
    //     if (inputs.l) {
    //         set_mode(
    //             backend,
    //             new Melee20Button(socd::SOCD_2IP_NO_REAC, { .crouch_walk_os = false })
    //         );
    //     } else if (inputs.left) {
    //         set_mode(
    //             backend,
    //             new ProjectM(
    //                 socd::SOCD_2IP_NO_REAC,
    //                 { .true_z_press = false, .ledgedash_max_jump_traj = true }
    //             )
    //         );
    //     } else if (inputs.down) {
    //         set_mode(backend, new Ultimate(socd::SOCD_2IP));
    //     } else if (inputs.right) {
    //         set_mode(backend, new FgcMode(socd::SOCD_NEUTRAL, socd::SOCD_NEUTRAL));
    //     } else if (inputs.b) {
    //         set_mode(backend, new RivalsOfAether(socd::SOCD_2IP));
    //     }
    // } else if (inputs.mod_y && !inputs.mod_x && inputs.start) {
    //     if (inputs.l) {
    //         set_mode(backend, new DefaultKeyboardMode(socd::SOCD_2IP));
    //     }
    // }
}

#endif
