#ifndef _MODES_DEFAULTKEYBOARDMODE_HPP
#define _MODES_DEFAULTKEYBOARDMODE_HPP

#include "core/KeyboardMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class DefaultKeyboardMode : public KeyboardMode {
  public:
    DefaultKeyboardMode(socd::SocdType socd_type);

  private:
    void UpdateKeys(InputState &inputs);
};

#endif
