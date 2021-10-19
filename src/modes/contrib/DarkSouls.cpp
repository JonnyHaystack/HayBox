#include "DarkSouls.h"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

DarkSouls::DarkSouls(socd::SocdType socdType, state::InputState &rInputState,
                     CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.mod_x});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
}

void DarkSouls::UpdateDigitalOutputs() {
  mOutputState.y = mrInputState.y;
  mOutputState.x = mrInputState.r;

  // Base layer.
  bool layer0 = !mrInputState.x && !mrInputState.nunchuk_z;
  // Secondary layer when X is held.
  bool layerX = mrInputState.x && !mrInputState.nunchuk_z;
  // DPad layer when Nunchuk Z is held.
  bool layerZ = mrInputState.nunchuk_z;

  if (layer0) {
    mOutputState.a = mrInputState.a;
    mOutputState.b = mrInputState.b;
    mOutputState.buttonR = mrInputState.z;
    mOutputState.buttonL = mrInputState.up;
    mOutputState.start = mrInputState.start | mrInputState.nunchuk_c;
  } else if (layerX) {
    mOutputState.rightStickClick = mrInputState.a;
    mOutputState.triggerRDigital = mrInputState.z;
    mOutputState.triggerLDigital = mrInputState.up;
    mOutputState.select = mrInputState.start;
  } else if (layerZ) {
    mOutputState.dpadLeft = mrInputState.b;
    mOutputState.dpadDown = mrInputState.x;
    mOutputState.dpadUp = mrInputState.z;
    mOutputState.dpadRight = mrInputState.up;
    mOutputState.select = mrInputState.nunchuk_c;
  }
}

void DarkSouls::UpdateAnalogOutputs() {
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.mod_x, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  // L reduces all coordinates.
  if (mrInputState.l) {
    mOutputState.leftStickX =
        ANALOG_STICK_NEUTRAL + (mVectorState.directionX * 53);
    mOutputState.leftStickY =
        ANALOG_STICK_NEUTRAL + (mVectorState.directionY * 53);
    mOutputState.rightStickX =
        ANALOG_STICK_NEUTRAL + (mVectorState.directionCX * 70);
    mOutputState.rightStickY =
        ANALOG_STICK_NEUTRAL + (mVectorState.directionCY * 70);
  }

  mOutputState.rightStickX = ANALOG_STICK_NEUTRAL;
  mOutputState.rightStickY = ANALOG_STICK_NEUTRAL;
}
