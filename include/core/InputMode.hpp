#ifndef _CORE_INPUTMODE_HPP
#define _CORE_INPUTMODE_HPP

#include "socd.hpp"
#include "state.hpp"

#include <Vector.h>

class InputMode {
  public:
    InputMode(socd::SocdType socd_type);
    virtual ~InputMode(){};

  protected:
    Vector<socd::SocdPair> socd_pairs;
    virtual void HandleSocd(InputState &inputs);
    /* Exposed to child classes so that game modes are able to have different behaviour depending on
     * SOCD cleaning mode. */
    socd::SocdType socd_type;

  private:
    Vector<socd::SocdState> socd_states;
};

#endif
