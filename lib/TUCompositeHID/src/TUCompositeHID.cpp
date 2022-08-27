#include "TUCompositeHID.hpp"

#include <Adafruit_TinyUSB.h>

#define HID_DESCRIPTOR_BUFSIZE 1024U

namespace TUCompositeHID {
    uint8_t _hid_report_desc[HID_DESCRIPTOR_BUFSIZE] = {};
    size_t _current_descriptor_len = 0;

    Adafruit_USBD_HID _usb_hid = Adafruit_USBD_HID(
        _hid_report_desc,
        _current_descriptor_len,
        HID_ITF_PROTOCOL_NONE,
        1,
        false
    );

    bool addDescriptor(uint8_t *descriptor, size_t descriptor_len) {
        if (_current_descriptor_len + descriptor_len > HID_DESCRIPTOR_BUFSIZE) {
            return false;
        }
        for (size_t i = 0; i < descriptor_len; i++) {
            _hid_report_desc[_current_descriptor_len + i] = descriptor[i];
        }
        _current_descriptor_len += descriptor_len;

        _usb_hid.setReportDescriptor(_hid_report_desc, _current_descriptor_len);

        return true;
    }
}
