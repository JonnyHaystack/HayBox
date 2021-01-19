#include "RocketLeague.h"
#include "CommunicationBackend.h"
#include "ControllerMode.h"
#include "socd.h"
#include "state.h"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

RocketLeague::RocketLeague(socd::SocdType socdType,
                           state::InputState &rInputState,
                           CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.mod_x});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
}

void RocketLeague::HandleSocd() {
  if (mrInputState.mod_x && mrInputState.down) {
    mrInputState.down = false;
  }
  InputMode::HandleSocd();
}

void RocketLeague::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.x = mrInputState.midshield;
  mOutputState.y = mrInputState.up;
  mOutputState.buttonL = mrInputState.l;
  mOutputState.buttonR = mrInputState.lightshield;
  mOutputState.triggerLDigital = mrInputState.z;
  mOutputState.triggerRDigital = mrInputState.x;
  mOutputState.leftStickClick = mrInputState.r;

  // Hold accelerate and reverse simultaneously for rear view.
  if (mrInputState.z && mrInputState.x) {
    mOutputState.rightStickClick = true;
    // Override (deactivate) accelerator.
    mOutputState.triggerRDigital = false;
  }

  // MX + Start = Select
  if (mrInputState.mod_x)
    mOutputState.select = mrInputState.start;
  else
    mOutputState.start = mrInputState.start;

  // D-Pad
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.dpadUp = mrInputState.c_up;
    mOutputState.dpadDown = mrInputState.c_down;
    mOutputState.dpadLeft = mrInputState.c_left;
    mOutputState.dpadRight = mrInputState.c_right;
  }
}

void RocketLeague::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.mod_x, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  if (mrInputState.mod_y) {
    if (mVectorState.diagonal) {
      mOutputState.leftStickX =
          ANALOG_STICK_NEUTRAL + (mVectorState.directionX * 70);
      // mOutputState.leftStickY =
      // ANALOG_STICK_NEUTRAL + (mVectorState.directionY * 76);
    } else if (mVectorState.horizontal) {
      mOutputState.leftStickX =
          ANALOG_STICK_NEUTRAL + (mVectorState.directionX * 35);
    } else if (mVectorState.vertical) {
      mOutputState.leftStickY =
          ANALOG_STICK_NEUTRAL + (mVectorState.directionY * 76);
    }
  } else if (mVectorState.diagonal) {
    // Good speed flip angle with no mods.
    mOutputState.leftStickX =
        ANALOG_STICK_NEUTRAL + (mVectorState.directionX * 70);
  }

  // Shut off right stick when using dpad layer.
  if (mrInputState.mod_x && mrInputState.mod_y) {
    mOutputState.rightStickX = ANALOG_STICK_NEUTRAL;
    mOutputState.rightStickY = ANALOG_STICK_NEUTRAL;
  }
}
