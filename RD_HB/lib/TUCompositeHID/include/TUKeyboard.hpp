#ifndef _TUCOMPOSITE_TUKEYBOARD_HPP
#define _TUCOMPOSITE_TUKEYBOARD_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <TUCompositeHID.hpp>

class TUKeyboard {
  public:
    TUKeyboard();

    static void registerDescriptor();

    void begin();
    void setPressed(uint8_t keycode, bool pressed);
    void press(uint8_t keycode);
    void release(uint8_t keycode);
    void releaseAll();
    void sendState();

  private:
    static const uint8_t _report_id = 2;
    static uint8_t _descriptor[];

    hid_keyboard_report_t _report;
};

#endif