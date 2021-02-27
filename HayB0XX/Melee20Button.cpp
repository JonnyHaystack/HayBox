#include "Melee20Button.h"
#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

#include <Arduino.h>

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

Melee20Button::Melee20Button(socd::SocdType socdType,
                             state::InputState &rInputState,
                             CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});

  mHorizontalSocd = false;
  mFoxCssMacro = 0;
}

void Melee20Button::HandleSocd() {
  mHorizontalSocd = mrInputState.left && mrInputState.right;
  InputMode::HandleSocd();
}

void Melee20Button::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.x = mrInputState.x;
  mOutputState.y = mrInputState.y;
  mOutputState.buttonR = mrInputState.z;
  mOutputState.triggerLDigital = mrInputState.l;
  mOutputState.triggerRDigital = mrInputState.r;
  mOutputState.start = mrInputState.start;

  // D-Pad
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

void Melee20Button::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  bool shield_button_pressed = mrInputState.l || mrInputState.r ||
                               mrInputState.lightshield ||
                               mrInputState.midshield;
  if (mVectorState.diagonal) {
    // L, R, LS, and MS + q1/2 = 7000 7000
    mOutputState.leftStickX = 128 + (mVectorState.directionX * 56);
    mOutputState.leftStickY = 128 + (mVectorState.directionY * 56);
    // L, R, LS, and MS + q3/4 = 7000 6875 (For vanilla shield drop. Gives 44.5
    // degree wavedash).
    if (mVectorState.directionY == -1 && shield_button_pressed) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 56);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 55);
    }
  }

  if (mrInputState.mod_x) {
    // MX + Horizontal (even if shield is held) = 6625 = 53
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
    }
    // MX + Vertical (even if shield is held) = 5375 = 43
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 43);
    }
    if (mVectorState.diagonal) {
      // MX + q1/2/3/4 = 7375 3125 = 59 25
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 59);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 25);
      if (shield_button_pressed) {
        // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 30);
      }
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
    // MY + Horizontal (even if shield is held) = 3375 = 27
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 27);
    }
    // MY + Vertical (even if shield is held) = 7375 = 59
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 59);
    }
    if (mVectorState.diagonal) {
      // MY + q1/2/3/4 = 3125 7375 = 25 59
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 25);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 59);
      if (shield_button_pressed) {
        // MY + L, R, LS, and MS + q1/2 = 4750 8750 = 38 70
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 38);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 70);
        // MY + L, R, LS, and MS + q3/4 = 5000 8500 = 40 68
        if (mVectorState.directionY == -1) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
        }
      }
    }

    // Side B nerf
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

  // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
  // angled fsmash).
  if (mVectorState.directionCX != 0 && mVectorState.directionCY != 0) {
    // 5250 8500 = 42 68
    mOutputState.rightStickX = 128 + (mVectorState.directionCX * 42);
    mOutputState.rightStickY = 128 + (mVectorState.directionCY * 68);
  }

  // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
  // trajectory).
  if (mHorizontalSocd && !mVectorState.vertical) {
    mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
  }

  if (mrInputState.lightshield) {
    mOutputState.triggerRAnalog = 49;
  }
  if (mrInputState.midshield) {
    mOutputState.triggerRAnalog = 94;
  }

  if (mrInputState.l) {
    mOutputState.triggerLAnalog = 140;
  }

  if (mrInputState.r) {
    mOutputState.triggerRAnalog = 140;
  }

  // Shut off c-stick when using dpad layer.
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.rightStickX = 128;
    mOutputState.rightStickY = 128;
  }

  // CSS Macro
  mFoxCssMacro = PickFox(mFoxCssMacro);
}

int Melee20Button::PickFox(int step) {
  // When macro is first triggered, set the stick position and let it be sent
  // to the console.
  if (step == 0) {
    // Mod X and Mod Y must both be held.
    if (!(mrInputState.mod_x && mrInputState.mod_y)) {
      return 0;
    }

    // Select controller port based on top right hand row buttons.
    if (mrInputState.r) {
      step = 1;
    } else if (mrInputState.y) {
      step = 2;
    } else if (mrInputState.lightshield) {
      step = 3;
    } else if (mrInputState.midshield) {
      step = 4;
    }

    // Fairly optimal angles from each port to Fox.
    if (step == 1) {
      mOutputState.leftStickX = 128 + 27;
      mOutputState.leftStickY = 128 + 75;
    } else if (step == 2) {
      mOutputState.leftStickX = 128 - 39;
      mOutputState.leftStickY = 128 + 69;
    } else if (step == 3) {
      mOutputState.leftStickX = 128 - 70;
      mOutputState.leftStickY = 128 + 38;
    } else if (step == 4) {
      mOutputState.leftStickX = 128 - 76;
      mOutputState.leftStickY = 128 + 24;
    }
    return step;
  }

  // On the next update, we want to delay before we send any other inputs.
  if (1 <= step && step <= 4) {
    if (step == 1) {
      delay(180);
    } else if (step == 2) {
      delay(170);
    } else if (step == 3) {
      delay(320);
    } else if (step == 4) {
      delay(530);
    }
    return 5;
  }

  // Next step is to press A to select Fox.
  if (step == 5) {
    mOutputState.a = true;
    return step + 1;
  }

  // Next step is to press Y twice to cycle to Blue Fox.
  if (step == 6) {
    delay(30); // Hold A for 1 frame.
    mOutputState.y = true;
    return step + 1;
  }
  if (step == 7) {
    delay(30); // Hold Y for 1 frame.
    return step + 1;
  }
  if (step == 8) {
    delay(30); // Release Y for 1 frame.
    mOutputState.y = true;
    return step + 1;
  }
  if (step == 9) {
    delay(30); // Hold Y for 1 frame.
    return 0;
  }
}
