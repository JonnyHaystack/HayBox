#ifndef _CORE_INPUTMODE_HPP
#define _CORE_INPUTMODE_HPP

#include "socd.hpp"
#include "state.hpp"

class InputMode {
  public:
    InputMode();
    virtual ~InputMode();

  protected:
    socd::SocdPair *_socd_pairs = nullptr;
    size_t _socd_pair_count = 0;

    virtual void HandleSocd(InputState &inputs);

  private:
    socd::SocdState *_socd_states = nullptr;
};

#endif
