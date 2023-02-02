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
#ifndef _TUCOMPOSITE_TUGAMEPAD_HPP
#define _TUCOMPOSITE_TUGAMEPAD_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <TUCompositeHID.hpp>

typedef struct TU_ATTR_PACKED {
    uint16_t x; // X value of left analog stick
    uint16_t y; // Y value of left analog stick
    uint16_t z; // Value of analog left trigger
    uint16_t rz; // Value of analog right trigger
    uint16_t rx; // X value of right analog stick
    uint16_t ry; // Y value of right analog stick
    uint8_t hat; // Position value of the DPad/hat switch
    uint16_t buttons; // Buttons mask for currently pressed buttons
} gamepad_report_t;

class TUGamepad {
  public:
    TUGamepad();

    static void registerDescriptor();

    void begin();
    bool ready();
    void resetInputs();
    bool sendState();

    void press(uint8_t button);
    void release(uint8_t button);
    void setButton(uint8_t button, bool pressed);
    void releaseAll();

    void buttons(uint16_t buttons);
    void leftXAxis(uint8_t value);
    void leftYAxis(uint8_t value);
    void rightXAxis(uint8_t value);
    void rightYAxis(uint8_t value);
    void triggerLAnalog(uint8_t value);
    void triggerRAnalog(uint8_t value);
    void hatSwitch(hid_gamepad_hat_t direction);
    void hatSwitch(bool left, bool right, bool down, bool up);

  protected:
    static const uint8_t _report_id = 1;
    static uint8_t _descriptor[];

    gamepad_report_t _report;

  private:
    static hid_gamepad_hat_t getHatPosition(bool left, bool right, bool down, bool up);
};

#endif