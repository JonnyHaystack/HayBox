#include "Melee18Button.h"
#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

Melee18Button::Melee18Button(socd::SocdType socdType,
                             state::InputState &rInputState,
                             CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});

  mHorizontalSocd = false;
}

void Melee18Button::HandleSocd() {
  mHorizontalSocd = mrInputState.left && mrInputState.right;
  InputMode::HandleSocd();
}

void Melee18Button::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.x = mrInputState.x;
  mOutputState.y = mrInputState.y;
  mOutputState.buttonR = mrInputState.z;
  mOutputState.triggerLDigital = mrInputState.l;
  mOutputState.triggerRDigital = mrInputState.r;
  mOutputState.start = mrInputState.start;

  /********* DPAD *********/
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.dpadUp = mrInputState.c_up;
    mOutputState.dpadDown = mrInputState.c_down;
    mOutputState.dpadLeft = mrInputState.c_left;
    mOutputState.dpadRight = mrInputState.c_right;
  }

  if (mrInputState.select)
    mOutputState.dpadLeft = true;
  if (mrInputState.home)
    mOutputState.dpadRight = true;
}

void Melee18Button::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  bool shield_button_pressed = mrInputState.l || mrInputState.r ||
                               mrInputState.lightshield ||
                               mrInputState.midshield;

  if (mrInputState.mod_x) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
    }
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 23);
    }

    // Side B nerf
    if (mrInputState.b) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
    }

    // Angled fsmash
    if (mVectorState.directionCX != 0) {
      mOutputState.rightStickX = 128 + (mVectorState.directionCX * 65);
      mOutputState.rightStickY = 128 + (mVectorState.directionY * 23);
    }

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      // 22.9638 - 7375 3125 = 59 25
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 59);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 25);
      // 27.37104 - 7000 3625 (27.38) = 56 29
      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 56);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 29);
      }
      // 31.77828 - 7875 4875 (31.76) = 63 39
      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 63);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 39);
      }
      // 36.18552 - 7000 5125 (36.21) = 56 41
      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 56);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 41);
      }
      // 40.59276 - 6125 5250 (40.6) = 49 42
      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 49);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 42);
      }

      /* Extended Up B Angles */
      if (mrInputState.b) {
        // 22.9638 - 9125 3875 (23.0) = 73 31
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 73);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 31);
        // 27.37104 - 8750 4500 (27.2) = 70 36
        if (mrInputState.c_down) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 70);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 36);
        }
        // 31.77828 - 8500 5250 (31.7) = 68 42
        if (mrInputState.c_left) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 68);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 42);
        }
        // 36.18552 - 7375 5375 (36.1) = 59 43
        if (mrInputState.c_up) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 59);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 43);
        }
        // 40.59276 - 6375 5375 (40.1) = 51 43
        if (mrInputState.c_right) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 43);
        }
      }
    }
  }

  if (mrInputState.mod_y) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 27);
    }
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 59);
    }

    if (mrInputState.b) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 80);
    }

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      // 67.0362 - 3125 7375 = 25 59
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 25);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 59);
      // 62.62896 - 3625 7000 (62.62) = 29 56
      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 29);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 56);
      }
      // 58.22172 - 4875 7875 (58.24) = 39 63
      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 39);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 63);
      }
      // 53.81448 - 5125 7000 (53.79) = 41 56
      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 41);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 56);
      }
      // 49.40724 - 6375 7625 (50.10) = 51 61
      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 61);
      }

      /* Extended Up B Angles */
      if (mrInputState.b) {
        // 67.0362 - 3875 9125 = 31 73
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 31);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 73);
        // 62.62896 - 4500 8750 (62.8) = 36 70
        if (mrInputState.c_down) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 36);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 70);
        }
        // 58.22172 - 5250 8500 (58.3) = 42 68
        if (mrInputState.c_left) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 42);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
        }
        // 53.81448 - 5875 8000 (53.7) = 47 64
        if (mrInputState.c_up) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 47);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 64);
        }
        // 49.40724 - 5875 7125 (50.49) = 47 57
        if (mrInputState.c_right) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 47);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 57);
        }
      }
    }
  }

  if (mrInputState.l) {
    if (mVectorState.horizontal)
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
    if (mVectorState.vertical)
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 100);
    if (mVectorState.horizontal && (mVectorState.directionY == 1)) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 43);
      mOutputState.leftStickY = 128 + 43;
    }
    if (mVectorState.horizontal && (mVectorState.directionY == -1)) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 57);
      mOutputState.leftStickY = 128 - 55;
    }
    if (mrInputState.mod_x || mrInputState.mod_y) {
      if (!(mrInputState.mod_x && mrInputState.mod_y)) {
        mOutputState.triggerLDigital = false;
        mOutputState.triggerRAnalog = 49;
      }

      if (mVectorState.diagonal) {
        if (mrInputState.mod_x) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 30);
        }
        if (mrInputState.mod_y) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
        }
      }
    }
  }

  if (mrInputState.r) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
    }
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 43);
    }
    if (mVectorState.diagonal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 43);
      if (mrInputState.mod_x) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 30);
      }
      if (mrInputState.mod_y) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
      }
    }
  }

  // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
  // angled fsmash).
  if (mVectorState.directionCX != 0 && mVectorState.directionCY != 0) {
    // 5250 8500 = 42 68
    mOutputState.rightStickX = 128 + (mVectorState.directionCX * 42);
    mOutputState.rightStickY = 128 + (mVectorState.directionCY * 68);
  }

  // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
  // trajectory).
  if (!mrInputState.r && mHorizontalSocd && !mVectorState.vertical) {
    mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
  }

  // Shut off c-stick when using dpad layer.
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.rightStickX = 128;
    mOutputState.rightStickY = 128;
  }
}
