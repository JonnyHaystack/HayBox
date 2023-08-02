#ifndef _CORE_SOCD_HPP
#define _CORE_SOCD_HPP

#include "state.hpp"
#include "stdlib.hpp"

namespace socd {
    typedef enum {
        SOCD_NEUTRAL,
        SOCD_2IP,
        SOCD_2IP_NO_REAC,
        SOCD_DIR1_PRIORITY,
        SOCD_DIR2_PRIORITY,
        SOCD_NONE,
    } SocdType;

    typedef struct {
        bool InputState::*input_dir1;
        bool InputState::*input_dir2;
        SocdType socd_type = SOCD_NEUTRAL;
    } SocdPair;

    typedef struct {
        bool was_dir1 = false;
        bool was_dir2 = false;
        bool lock_dir1 = false;
        bool lock_dir2 = false;
    } SocdState;

    void second_input_priority_no_reactivation(
        bool &input_dir1,
        bool &input_dir2,
        SocdState &socd_state
    );

    void second_input_priority(bool &input_dir1, bool &input_dir2, SocdState &socd_state);

    void neutral(bool &input_dir1, bool &input_dir2);

    void dir1_priority(bool &input_dir1, bool &input_dir2);
}

#endif
