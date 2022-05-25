#include "core/socd.hpp"

socd::SocdState socd::twoIPNoReactivate(bool &dir1, bool &dir2, SocdState socd_state) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (dir1 && dir2) {
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
    if (!dir1 && dir2 && (socd_state.lock_dir2 == false)) {
        is_dir1 = false;
        is_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
    }
    if (dir1 && !dir2 && (socd_state.lock_dir1 == false)) {
        is_dir1 = true;
        is_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
        socd_state.lock_dir2 = false;
    }
    if (!dir1 && !dir2) {
        socd_state.was_dir2 = false;
        socd_state.was_dir1 = false;
        socd_state.lock_dir1 = false;
        socd_state.lock_dir2 = false;
    }
    dir1 = is_dir1;
    dir2 = is_dir2;
    return socd_state;
}

socd::SocdState socd::twoIP(bool &dir1, bool &dir2, SocdState socd_state) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (dir1 && socd_state.was_dir2) {
        is_dir1 = true;
        is_dir2 = false;
    }
    if (dir2 && socd_state.was_dir1) {
        is_dir1 = false;
        is_dir2 = true;
    }
    if (!dir1 && dir2) {
        is_dir1 = false;
        is_dir2 = true;
        socd_state.was_dir2 = true;
        socd_state.was_dir1 = false;
    }
    if (dir1 && !dir2) {
        is_dir1 = true;
        is_dir2 = false;
        socd_state.was_dir1 = true;
        socd_state.was_dir2 = false;
    }
    dir1 = is_dir1;
    dir2 = is_dir2;
    return socd_state;
}

void socd::neutral(bool &dir1, bool &dir2) {
    bool is_dir1 = false;
    bool is_dir2 = false;
    if (!dir1 && dir2) {
        is_dir1 = false;
        is_dir2 = true;
    }
    if (dir1 && !dir2) {
        is_dir1 = true;
        is_dir2 = false;
    }
    dir1 = is_dir1;
    dir2 = is_dir2;
}
