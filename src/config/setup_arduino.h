#ifndef SETUP_ARDUINO_H_OIVLOHCL
#define SETUP_ARDUINO_H_OIVLOHCL

#include "core/CommunicationBackend.h"
#include "core/InputMode.h"
#include "core/socd.h"
#include "core/state.h"
#include "comms/GamecubeBackend.h"
#include "comms/N64Backend.h"
#include "modes/Melee20Button.h"

#include "pinout_arduino.h"

CommunicationBackend *gCurrentBackend;
InputMode *gCurrentMode;
state::InputState gInputState;

void initialise() {
  /* Choose communication backend. */
  if (gInputState.c_left) {
    // Hold C-Left on plugin for N64.
    gCurrentBackend = new N64Backend(60, pinout::GCC_DATA);
  } else if (gInputState.a) {
    // Hold A on plugin for GameCube adapter.
    gCurrentBackend = new GamecubeBackend(0, pinout::GCC_DATA);
  } else {
    // Default to GameCube/Wii.
    gCurrentBackend = new GamecubeBackend(125, pinout::GCC_DATA);
  }

  /* Always start in Melee mode. Must set mode only after initialising the
     backend. */
  gCurrentMode =
      new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState, gCurrentBackend);

  // Uncomment the following to enable input viewer when USB and GameCube
  // cable are both connected.
  // Serial.begin(115200, SERIAL_8N1);
}

#endif /* end of include guard: SETUP_ARDUINO_H_OIVLOHCL */
