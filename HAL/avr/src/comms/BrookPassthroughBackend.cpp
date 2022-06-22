#include "comms/BrookPassthroughBackend.hpp"

BrookPassthroughBackend::BrookPassthroughBackend(
    InputSource **input_sources,
    size_t input_source_count,
    int dpadUp_pin,
    int l3_pin
)
    : CommunicationBackend(input_sources, input_source_count) {
    _dpadUp_pin = dpadUp_pin;
    _l3_pin = l3_pin;
}

void BrookPassthroughBackend::UpdateOutputs() {
    ResetOutputs();

    // Put all of the thumb cluster buttons as up by default.
    _outputs.dpadUp = _inputs.mod_x || _inputs.mod_y || _inputs.dpad_toggle || _inputs.c_down ||
                      _inputs.a || _inputs.c_left || _inputs.c_right || _inputs.c_up;

    _outputs.leftStickClick = _inputs.l;
}

void BrookPassthroughBackend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();
    UpdateOutputs();

    if (_dpadUp_pin > -1) {
        digitalWrite(_dpadUp_pin, (_outputs.dpadUp) ? LOW : HIGH);
    }
    if (_l3_pin > -1) {
        digitalWrite(_l3_pin, (_outputs.leftStickClick) ? LOW : HIGH);
    }
}
