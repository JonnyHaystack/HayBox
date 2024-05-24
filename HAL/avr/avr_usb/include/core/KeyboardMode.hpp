#ifndef _CORE_KEYBOARDMODE_HPP
#define _CORE_KEYBOARDMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

#include <keycodes.h>

class KeyboardMode : public InputMode {
  public:
    KeyboardMode(GameModeConfig &config);
    ~KeyboardMode();
    void SendReport(const InputState &inputs);

    void UpdateOutputs(const InputState &inputs, OutputState &outputs) {}

  protected:
    void Press(uint8_t keycode, bool press);

  private:
    virtual void UpdateKeys(const InputState &inputs) = 0;
};

#endif
