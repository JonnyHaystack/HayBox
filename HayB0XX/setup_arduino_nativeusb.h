#ifndef SETUP_ARDUINO_NATIVEUSB_H_4FOBJGQ0
#define SETUP_ARDUINO_NATIVEUSB_H_4FOBJGQ0

#include "pinout_arduino.h"

#include "CommunicationBackend.h"
#include "DInputBackend.h"
#include "GamecubeBackend.h"
#include "InputMode.h"
#include "Melee20Button.h"
#include "N64Backend.h"
#include "socd.h"
#include "state.h"

CommunicationBackend *gCurrentBackend;
InputMode *gCurrentMode;
state::InputState gInputState;

void initialise() {
  /* Choose communication backend. Default to DInput mode. Hold C-Down on plugin
     for GameCube mode. */
  if (gInputState.c_down) {
    gCurrentBackend = new GamecubeBackend(125, pinout::GCC_DATA);
  } else if (gInputState.c_left) {
    // Hold C-Left on plugin for N64 mode.
    gCurrentBackend = new N64Backend(60, pinout::GCC_DATA);
  } else {
    gCurrentBackend = new DInputBackend();
    // Input viewer only used when connected to PC i.e. when using DInput mode.
    Serial.begin(115200, SERIAL_8N1);
  }

  /* Always start in Melee mode. Must set mode only after initialising the
     backend. */
  gCurrentMode =
      new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState, gCurrentBackend);
}

#endif /* end of include guard: SETUP_ARDUINO_NATIVEUSB_H_4FOBJGQ0 */
