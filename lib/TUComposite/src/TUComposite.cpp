#include "TUComposite.hpp"

#include <Adafruit_TinyUSB.h>

Adafruit_USBD_HID _usb_hid =
    Adafruit_USBD_HID(_hid_report_desc, sizeof(_hid_report_desc), HID_ITF_PROTOCOL_NONE, 1, false);