#ifndef _CONFIG_MODE_SELECTION_HPP
#define _CONFIG_MODE_SELECTION_HPP

#include "core/state.hpp"
#include "modes/DefaultKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"

extern KeyboardMode *current_kb_mode;

void select_mode(CommunicationBackend **backends, uint8_t backend_count) {
    for (int i = 0; i < backend_count; i++) {
        InputState &inputs = backends[i]->CurrentInputs();
        if (inputs.mod_x && !inputs.mod_y && inputs.start) {
            if (inputs.l) {
                backends[i]->SetGameMode(new Melee20Button(socd::SOCD_2IP_NO_REAC));
            } else if (inputs.left) {
                backends[i]->SetGameMode(new ProjectM(socd::SOCD_2IP_NO_REAC, true, false));
            } else if (inputs.down) {
                backends[i]->SetGameMode(new FgcMode(socd::SOCD_NEUTRAL));
            }
        } else if (inputs.mod_y && !inputs.mod_x && inputs.start) {
            if (inputs.l) {
                current_kb_mode = new DefaultKeyboardMode(socd::SOCD_2IP);
                backends[i]->SetGameMode(nullptr);
            }
        }
    }
}

#endif
