#include "core/InputMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

InputMode::InputMode(socd::SocdType socd_type) {
    this->socd_type = socd_type;
}

void InputMode::HandleSocd(InputState &inputs) {
    // Initialize SOCD states if they aren't initialized.
    if (socd_states.size() != socd_pairs.size()) {
        for (int i = 0; i < socd_pairs.size(); i++) {
            socd_states.push_back({
                .was_dir1 = false,
                .was_dir2 = false,
                .lock_dir1 = false,
                .lock_dir2 = false,
            });
        }
    }

    // Handle SOCD resolution for each SOCD button pair.
    for (int i = 0; i < socd_pairs.size(); i++) {
        socd::SocdPair pair = socd_pairs[i];
        socd::SocdState socd_state = socd_states[i];
        switch (socd_type) {
            case socd::SOCD_NEUTRAL:
                socd::neutral(inputs.*(pair.input_dir1), inputs.*(pair.input_dir2));
                break;
            case socd::SOCD_2IP:
                socd_states[i] =
                    socd::twoIP(inputs.*(pair.input_dir1), inputs.*(pair.input_dir2), socd_state);
                break;
            case socd::SOCD_2IP_NO_REAC:
                socd_states[i] = socd::twoIPNoReactivate(
                    inputs.*(pair.input_dir1),
                    inputs.*(pair.input_dir2),
                    socd_state
                );
                break;
            case socd::SOCD_KEYBOARD:
                break;
        }
    }
}
