/*
    The MIT License (MIT)

    Copyright (c) 2018, hathach for Adafruit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#ifndef _TUSB_CONFIG_RP2040_H_
#define _TUSB_CONFIG_RP2040_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE

// Enable device stack
#define CFG_TUD_ENABLED 1

// Enable host stack with pio-usb if Pico-PIO-USB library is available
#if __has_include("pio_usb.h")
#define CFG_TUH_ENABLED 1
#define CFG_TUH_RPI_PIO_USB 1
#endif

#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU OPT_MCU_RP2040
#endif
#define CFG_TUSB_OS OPT_OS_PICO

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

#if CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG_PRINTF serial1_printf
extern int serial1_printf(const char *__restrict __format, ...);
#endif

#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN TU_ATTR_ALIGNED(4)

//--------------------------------------------------------------------
// Device Configuration
//--------------------------------------------------------------------

#define CFG_TUD_ENDOINT0_SIZE 64

#define CFG_TUD_CDC 1
#define CFG_TUD_MSC 1
#define CFG_TUD_HID 1
#define CFG_TUD_MIDI 1
#define CFG_TUD_VENDOR 0

// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE 256
#define CFG_TUD_CDC_TX_BUFSIZE 256

// MSC Buffer size of Device Mass storage
#define CFG_TUD_MSC_EP_BUFSIZE 512

// HID buffer size Should be sufficient to hold ID (if any) + Data
#define CFG_TUD_HID_EP_BUFSIZE 64

// MIDI FIFO size of TX and RX
#define CFG_TUD_MIDI_RX_BUFSIZE 128
#define CFG_TUD_MIDI_TX_BUFSIZE 128

// Vendor FIFO size of TX and RX
#define CFG_TUD_VENDOR_RX_BUFSIZE 64
#define CFG_TUD_VENDOR_TX_BUFSIZE 64

//--------------------------------------------------------------------
// Host Configuration
//--------------------------------------------------------------------

// Size of buffer to hold descriptors and other data used for enumeration
#define CFG_TUH_ENUMERATION_BUFSIZE 256

#define CFG_TUH_HUB 1
// max device support (excluding hub device)
#define CFG_TUH_DEVICE_MAX (CFG_TUH_HUB ? 4 : 1) // hub typically has 4 ports

// Enable tuh_edpt_xfer() API
//#define CFG_TUH_API_EDPT_XFER       1

#define CFG_TUH_HID 4

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_RP2040_H_ */
