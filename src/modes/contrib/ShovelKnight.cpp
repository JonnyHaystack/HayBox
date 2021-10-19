#include "ShovelKnight.h"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

ShovelKnight::ShovelKnight(socd::SocdType socdType,
                           state::InputState &rInputState,
                           CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.mod_x});
  mSocdPairs.push_back(
      socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
}

void ShovelKnight::UpdateDigitalOutputs() {
  mOutputState.dpadLeft = mrInputState.left;
  mOutputState.dpadRight = mrInputState.right;
  mOutputState.dpadDown = mrInputState.down;
  mOutputState.dpadUp = mrInputState.mod_x;

  mOutputState.b = mrInputState.x;       // Jump
  mOutputState.a = mrInputState.a;       // Attack
  mOutputState.y = mrInputState.b;       // Attack
  mOutputState.x = mrInputState.z;       // Subweapon
  mOutputState.buttonL = mrInputState.r; // Subweapon prev
  mOutputState.buttonR = mrInputState.y; // Subweapon next

  mOutputState.select = mrInputState.lightshield; // Inventory
  mOutputState.start = mrInputState.start;        // Pause
}

void ShovelKnight::UpdateAnalogOutputs() {
  // Coordinate calculations to make modifier handling simpler.
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.mod_x, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);
}
