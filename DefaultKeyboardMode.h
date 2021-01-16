#ifndef DEFAULTKEYBOARDMODE_H_MNDOUIGV
#define DEFAULTKEYBOARDMODE_H_MNDOUIGV

#include "KeyboardMode.h"
#include "socd.h"
#include "state.h"

class DefaultKeyboardMode : public KeyboardMode {
public:
  DefaultKeyboardMode(socd::SocdType, state::InputState &rInputState);
  void SendKeys();
};

#endif /* end of include guard: DEFAULTKEYBOARDMODE_H_MNDOUIGV */
