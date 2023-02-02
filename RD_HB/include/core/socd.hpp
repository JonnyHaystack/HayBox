#ifndef _CORE_SOCD_HPP
#define _CORE_SOCD_HPP

#include "state.hpp"
#include "stdlib.hpp"

namespace socd {

    typedef enum {
        SOCD_NEUTRAL,
        SOCD_2IP,
        SOCD_2IP_NO_REAC,
        SOCD_KEYBOARD,
    } SocdType;

    typedef struct {
        bool InputState::*input_dir1;
        bool InputState::*input_dir2;
    } SocdPair;

    typedef struct {
        bool was_dir1 = false;
        bool was_dir2 = false;
        bool lock_dir1 = false;
        bool lock_dir2 = false;
    } SocdState;

    void twoIPNoReactivate(bool &input_dir1, bool &input_dir2, SocdState &socd_state);

    void twoIP(bool &input_dir1, bool &input_dir2, SocdState &socd_state);

    void neutral(bool &input_dir1, bool &input_dir2);

}

#endif
