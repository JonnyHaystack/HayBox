#include "modes/extra/SaltAndSanctuary.hpp"

SaltAndSanctuary::SaltAndSanctuary(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void SaltAndSanctuary::UpdateDigitalOutputs() {
    _outputs->dpadRight = _inputs->l; // Block
    _outputs->b = _inputs->b; // Roll
    _outputs->a = _inputs->a; // Attack
    _outputs->y = _inputs->z; // Strong
    _outputs->dpadDown = _inputs->mod_y; // Use
    _outputs->x = _inputs->x; // Jump

    _outputs->buttonL = _inputs->r; // Previous item
    _outputs->buttonR = _inputs->y; // Next item
    _outputs->triggerLDigital = _inputs->lightshield; // Use item

    _outputs->triggerRDigital = _inputs->midshield; // Use torch

    _outputs->dpadLeft = _inputs->up; // Switch loadout

    _outputs->start = _inputs->start; // Inventory
}

void SaltAndSanctuary::UpdateAnalogOutputs() {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        _inputs->left,
        _inputs->right,
        _inputs->down,
        _inputs->mod_x,
        _inputs->c_left,
        _inputs->c_right,
        _inputs->c_down,
        _inputs->c_up
    );
}
