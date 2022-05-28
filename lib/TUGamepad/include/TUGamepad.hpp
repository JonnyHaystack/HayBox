/************************************************************************
MIT License

Copyright (c) 2021 gdsports625@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*************************************************************************/
#ifndef _TUGAMEPAD_GAMEPAD_HPP
#define _TUGAMEPAD_GAMEPAD_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

const uint8_t _gamepad_hid_report_desc[] = { TUD_HID_REPORT_DESC_GAMEPAD() };

class TUGamepad {
  public:
    TUGamepad();

    void begin();
    bool ready();
    void resetInputs();
    bool sendState();

    void press(uint8_t button);
    void release(uint8_t button);
    void setButton(uint8_t button, bool pressed);
    void releaseAll();

    void buttons(uint32_t buttons);
    void leftXAxis(int8_t value);
    void leftYAxis(int8_t value);
    void rightXAxis(int8_t value);
    void rightYAxis(int8_t value);
    void triggerLAnalog(int8_t value);
    void triggerRAnalog(int8_t value);
    void hatSwitch(hid_gamepad_hat_t direction);

  protected:
    hid_gamepad_report_t _report;
    Adafruit_USBD_HID _usb_hid;
};

#endif