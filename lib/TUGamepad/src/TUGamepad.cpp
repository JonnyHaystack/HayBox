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
#include "TUGamepad.hpp"

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

TUGamepad::TUGamepad() {
    _usb_hid.setPollInterval(1);
    _usb_hid.setReportDescriptor(_gamepad_hid_report_desc, sizeof(_gamepad_hid_report_desc));
}

void TUGamepad::begin() {
    _usb_hid.begin();

    // Release all buttons, center all sticks, etc.
    resetInputs();
}

bool TUGamepad::ready() {
    return _usb_hid.ready();
};

bool TUGamepad::sendState() {
    return _usb_hid.sendReport(0, &_report, sizeof(gamepad_report_t));
}

void TUGamepad::resetInputs() {
    releaseAll();
    leftXAxis(128);
    leftYAxis(128);
    rightXAxis(128);
    rightYAxis(128);
    triggerLAnalog(0);
    triggerRAnalog(0);
}

void TUGamepad::press(uint8_t button) {
    _report.buttons |= TU_BIT(button);
}

void TUGamepad::release(uint8_t button) {
    _report.buttons &= ~TU_BIT(button);
}

void TUGamepad::releaseAll() {
    _report.buttons = 0;
}

void TUGamepad::setButton(uint8_t button, bool pressed) {
    if (pressed) {
        press(button);
    } else {
        release(button);
    }
}

void TUGamepad::buttons(uint32_t buttons) {
    _report.buttons = buttons;
}

void TUGamepad::leftXAxis(uint8_t value) {
    _report.x = (value << 8) + 127;
}

void TUGamepad::leftYAxis(uint8_t value) {
    _report.y = (value << 8) + 127;
}

void TUGamepad::rightXAxis(uint8_t value) {
    _report.rx = (value << 8) + 127;
}

void TUGamepad::rightYAxis(uint8_t value) {
    _report.ry = (value << 8) + 127;
}

void TUGamepad::triggerLAnalog(uint8_t value) {
    _report.z = (value << 8) + 127;
}

void TUGamepad::triggerRAnalog(uint8_t value) {
    _report.rz = (value << 8) + 127;
}

void TUGamepad::hatSwitch(hid_gamepad_hat_t direction) {
    _report.hat = direction;
}
