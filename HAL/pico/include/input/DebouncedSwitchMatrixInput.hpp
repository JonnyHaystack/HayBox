#ifndef _INPUT_DEBOUNCEDSWITCHMATRIXINPUT_HPP
#define _INPUT_DEBOUNCEDSWITCHMATRIXINPUT_HPP

#include "input/SwitchMatrixInput.hpp"
#include "input/debounce.hpp"

template <size_t num_rows, size_t num_cols>
class DebouncedSwitchMatrixInput : public SwitchMatrixInput<num_rows, num_cols> {
  public:
    DebouncedSwitchMatrixInput(
        const uint row_pins[num_rows],
        const uint col_pins[num_cols],
        const Button (&matrix)[num_rows][num_cols],
        DiodeDirection direction,
        uint32_t debounce_period_ms = 5
    )
        : SwitchMatrixInput<num_rows, num_cols>(row_pins, col_pins, matrix, direction) {
        _debounce_period_ms = debounce_period_ms;
    }

  private:
    DebounceState _debounce_state[num_rows][num_cols];
    uint32_t _debounce_period_ms;

    void UpdateButtonState(InputState &inputs, size_t col_index, size_t row_index, bool pressed) {
        bool state_changed = update_debounce_state(
            _debounce_state[col_index][row_index],
            pressed,
            _debounce_period_ms
        );
        if (state_changed) {
            Button button = _matrix[col_index][row_index];
            set_button(inputs.buttons, button, pressed);
        }
    };
};

#endif
