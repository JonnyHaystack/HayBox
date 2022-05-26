#ifndef _CORE_KEYBOARDMODE_HPP
#define _CORE_KEYBOARDMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class KeyboardMode : public InputMode {
  public:
    KeyboardMode(socd::SocdType socd_type);
    ~KeyboardMode();
    void SendReport();
    virtual void SendKeys() = 0;

  protected:
    void Press(uint8_t keycode, bool press);
};

#endif
