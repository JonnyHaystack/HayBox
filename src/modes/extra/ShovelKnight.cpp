#include "modes/extra/ShovelKnight.hpp"

ShovelKnight::ShovelKnight(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void ShovelKnight::UpdateDigitalOutputs(InputState &inputs) {
    _outputs->dpadLeft = inputs.left;
    _outputs->dpadRight = inputs.right;
    _outputs->dpadDown = inputs.down;
    _outputs->dpadUp = inputs.mod_x;

    _outputs->b = inputs.x; // Jump
    _outputs->a = inputs.a; // Attack
    _outputs->y = inputs.b; // Attack
    _outputs->x = inputs.z; // Subweapon
    _outputs->buttonL = inputs.r; // Subweapon prev
    _outputs->buttonR = inputs.y; // Subweapon next

    _outputs->select = inputs.lightshield; // Inventory
    _outputs->start = inputs.start; // Pause
}

void ShovelKnight::UpdateAnalogOutputs(InputState &inputs) {
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.mod_x,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up
    );
}
