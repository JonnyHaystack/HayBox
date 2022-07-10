#include "modes/extra/MKWii.hpp"

MKWii::MKWii(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right},
        socd::SocdPair{ &InputState::l,   &InputState::down },
        socd::SocdPair{ &InputState::l,   &InputState::mod_x},
        socd::SocdPair{ &InputState::l,   &InputState::mod_y},
    };
}

void MKWii::UpdateDigitalOutputs(InputState &inputs) {
    _outputs->a = inputs.b;
    _outputs->b = inputs.x;
    _outputs->triggerLDigital = inputs.z;
    _outputs->buttonR = inputs.up;
    _outputs->dpadUp = inputs.a;
    _outputs->start = inputs.start;
}

void MKWii::UpdateAnalogOutputs(InputState &inputs) {
    bool up = inputs.down || inputs.mod_x || inputs.mod_y;

    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.l,
        up,
        false,
        false,
        false,
        false
    );

    if (inputs.z) {
        _outputs->triggerLAnalog = 140;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        _outputs->leftStickX = inputs.nunchuk_x;
        _outputs->leftStickY = inputs.nunchuk_y;
    }
}
