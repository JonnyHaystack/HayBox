// Haystack's firmware for RoA using numbers directly translated from Crane's code

#include "RoA.h"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

RoA::RoA(socd::SocdType socdType,
                             state::InputState &rInputState,
                             CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});

}

void RoA::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.x = mrInputState.x;
  mOutputState.y = mrInputState.y;
  mOutputState.buttonR = mrInputState.z;
  if (mrInputState.nunchuk_connected) {
    // Lightshield with C button.
    if (mrInputState.nunchuk_c) {
      mOutputState.triggerLAnalog = 49;
    }
    mOutputState.triggerLDigital = mrInputState.nunchuk_z;
  } else {
    mOutputState.triggerLDigital = mrInputState.l;
  }
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
    mOutputState.select = true;
  if (mrInputState.home)
    mOutputState.home = true;
}

void RoA::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  bool shield_button_pressed = mrInputState.l || mrInputState.r;

  if (mrInputState.mod_x) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 66);
    }

    // Angled fsmash
    if (mVectorState.directionCX != 0) {
      
      mOutputState.rightStickX = 128 + (mVectorState.directionCX * 65);
      mOutputState.rightStickY = 128 + (mVectorState.directionY * 23);
    }

    // Need to check coord system in RoA

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 59);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 23);
      
      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 49);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 24);
      }
      
      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 52);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 31);
      }
      
      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 49);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 35);
      }
     
      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 51);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 43);
      }

    }
  }

  if (mrInputState.mod_y) {
    if (mVectorState.horizontal) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 44);
    }

    /* Up B angles */
    if (mVectorState.diagonal && !shield_button_pressed) {
      
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 44);
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 113);
      
      if (mrInputState.c_down) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 44);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 90);
      }
      
      if (mrInputState.c_left) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 44);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 74);
      }
      
      if (mrInputState.c_up) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 45);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 63);
      }
      
      if (mrInputState.c_right) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 47);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 57);
      }
    }
  }

  if (mrInputState.l) {
    if (mVectorState.horizontal)
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
    if (mVectorState.vertical)
      mOutputState.leftStickY = 128 + (mVectorState.directionY * 100);
    if (mVectorState.horizontal && (mVectorState.directionY == -1)) {
      mOutputState.leftStickX = 128 + (mVectorState.directionX * 100);
      mOutputState.leftStickY = ANALOG_STICK_MIN;
    }
  }

  if (mrInputState.r) {
    if (mVectorState.diagonal) {
      if (mrInputState.mod_y) {
        mOutputState.leftStickX = 128 + (mVectorState.directionX * 40);
        mOutputState.leftStickY = 128 + (mVectorState.directionY * 68);
      }
    }
  }

  // Shut off c-stick when using dpad layer.
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.rightStickX = 128;
    mOutputState.rightStickY = 128;
  }
}
