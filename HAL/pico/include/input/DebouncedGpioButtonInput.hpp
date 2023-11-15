#ifndef _INPUT_DEBOUNCEDGPIOBUTTONINPUT_HPP
#define _INPUT_DEBOUNCEDGPIOBUTTONINPUT_HPP

#include "core/state.hpp"
#include "input/GpioButtonInput.hpp"
#include "input/debounce.hpp"
#include "util/state_util.hpp"

template <size_t button_count> class DebouncedGpioButtonInput : public GpioButtonInput {
  public:
    DebouncedGpioButtonInput(
        const GpioButtonMapping button_mappings[button_count],
        uint32_t debounce_period_ms = 5
    )
        : GpioButtonInput(button_mappings, button_count) {
        _debounce_period_ms = debounce_period_ms;
    }

  private:
    DebounceState _debounce_state[button_count];
    uint32_t _debounce_period_ms;

    void UpdateButtonState(InputState &inputs, size_t button_mapping_index, bool pressed) {
        bool state_changed = update_debounce_state(
            _debounce_state[button_mapping_index],
            pressed,
            _debounce_period_ms
        );
        if (state_changed) {
            set_button(inputs.buttons, _button_mappings[button_mapping_index].button, pressed);
        }
    }
};

#endif