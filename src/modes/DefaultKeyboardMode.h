#ifndef DEFAULTKEYBOARDMODE_H_MNDOUIGV
#define DEFAULTKEYBOARDMODE_H_MNDOUIGV

#include "core/KeyboardMode.h"
#include "core/socd.h"
#include "core/state.h"

class DefaultKeyboardMode : public KeyboardMode {
public:
  DefaultKeyboardMode(socd::SocdType, state::InputState &rInputState);
  void SendKeys();
};

#endif /* end of include guard: DEFAULTKEYBOARDMODE_H_MNDOUIGV */
