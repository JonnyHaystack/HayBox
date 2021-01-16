#ifndef KEYBOARDMODE_H_MMPWZ9DS
#define KEYBOARDMODE_H_MMPWZ9DS

#include "InputMode.h"
#include "socd.h"
#include "state.h"

class KeyboardMode : public InputMode {
public:
  KeyboardMode(socd::SocdType, state::InputState &rInputState);
  ~KeyboardMode();
  void UpdateOutputs();
  virtual void SendKeys() = 0;
protected:
  void Press(char key, bool press);
};

#endif /* end of include guard: KEYBOARDMODE_H_MMPWZ9DS */
