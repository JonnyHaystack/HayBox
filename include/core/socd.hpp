#ifndef _CORE_SOCD_HPP
#define _CORE_SOCD_HPP

#include "stdlib.hpp"

namespace socd {

    typedef enum {
        SOCD_NEUTRAL,
        SOCD_2IP,
        SOCD_2IP_NO_REAC,
        SOCD_KEYBOARD,
    } SocdType;

    typedef struct {
        bool *button_low;
        bool *button_high;
    } SocdPair;

    typedef struct {
        bool was_low;
        bool was_high;
        bool lock_low;
        bool lock_high;
    } SocdState;

    SocdState twoIPNoReactivate(bool &isLow, bool &isHigh, SocdState socdState);

    SocdState twoIP(bool &isLow, bool &isHigh, SocdState socdState);

    void neutral(bool &isLow, bool &isHigh);

}

#endif
