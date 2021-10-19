#ifndef SETUP_H_YUWJ6QGL
#define SETUP_H_YUWJ6QGL

#if defined(ARDUINO_NOUSB)
#include "setup_arduino.h"
#elif defined(ARDUINO_NATIVEUSB)
#include "setup_arduino_nativeusb.h"
#elif defined(GCCPCB1)
#include "setup_gccpcb1.h"
#elif defined(GCCPCB2)
#include "setup_gccpcb2.h"
#elif defined(GCCMX)
#include "setup_gccmx.h"
#elif defined(R1B0XX)
#include "setup_r1b0xx.h"
#elif defined(R2B0XX)
#include "setup_r2b0xx.h"
#elif defined(SMASHBOX)
#include "setup_smashbox.h"
#endif

#endif /* end of include guard: SETUP_H_YUWJ6QGL */
