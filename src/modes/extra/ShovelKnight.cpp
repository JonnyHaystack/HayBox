#include "modes/extra/ShovelKnight.hpp"

ShovelKnight::ShovelKnight(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void ShovelKnight::UpdateDigitalOutputs() {
    _outputs->dpadLeft = _inputs->left;
    _outputs->dpadRight = _inputs->right;
    _outputs->dpadDown = _inputs->down;
    _outputs->dpadUp = _inputs->mod_x;

    _outputs->b = _inputs->x; // Jump
    _outputs->a = _inputs->a; // Attack
    _outputs->y = _inputs->b; // Attack
    _outputs->x = _inputs->z; // Subweapon
    _outputs->buttonL = _inputs->r; // Subweapon prev
    _outputs->buttonR = _inputs->y; // Subweapon next

    _outputs->select = _inputs->lightshield; // Inventory
    _outputs->start = _inputs->start; // Pause
}

void ShovelKnight::UpdateAnalogOutputs() {
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
