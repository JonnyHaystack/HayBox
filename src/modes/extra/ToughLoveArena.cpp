#include "modes/extra/ToughLoveArena.hpp"

ToughLoveArena::ToughLoveArena(socd::SocdType socd_type) {
    _socd_pair_count = 1;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right, socd_type},
    };
}

void ToughLoveArena::UpdateKeys(InputState &inputs) {
    Press('s', inputs.left);
    Press('d', inputs.right);
    Press('j', inputs.b);
    Press('k', inputs.x);
    Press('l', inputs.z);
}
