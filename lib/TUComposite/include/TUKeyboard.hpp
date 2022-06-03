#ifndef _TUCOMPOSITE_TUKEYBOARD_HPP
#define _TUCOMPOSITE_TUKEYBOARD_HPP

#include "TUComposite.hpp"

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

class TUKeyboard {
  public:
    TUKeyboard();

    void begin();
    void setPressed(uint8_t keycode, bool pressed);
    void press(uint8_t keycode);
    void release(uint8_t keycode);
    void releaseAll();
    void sendState();

  private:
    hid_keyboard_report_t _report;
};

#endif