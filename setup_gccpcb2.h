#ifndef SETUP_GCCPCB2_H_59DFLKAP
#define SETUP_GCCPCB2_H_59DFLKAP

#include "pinout_gccpcb2.h"

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
  pinMode(pinout::SWITCH, OUTPUT);

  // Hold Mod X + Mod Y on plugin for Brook board mode.
  if (gInputState.mod_x && gInputState.mod_y)
    digitalWrite(pinout::SWITCH, HIGH);
  else
    digitalWrite(pinout::SWITCH, LOW);

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

#endif /* end of include guard: SETUP_GCCPCB2_H_59DFLKAP */
