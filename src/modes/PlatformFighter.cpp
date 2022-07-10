#include "modes/PlatformFighter.hpp"

PlatformFighter::PlatformFighter(socd::SocdType socd_type, uint8_t analog_stick_length)
    : ControllerMode(socd_type, analog_stick_length) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,    &InputState::right },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    _horizontal_socd = false;
}

void PlatformFighter::HandleSocd() {
    _horizontal_socd = _inputs->left && _inputs->right;
    InputMode::HandleSocd();
}
