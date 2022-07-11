#include "modes/extra/HollowKnight.hpp"

HollowKnight::HollowKnight(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void HollowKnight::UpdateDigitalOutputs() {
    _outputs->a = _inputs->a; // Attack
    _outputs->b = _inputs->b; // Dash
    _outputs->x = _inputs->x; // Jump
    _outputs->y = _inputs->mod_y; // Spell
    _outputs->triggerLDigital = _inputs->r; // Focus/cast
    _outputs->triggerRDigital = _inputs->z;
    _outputs->buttonR = _inputs->up; // Dream nail

    _outputs->buttonL = _inputs->lightshield; // Map
    _outputs->select = _inputs->midshield; // Inventory
    _outputs->start = _inputs->start; // Pause
}

void HollowKnight::UpdateAnalogOutputs() {
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
