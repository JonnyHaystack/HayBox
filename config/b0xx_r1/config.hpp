#ifndef SETUP_R1B0XX_H_1TL59WRN
#define SETUP_R1B0XX_H_1TL59WRN

#include "pinout_r1b0xx.h"

#include "core/CommunicationBackend.h"
#include "core/InputMode.h"
#include "core/socd.h"
#include "core/state.h"
#include "comms/DInputBackend.h"
#include "comms/GamecubeBackend.h"
#include "comms/N64Backend.h"
#include "modes/Melee20Button.h"

CommunicationBackend *gCurrentBackend;
InputMode *gCurrentMode;
state::InputState gInputState;

void initialise() {
  gCurrentBackend = new DInputBackend();
  delay(500);
  bool usb_connected = UDADDR & _BV(ADDEN);

  /* Choose communication backend. */
  if (usb_connected) {
    // Default to DInput mode if USB is connected.
    // Input viewer only used when connected to PC i.e. when using DInput mode.
    Serial.begin(115200, SERIAL_8N1);
  } else {
    delete gCurrentBackend;
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
  }

  /* Always start in Melee mode. Must set mode only after initialising the
     backend. Change the below to Melee18Button for Mod X lightshield */
  gCurrentMode =
      new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState, gCurrentBackend);
}

#endif /* end of include guard: SETUP_R1B0XX_H_1TL59WRN */
