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
        bool was_dir1;
        bool was_dir2;
        bool lock_dir1;
        bool lock_dir2;
    } SocdState;

    SocdState twoIPNoReactivate(bool &dir1, bool &dir2, SocdState socd_state);

    SocdState twoIP(bool &dir1, bool &dir2, SocdState socd_state);

    void neutral(bool &dir1, bool &dir2);

}

#endif
