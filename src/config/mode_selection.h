#ifndef MODE_SELECTION_H_METMBOLA
#define MODE_SELECTION_H_METMBOLA

#include "core/InputMode.h"
#include "core/state.h"
#include "modes/FgcMode.h"
#include "modes/Melee20Button.h"
#include "modes/ProjectM.h"

#include "modes/DefaultKeyboardMode.h"

extern CommunicationBackend *gCurrentBackend;
extern InputMode *gCurrentMode;
extern state::InputState gInputState;

void selectInputMode() {
  if (gInputState.mod_x && !gInputState.mod_y && gInputState.start) {
    if (gInputState.l) {
      delete gCurrentMode;
      gCurrentMode = new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState,
                                       gCurrentBackend);
    } else if (gInputState.left) {
      delete gCurrentMode;
      gCurrentMode = new ProjectM(socd::SOCD_2IP_NO_REAC, gInputState,
                                  gCurrentBackend, true, false);
    } else if (gInputState.down) {
      delete gCurrentMode;
      gCurrentMode =
          new FgcMode(socd::SOCD_NEUTRAL, gInputState, gCurrentBackend);
    }
  } else if (gInputState.mod_y && !gInputState.mod_x && gInputState.start) {
    if (gInputState.l) {
      delete gCurrentMode;
      gCurrentMode = new DefaultKeyboardMode(socd::SOCD_2IP, gInputState);
    }
  }
}

#endif /* end of include guard: MODE_SELECTION_H_METMBOLA */
