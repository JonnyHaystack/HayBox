#include "modes/CustomKeyboardMode.hpp"

#include "core/state.hpp"
#include "util/state_util.hpp"

#include <config.pb.h>

CustomKeyboardMode::CustomKeyboardMode(
    const GameModeConfig &config,
    const KeyboardModeConfig &keyboard_config
)
    : KeyboardMode(config),
      _keyboard_config(keyboard_config) {}

void CustomKeyboardMode::UpdateKeys(const InputState &inputs) {
    const ButtonToKeycodeMapping *keymap = _keyboard_config.buttons_to_keycodes;
    size_t key_count = _keyboard_config.buttons_to_keycodes_count;
    for (size_t i = 0; i < key_count; i++) {
        if (keymap[i].button == BTN_UNSPECIFIED) {
            continue;
        }
        Press(keymap[i].keycode, get_button(inputs.buttons, keymap[i].button));
    }
}