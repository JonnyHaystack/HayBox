#ifndef _CORE_SOCD_HPP
#define _CORE_SOCD_HPP

#include "state.hpp"
#include "stdlib.hpp"

#include <config.pb.h>

namespace socd {
    typedef struct {
        bool was_dir1 = false;
        bool was_dir2 = false;
        bool lock_dir1 = false;
        bool lock_dir2 = false;
    } SocdState;

    void second_input_priority_no_reactivation(
        InputState &inputs,
        Button button_dir1,
        Button button_dir2,
        SocdState &socd_state
    );

    void second_input_priority(
        InputState &inputs,
        Button button_dir1,
        Button button_dir2,
        SocdState &socd_state
    );

    void neutral(InputState &inputs, Button button_dir1, Button button_dir2);

    void dir1_priority(InputState &inputs, Button button_dir1, Button button_dir2);
}

#endif
