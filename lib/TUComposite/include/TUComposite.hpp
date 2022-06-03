#ifndef _TUCOMPOSITE_TUCOMPOSITE_HPP
#define _TUCOMPOSITE_TUCOMPOSITE_HPP

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

// clang-format off

#define HID_REPORT_DESC_GAMEPAD(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_GAMEPAD  )                 ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 16 bit X, Y, Z, Rz, Rx, Ry (min 0 max 65535 ) */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RZ                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX_N  ( 65535, 2                               ) ,\
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
    /* 32 bit Button Map */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 32                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 32                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END

// clang-format on

typedef struct TU_ATTR_PACKED {
    uint16_t x; ///< Delta x  movement of left analog-stick
    uint16_t y; ///< Delta y  movement of left analog-stick
    uint16_t z; ///< Delta z  movement of right analog-joystick
    uint16_t rz; ///< Delta Rz movement of right analog-joystick
    uint16_t rx; ///< Delta Rx movement of analog left trigger
    uint16_t ry; ///< Delta Ry movement of analog right trigger
    uint8_t hat; ///< Buttons mask for currently pressed buttons in the DPad/hat
    uint32_t buttons; ///< Buttons mask for currently pressed buttons
} gamepad_report_t;

typedef enum {
    RID_GAMEPAD = 1,
    RID_KEYBOARD,
} ReportId;

const uint8_t _hid_report_desc[] = {
    HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD)),
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
};

extern Adafruit_USBD_HID _usb_hid;

#endif