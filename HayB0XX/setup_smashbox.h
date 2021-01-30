#ifndef SETUP_SMASHBOX_H_J3HYSKAN
#define SETUP_SMASHBOX_H_J3HYSKAN

#include "pinout_smashbox.h"

#include "CommunicationBackend.h"
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
  /* Choose communication backend. */
  if (gInputState.c_left) {
    // Hold C-Left on plugin for N64 mode.
    gCurrentBackend = new N64Backend(60, pinout::GCC_DATA);
  } else {
    // Default to GameCube mode.
    gCurrentBackend = new GamecubeBackend(125, pinout::GCC_DATA);
  }

  /* Always start in Melee mode. Must set mode only after initialising the
     backend. */
  gCurrentMode =
      new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState, gCurrentBackend);

  // Uncomment the following to enable input viewer when USB and GameCube
  // cable are both connected. Not sure if this works on Smash Box.
  // Serial.begin(115200, SERIAL_8N1);
}

#endif /* end of include guard: SETUP_SMASHBOX_H_J3HYSKAN */
