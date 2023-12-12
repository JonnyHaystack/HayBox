#include "core/InputMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"
#include "util/state_util.hpp"

InputMode::InputMode(const GameModeConfig &config) : _config(config) {
    _socd_states = new socd::SocdState[_config.socd_pairs_count];
}

InputMode::~InputMode() {
    delete[] _socd_states;
}

const GameModeConfig &InputMode::GetConfig() {
    return _config;
}

void InputMode::HandleSocd(InputState &inputs) {
    // Handle SOCD resolution for each SOCD button pair.
    for (size_t i = 0; i < _config.socd_pairs_count; i++) {
        const SocdPair &pair = _config.socd_pairs[i];
        switch (pair.socd_type) {
            case SOCD_NEUTRAL:
                socd::neutral(inputs, pair.button_dir1, pair.button_dir2);
                break;
            case SOCD_2IP:
                socd::second_input_priority(
                    inputs,
                    pair.button_dir1,
                    pair.button_dir2,
                    _socd_states[i]
                );
                break;
            case SOCD_2IP_NO_REAC:
                socd::second_input_priority_no_reactivation(
                    inputs,
                    pair.button_dir1,
                    pair.button_dir2,
                    _socd_states[i]
                );
                break;
            case SOCD_DIR1_PRIORITY:
                socd::dir1_priority(inputs, pair.button_dir1, pair.button_dir2);
                break;
            case SOCD_DIR2_PRIORITY:
                socd::dir1_priority(inputs, pair.button_dir2, pair.button_dir1);
                break;
            case SOCD_UNSPECIFIED:
            default:
                break;
        }
    }
}

void InputMode::HandleRemap(const InputState &original_inputs, InputState &remapped_inputs) {
    // Keep track of which buttons have been remapped so that we can avoid conflicts for buttons
    // that are remapped to multiple buttons and prevent macro remapping.
    uint64_t physical_buttons_already_remapped = 0;
    uint64_t buttons_already_mapped_to = 0;
    for (size_t i = 0; i < _config.button_remapping_count; i++) {
        const ButtonRemap &remapping = _config.button_remapping[i];
        // If this physical button was already mapped to something else, ignore this remapping. This
        // is to prevent creating macro behaviour through remapping.
        if (get_button(physical_buttons_already_remapped, remapping.physical_button)) {
            continue;
        }
        // If physical button has not been mapped to yet, set it to false. This too is intended to
        // prevent creating macro behaviour through remapping.
        if (!get_button(buttons_already_mapped_to, remapping.physical_button)) {
            set_button(remapped_inputs.buttons, remapping.physical_button, false);
        }

        // Either use the value of the physical button, or if the physical button is not pressed,
        // but the target button has another physical button remapped to it, and is considered to be
        // pressed, leave it as pressed.
        bool should_be_pressed = get_button(original_inputs.buttons, remapping.physical_button) ||
                                 get_button(remapped_inputs.buttons, remapping.activates);
        set_button(remapped_inputs.buttons, remapping.activates, should_be_pressed);

        // Track which buttons have been mapped from/to.
        set_button(physical_buttons_already_remapped, remapping.physical_button, true);
        set_button(buttons_already_mapped_to, remapping.activates, true);
    }
}
