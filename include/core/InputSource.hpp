#ifndef _CORE_INPUTSOURCE_HPP
#define _CORE_INPUTSOURCE_HPP

#include "core/state.hpp"

enum class InputScanSpeed {
    SLOW,
    MEDIUM,
    FAST,
};

class InputSource {
  public:
    InputSource();
    virtual ~InputSource(){};
    virtual void UpdateInputs(InputState &inputs) = 0;
    virtual InputScanSpeed ScanSpeed() = 0;
};

#endif