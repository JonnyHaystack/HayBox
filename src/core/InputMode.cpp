#include "core/InputMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

InputMode::InputMode() {}

InputMode::~InputMode() {
    delete[] _socd_pairs;
    delete[] _socd_states;
}

void InputMode::HandleSocd(InputState &inputs) {
    if (_socd_pairs == nullptr) {
        return;
    }

    // Initialize SOCD states if they aren't initialized.
    if (_socd_states == nullptr) {
        _socd_states = new socd::SocdState[_socd_pair_count];
    }

    // Handle SOCD resolution for each SOCD button pair.
    for (size_t i = 0; i < _socd_pair_count; i++) {
        socd::SocdPair pair = _socd_pairs[i];
        switch (pair.socd_type) {
            case socd::SOCD_NEUTRAL:
                socd::neutral(inputs.*(pair.input_dir1), inputs.*(pair.input_dir2));
                break;
            case socd::SOCD_2IP:
                socd::second_input_priority(
                    inputs.*(pair.input_dir1),
                    inputs.*(pair.input_dir2),
                    _socd_states[i]
                );
                break;
            case socd::SOCD_2IP_NO_REAC:
                socd::second_input_priority_no_reactivation(
                    inputs.*(pair.input_dir1),
                    inputs.*(pair.input_dir2),
                    _socd_states[i]
                );
                break;
            case socd::SOCD_DIR1_PRIORITY:
                socd::dir1_priority(inputs.*(pair.input_dir1), inputs.*(pair.input_dir2));
                break;
            case socd::SOCD_DIR2_PRIORITY:
                socd::dir1_priority(inputs.*(pair.input_dir2), inputs.*(pair.input_dir1));
                break;
            case socd::SOCD_NONE:
                break;
        }
    }
}
