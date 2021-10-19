/* Ultimate2 profile by Taker */
#include "Ultimate2.h"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate2::Ultimate2(socd::SocdType socdType,
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

void Ultimate2::HandleSocd() {
  mHorizontalSocd = mrInputState.left && mrInputState.right;
  InputMode::HandleSocd();
}

void Ultimate2::UpdateDigitalOutputs() {
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

void Ultimate2::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  bool shield_button_pressed = mrInputState.l || mrInputState.r ||
                               mrInputState.lightshield ||
                               mrInputState.midshield;

  if (mrInputState.mod_x) {
    // MX + Horizontal = 6625 = 53
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
      // Horizontal Shield tilt = 51
      if (shield_button_pressed) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
      }
      // Horizontal Tilts = 36
      if (mrInputState.a) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 36);
      }
    }
    // MX + Vertical = 44
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 44);
      // Vertical Shield Tilt = 51
      if (shield_button_pressed) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 51);
      }
    }
    if (mVectorState.diagonal) {
      // MX + q1/2/3/4 = 53 40
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 40);
      if (shield_button_pressed) {
        // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 30);
      }
    }

    // Angled fsmash/ftilt with C-Stick + MX
    if (mVectorState.directionCX != 0) {
      mOutputState.rightStickX = 128 + (mVectorState.directionCX * 127);
      mOutputState.rightStickY = 128 + (mVectorState.directionY * 59);
    }

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      // (33.44) = 53 40
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 53);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 40);
      
      // Angled Ftilts
      if (mrInputState.a) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 36);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 26);
      }
    }
  }

  if (mrInputState.mod_y) {
    // MY + Horizontal (even if shield is held) = 41
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 41);
      // MY Horizontal Tilts
      if (mrInputState.a) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 36);
      }
    }
    // MY + Vertical (even if shield is held) = 44
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 44);
      // MY Vertical Tilts
      if (mrInputState.a) {
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 36);
      }
    }
    if (mVectorState.diagonal) {
      // MY + q1/2/3/4 = 41 44
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 41);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 44);
      if (shield_button_pressed) {
        // MY + L, R, LS, and MS + q1/2 = 38 70
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 38);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 70);
        // MY + L, R, LS, and MS + q3/4 = 40 68
        if (mVectorState.directionX == -1) {
          mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
          mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
        }
      }
    }

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      // (56.56) = 41 44
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 41);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 44);

      // MY Pivot Uptilt/Dtilt
      if (mrInputState.a) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 34);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 38);
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
}