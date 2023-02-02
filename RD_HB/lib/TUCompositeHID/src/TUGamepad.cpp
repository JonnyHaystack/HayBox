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
#include <TUCompositeHID.hpp>

// clang-format off

#define HID_REPORT_DESC(...) \
    HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
    HID_USAGE      ( HID_USAGE_DESKTOP_GAMEPAD  )                 ,\
    HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
        /* Report ID if any */\
        __VA_ARGS__ \
        /* 16 bit X, Y, Z, Rz, Rx, Ry (min -32768 max 32767 ) */ \
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_RZ                   ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ) ,\
        HID_LOGICAL_MIN    ( 0                                      ) ,\
        HID_LOGICAL_MAX_N  ( 0xffff, 3                              ) ,\
        HID_REPORT_COUNT   ( 6                                      ) ,\
        HID_REPORT_SIZE    ( 16                                     ) ,\
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* 8 bit DPad/Hat Button Map  */ \
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_HAT_SWITCH           ) ,\
        HID_LOGICAL_MIN    ( 1                                      ) ,\
        HID_LOGICAL_MAX    ( 8                                      ) ,\
        HID_PHYSICAL_MIN   ( 0                                      ) ,\
        HID_PHYSICAL_MAX_N ( 315, 2                                 ) ,\
        HID_REPORT_COUNT   ( 1                                      ) ,\
        HID_REPORT_SIZE    ( 8                                      ) ,\
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* 16 bit Button Map */ \
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
        HID_USAGE_MIN      ( 1                                      ) ,\
        HID_USAGE_MAX      ( 16                                     ) ,\
        HID_LOGICAL_MIN    ( 0                                      ) ,\
        HID_LOGICAL_MAX    ( 1                                      ) ,\
        HID_REPORT_COUNT   ( 16                                     ) ,\
        HID_REPORT_SIZE    ( 1                                      ) ,\
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    HID_COLLECTION_END

// clang-format on

uint8_t TUGamepad::_descriptor[] = { HID_REPORT_DESC(HID_REPORT_ID(_report_id)) };

TUGamepad::TUGamepad() {}

void TUGamepad::registerDescriptor() {
    TUCompositeHID::addDescriptor(_descriptor, sizeof(_descriptor));
}

void TUGamepad::begin() {
    TUCompositeHID::_usb_hid.begin();

    // Release all buttons, center all sticks, etc.
    resetInputs();
}

bool TUGamepad::ready() {
    return TUCompositeHID::_usb_hid.ready();
};

bool TUGamepad::sendState() {
    return TUCompositeHID::_usb_hid.sendReport(_report_id, &_report, sizeof(gamepad_report_t));
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
    _report.x = value * 257;
}

void TUGamepad::leftYAxis(uint8_t value) {
    _report.y = value * 257;
}

void TUGamepad::rightXAxis(uint8_t value) {
    _report.rx = value * 257;
}

void TUGamepad::rightYAxis(uint8_t value) {
    _report.ry = value * 257;
}

void TUGamepad::triggerLAnalog(uint8_t value) {
    _report.z = value * 257;
}

void TUGamepad::triggerRAnalog(uint8_t value) {
    _report.rz = value * 257;
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
