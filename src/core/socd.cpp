#include "core/socd.hpp"

void socd::second_input_priority_no_reactivation(
    InputState &inputs,
    Button button_dir1,
    Button button_dir2,
    SocdState &socd_state
) {
    bool result_dir1 = false;
    bool result_dir2 = false;
    bool dir1_pressed = get_button(inputs.buttons, button_dir1);
    bool dir2_pressed = get_button(inputs.buttons, button_dir2);
    if (dir1_pressed && dir2_pressed) {
        if (socd_state.was_dir2) {
            result_dir1 = true;
            result_dir2 = false;
            socd_state.lock_dir2 = true;
        }
        if (socd_state.was_dir1) {
            result_dir1 = false;
            result_dir2 = true;
            socd_state.lock_dir1 = true;
        }
    }
    if (!dir1_pressed && dir2_pressed && !socd_state.lock_dir2) {
        result_dir1 = false;
        result_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
    }
    if (dir1_pressed && !dir2_pressed && !socd_state.lock_dir1) {
        result_dir1 = true;
        result_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
        socd_state.lock_dir2 = false;
    }
    if (!dir1_pressed && !dir2_pressed) {
        socd_state.was_dir2 = false;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
        socd_state.lock_dir2 = false;
    }
    set_button(inputs.buttons, button_dir1, result_dir1);
    set_button(inputs.buttons, button_dir2, result_dir2);
}

void socd::second_input_priority(
    InputState &inputs,
    Button button_dir1,
    Button button_dir2,
    SocdState &socd_state
) {
    bool result_dir1 = false;
    bool result_dir2 = false;
    bool dir1_pressed = get_button(inputs.buttons, button_dir1);
    bool dir2_pressed = get_button(inputs.buttons, button_dir2);
    if (dir1_pressed && socd_state.was_dir2) {
        result_dir1 = true;
        result_dir2 = false;
    }
    if (dir2_pressed && socd_state.was_dir1) {
        result_dir1 = false;
        result_dir2 = true;
    }
    if (!dir1_pressed && dir2_pressed) {
        result_dir1 = false;
        result_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
    }
    if (dir1_pressed && !dir2_pressed) {
        result_dir1 = true;
        result_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
    }
    set_button(inputs.buttons, button_dir1, result_dir1);
    set_button(inputs.buttons, button_dir2, result_dir2);
}

void socd::neutral(InputState &inputs, Button button_dir1, Button button_dir2) {
    if (get_button(inputs.buttons, button_dir1) && get_button(inputs.buttons, button_dir2)) {
        set_button(inputs.buttons, button_dir1, false);
        set_button(inputs.buttons, button_dir2, false);
    }
}

void socd::dir1_priority(InputState &inputs, Button button_dir1, Button button_dir2) {
    if (get_button(inputs.buttons, button_dir1) && get_button(inputs.buttons, button_dir2)) {
        set_button(inputs.buttons, button_dir2, false);
    }
}
