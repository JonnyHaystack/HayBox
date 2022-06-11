#include "TUKeyboard.hpp"

#include "TUComposite.hpp"

#include <Adafruit_TinyUSB.h>

#define MODIFIER_MASK(mod_kc) (1 << (mod_kc & 0x0F))

TUKeyboard::TUKeyboard() {}

void TUKeyboard::begin() {
    _usb_hid.begin();
    releaseAll();
}

void TUKeyboard::press(uint8_t keycode) {
    // If keycode >= E0 then it's a modifier key.
    if (keycode >= 0xE0) {
        // Create bitmask from the modifier keycode to set the corresponding bit in the modifier
        // byte.
        uint8_t bitmask = MODIFIER_MASK(keycode);
        _report.modifier |= bitmask;
        return;
    }

    // Check if the key is already pressed so that we don't send the same keycode multiple times in
    // the same report.
    for (int i = 0; i < 6; i++) {
        if (_report.keycode[i] == keycode) {
            return;
        }
    }

    // Place this keycode in the report in place of the first empty keycode.
    for (int i = 0; i < 6; i++) {
        if (_report.keycode[i] == HID_KEY_NONE) {
            _report.keycode[i] = keycode;
            return;
        }
    }
}

void TUKeyboard::release(uint8_t keycode) {
    // If keycode >= E0 then it's a modifier key.
    if (keycode >= 0xE0) {
        // Create bitmask from the modifier keycode to unset the corresponding bit in the modifier
        // byte.
        uint8_t bitmask = ~MODIFIER_MASK(keycode);
        _report.modifier &= bitmask;
        return;
    }

    // Loop through keycodes in report. If we find the specified key code, we clear it.
    for (int i = 0; i < 6; i++) {
        if (_report.keycode[i] == keycode) {
            _report.keycode[i] = HID_KEY_NONE;
        }
    }
}

void TUKeyboard::setPressed(uint8_t keycode, bool pressed) {
    if (pressed) {
        press(keycode);
    } else {
        release(keycode);
    }
}

void TUKeyboard::releaseAll() {
    for (int i = 0; i < 6; i++) {
        _report.keycode[i] = 0;
    }
    _report.modifier = 0;
}

void TUKeyboard::sendState() {
    while (!_usb_hid.ready()) {
        tight_loop_contents();
    }
    _usb_hid.sendReport(RID_KEYBOARD, &_report, sizeof(hid_keyboard_report_t));
}