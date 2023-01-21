#ifndef _TUCOMPOSITE_TUCOMPOSITE_HPP
#define _TUCOMPOSITE_TUCOMPOSITE_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

namespace TUCompositeHID {
    extern Adafruit_USBD_HID _usb_hid;

    bool addDescriptor(uint8_t *descriptor, size_t descriptor_len);
}

#endif