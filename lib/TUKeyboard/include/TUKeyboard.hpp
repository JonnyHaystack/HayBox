#ifndef _TUKEYBOARD_TUKEYBOARD_HPP
#define _TUKEYBOARD_TUKEYBOARD_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

const uint8_t _keyboard_hid_report_desc[] = { TUD_HID_REPORT_DESC_KEYBOARD() };

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
    Adafruit_USBD_HID _usb_hid;
};

#endif