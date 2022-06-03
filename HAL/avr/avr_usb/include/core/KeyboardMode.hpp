#ifndef _CORE_KEYBOARDMODE_HPP
#define _CORE_KEYBOARDMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

#include <keycodes.h>

class KeyboardMode : public InputMode {
  public:
    KeyboardMode(socd::SocdType socd_type);
    ~KeyboardMode();
    void SendReport(InputState &inputs);
    virtual void SendKeys(InputState &inputs) = 0;

  protected:
    void Press(uint8_t key, bool press);
};

#endif
