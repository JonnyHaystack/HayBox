#include "MKWii.h"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

MKWii::MKWii(socd::SocdType socdType, state::InputState &rInputState,
             CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.l, &rInputState.down});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.l, &rInputState.mod_x});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.l, &rInputState.mod_y});
}

void MKWii::UpdateDigitalOutputs() {
  mOutputState.a = mrInputState.b;
  mOutputState.b = mrInputState.x;
  mOutputState.triggerLDigital = mrInputState.z;
  mOutputState.buttonR = mrInputState.up;
  mOutputState.dpadUp = mrInputState.a;
  mOutputState.start = mrInputState.start;
}

void MKWii::UpdateAnalogOutputs() {
  bool up = mrInputState.down || mrInputState.mod_x || mrInputState.mod_y;

  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.l, up,
                false, false, false, false, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);

  if (mrInputState.z) {
    mOutputState.triggerLAnalog = 140;
  }
}
