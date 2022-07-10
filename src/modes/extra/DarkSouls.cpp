#include "modes/extra/DarkSouls.hpp"

DarkSouls::DarkSouls(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

void DarkSouls::UpdateDigitalOutputs(InputState &inputs) {
    _outputs->y = inputs.y;
    _outputs->x = inputs.r;

    // Base layer.
    bool layer0 = !inputs.x && !inputs.nunchuk_c;
    // Secondary layer when X is held.
    bool layerX = inputs.x && !inputs.nunchuk_c;
    // DPad layer when Nunchuk C is held.
    bool layerC = inputs.nunchuk_c;

    if (layer0) {
        _outputs->a = inputs.a;
        _outputs->b = inputs.b;
        _outputs->buttonR = inputs.z;
        _outputs->buttonL = inputs.up;
        _outputs->start = inputs.start | inputs.nunchuk_z;
    } else if (layerX) {
        _outputs->rightStickClick = inputs.a;
        _outputs->triggerRDigital = inputs.z;
        _outputs->triggerLDigital = inputs.up;
        _outputs->select = inputs.start;
    } else if (layerC) {
        _outputs->a = inputs.a;
        _outputs->dpadLeft = inputs.b;
        _outputs->dpadDown = inputs.x;
        _outputs->dpadUp = inputs.z;
        _outputs->dpadRight = inputs.up;
        _outputs->select = inputs.nunchuk_z;
    }
}

void DarkSouls::UpdateAnalogOutputs(InputState &inputs) {
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

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        _outputs->leftStickX = inputs.nunchuk_x;
        _outputs->leftStickY = inputs.nunchuk_y;
    }
}
