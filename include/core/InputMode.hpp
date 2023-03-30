#ifndef _CORE_INPUTMODE_HPP
#define _CORE_INPUTMODE_HPP

#include "socd.hpp"
#include "state.hpp"

class InputMode {
  public:
    InputMode(socd::SocdType socd_type);
    virtual ~InputMode();

  protected:
    socd::SocdPair *_socd_pairs = nullptr;
    size_t _socd_pair_count = 0;
    /* Exposed to child classes so that game modes are able to have different behaviour depending on
     * SOCD cleaning mode. */
    socd::SocdType _socd_type;

    virtual void HandleSocd(InputState &inputs);

  private:
    socd::SocdState *_socd_states = nullptr;
};

#endif
