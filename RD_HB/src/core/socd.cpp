#include "core/socd.hpp"

void socd::twoIPNoReactivate(bool &input_dir1, bool &input_dir2, SocdState &socd_state) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (input_dir1 && input_dir2) {
        if (socd_state.was_dir2) {
            is_dir1 = true;
            is_dir2 = false;
            socd_state.lock_dir2 = true;
        }
        if (socd_state.was_dir1) {
            is_dir1 = false;
            is_dir2 = true;
            socd_state.lock_dir1 = true;
        }
    }
    if (!input_dir1 && input_dir2 && !socd_state.lock_dir2) {
        is_dir1 = false;
        is_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
    }
    if (input_dir1 && !input_dir2 && !socd_state.lock_dir1) {
        is_dir1 = true;
        is_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
        socd_state.lock_dir2 = false;
    }
    if (!input_dir1 && !input_dir2) {
        socd_state.was_dir2 = false;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
        socd_state.lock_dir2 = false;
    }
    input_dir1 = is_dir1;
    input_dir2 = is_dir2;
}

void socd::twoIP(bool &input_dir1, bool &input_dir2, SocdState &socd_state) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (input_dir1 && socd_state.was_dir2) {
        is_dir1 = true;
        is_dir2 = false;
    }
    if (input_dir2 && socd_state.was_dir1) {
        is_dir1 = false;
        is_dir2 = true;
    }
    if (!input_dir1 && input_dir2) {
        is_dir1 = false;
        is_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
    }
    if (input_dir1 && !input_dir2) {
        is_dir1 = true;
        is_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
    }
    input_dir1 = is_dir1;
    input_dir2 = is_dir2;
}

void socd::neutral(bool &input_dir1, bool &input_dir2) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (!input_dir1 && input_dir2) {
        is_dir1 = false;
        is_dir2 = true;
    }
    if (input_dir1 && !input_dir2) {
        is_dir1 = true;
        is_dir2 = false;
    }
    input_dir1 = is_dir1;
    input_dir2 = is_dir2;
}
