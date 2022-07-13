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

#include "TUComposite.hpp"

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

TUGamepad::TUGamepad() {}

void TUGamepad::begin() {
    _usb_hid.begin();

    // Release all buttons, center all sticks, etc.
    resetInputs();
}

bool TUGamepad::ready() {
    return _usb_hid.ready();
};

bool TUGamepad::sendState() {
    return _usb_hid.sendReport(RID_GAMEPAD, &_report, sizeof(gamepad_report_t));
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

void TUGamepad::buttons(uint16_t buttons) {
    _report.buttons = buttons;
}

void TUGamepad::leftXAxis(uint8_t value) {
    _report.x = ((value - 128) << 8) + 127;
}

void TUGamepad::leftYAxis(uint8_t value) {
    _report.y = ((value - 128) << 8) + 127;
}

void TUGamepad::rightXAxis(uint8_t value) {
    _report.rx = ((value - 128) << 8) + 127;
}

void TUGamepad::rightYAxis(uint8_t value) {
    _report.ry = ((value - 128) << 8) + 127;
}

void TUGamepad::triggerLAnalog(uint8_t value) {
    _report.z = ((value - 128) << 8) + 127;
}

void TUGamepad::triggerRAnalog(uint8_t value) {
    _report.rz = ((value - 128) << 8) + 127;
}

void TUGamepad::hatSwitch(hid_gamepad_hat_t direction) {
    _report.hat = direction;
}

void TUGamepad::hatSwitch(bool left, bool right, bool down, bool up) {
    _report.hat = getHatPosition(left, right, down, up);
}

hid_gamepad_hat_t TUGamepad::getHatPosition(bool left, bool right, bool down, bool up) {
    hid_gamepad_hat_t angle = GAMEPAD_HAT_CENTERED;
    if (right && !left) {
        angle = GAMEPAD_HAT_RIGHT;
        if (down)
            angle = GAMEPAD_HAT_DOWN_RIGHT;
        if (up)
            angle = GAMEPAD_HAT_UP_RIGHT;
    } else if (left && !right) {
        angle = GAMEPAD_HAT_LEFT;
        if (down)
            angle = GAMEPAD_HAT_DOWN_LEFT;
        if (up)
            angle = GAMEPAD_HAT_UP_LEFT;
    } else if (down && !up) {
        angle = GAMEPAD_HAT_DOWN;
    } else if (up && !down) {
        angle = GAMEPAD_HAT_UP;
    }
    return angle;
}
