#ifndef _CONFIG_MODE_SELECTION_HPP
#define _CONFIG_MODE_SELECTION_HPP

#include "core/state.hpp"
#include "modes/DefaultKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/Melee20ButtonCustom.hpp"
#include "modes/ProjectM.hpp"
#include "modes/RivalsOfAether.hpp"
#include "modes/Ultimate.hpp"

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

void select_mode(CommunicationBackend *backend) {
    InputState &inputs = backend->GetInputs();
    if (inputs.mod_x && !inputs.mod_y && inputs.start && inputs.midshield) {
        if (inputs.l) { // Melee custom
            set_mode(backend, new Melee20ButtonCustom(socd::SOCD_2IP_NO_REAC));
        } else if (inputs.a) { // Melee default
            set_mode(backend, new Melee20Button(socd::SOCD_2IP_NO_REAC));
        } else if (inputs.left) { // PM
            set_mode(backend, new ProjectM(socd::SOCD_2IP_NO_REAC, true, false));
        } else if (inputs.down) { // Ultimate
            set_mode(backend, new Ultimate(socd::SOCD_2IP));
        } else if (inputs.right) { // FGC
            set_mode(backend, new FgcMode(socd::SOCD_NEUTRAL));
        } else if (inputs.b) {  // Rivals
            set_mode(backend, new RivalsOfAether(socd::SOCD_2IP));
        }
    } else if (inputs.mod_y && !inputs.mod_x && inputs.start && inputs.midshield) {
        if (inputs.l) {
            set_mode(backend, new DefaultKeyboardMode(socd::SOCD_2IP));
        }
    }
}

#endif
