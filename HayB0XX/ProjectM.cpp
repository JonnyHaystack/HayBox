#include "ProjectM.h"
#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

ProjectM::ProjectM(socd::SocdType socdType, state::InputState &rInputState,
                   CommunicationBackend *communicationBackend,
                   bool ledgedashMaxJumpTraj, bool trueZPress)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mLedgedashMaxJumpTraj = ledgedashMaxJumpTraj;
  mTrueZPress = trueZPress;

  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});

  mHorizontalSocd = false;
}

void ProjectM::HandleSocd() {
  mHorizontalSocd = mrInputState.left && mrInputState.right;
  InputMode::HandleSocd();
}

void ProjectM::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.x = mrInputState.x;
  mOutputState.y = mrInputState.y;
  // True Z press vs macro lightshield + A.
  if (mTrueZPress || mrInputState.mod_x) {
    mOutputState.buttonR = mrInputState.z;
  } else {
    mOutputState.a = mrInputState.a || mrInputState.z;
  }
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

  // Don't override dpad up if it's already pressed using the MX + MY dpad
  // layer.
  mOutputState.dpadUp = mOutputState.dpadUp || mrInputState.midshield;

  if (mrInputState.select)
    mOutputState.dpadLeft = true;
  if (mrInputState.home)
    mOutputState.dpadRight = true;
}

void ProjectM::UpdateAnalogOutputs() {
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  bool shield_button_pressed =
      mrInputState.l || mrInputState.lightshield || mrInputState.midshield;

  if (mVectorState.diagonal) {
    if (mVectorState.directionY == 1) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 83);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 93);
    }
  }

  if (mrInputState.mod_x) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 70);
    }
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 60);
    }

    if (mVectorState.directionCX != 0) {
      mOutputState.rightStickX = 128 + (mVectorState.directionCX * 65);
      mOutputState.rightStickY = 128 + (mVectorState.directionY * 23);
    }

    if (mVectorState.diagonal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 70);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 34);

      if (mrInputState.b) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 85);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 31);
      }

      if (mrInputState.r) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 82);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 35);
      }

      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 77);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 55);
      }

      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 82);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 36);
      }

      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 84);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 50);
      }

      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 72);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 61);
      }
    }
  }

  if (mrInputState.mod_y) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 35);
    }
    if (mVectorState.vertical) {
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 70);
    }

    if (mVectorState.diagonal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 28);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 58);

      if (mrInputState.b) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 28);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 85);
      }

      if (mrInputState.r) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 82);
      }

      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 55);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 77);
      }

      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 34);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 82);
      }

      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 84);
      }

      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 62);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 72);
      }
    }
  }

  // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
  // angled fsmash).
  // We don't apply this for c-up + c-left/c-right in case we want to implement
  // C-stick nair somehow.
  if (mVectorState.directionCX != 0 && mVectorState.directionCY == -1) {
    // 3000 9875 = 30 78
    mOutputState.rightStickX = 128 + (mVectorState.directionCX * 35);
    mOutputState.rightStickY = 128 + (mVectorState.directionCY * 98);
  }

  // Horizontal SOCD overrides X-axis modifiers (for ledgedash maximum jump
  // trajectory).
  if (mLedgedashMaxJumpTraj && mHorizontalSocd && !mVectorState.vertical &&
      !shield_button_pressed) {
    mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
  }

  if (mrInputState.lightshield) {
    mOutputState.triggerRAnalog = 49;
  }
  if (mrInputState.midshield) {
    mOutputState.triggerRAnalog = 94;
  }

  // Send lightshield input if we are using Z = lightshield + A macro.
  if (mrInputState.z && !(mrInputState.mod_x || mTrueZPress)) {
    mOutputState.triggerRAnalog = 49;
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
