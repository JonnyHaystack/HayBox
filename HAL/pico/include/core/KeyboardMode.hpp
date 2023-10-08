#ifndef _CORE_KEYBOARDMODE_HPP
#define _CORE_KEYBOARDMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

#include <TUKeyboard.hpp>
#include <config.pb.h>

class KeyboardMode : public InputMode {
  public:
    KeyboardMode(const GameModeConfig &config);
    ~KeyboardMode();
    void SendReport(InputState &inputs);

  protected:
    void Press(uint8_t keycode, bool press);

  private:
    TUKeyboard *_keyboard;

    virtual void UpdateKeys(InputState &inputs) = 0;
};

#endif
