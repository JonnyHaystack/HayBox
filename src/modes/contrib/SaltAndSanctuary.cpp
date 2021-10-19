#include "SaltAndSanctuary.h"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

SaltAndSanctuary::SaltAndSanctuary(socd::SocdType socdType,
                                   state::InputState &rInputState,
                                   CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.mod_x});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
}

void SaltAndSanctuary::UpdateDigitalOutputs() {
  mOutputState.dpadRight = mrInputState.l;                 // Block
  mOutputState.b = mrInputState.b;                         // Roll
  mOutputState.a = mrInputState.a;                         // Attack
  mOutputState.y = mrInputState.z;                         // Strong
  mOutputState.dpadDown = mrInputState.mod_y;              // Use
  mOutputState.x = mrInputState.x;                         // Jump

  mOutputState.buttonL = mrInputState.r;                   // Previous item
  mOutputState.buttonR = mrInputState.y;                   // Next item
  mOutputState.triggerLDigital = mrInputState.lightshield; // Use item

  mOutputState.triggerRDigital = mrInputState.midshield;   // Use torch

  mOutputState.dpadLeft = mrInputState.up;                 // Switch loadout

  mOutputState.start = mrInputState.start;                 // Inventory
}

void SaltAndSanctuary::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.mod_x, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);
}
