#include "FgcMode.h"
#include "ControllerMode.h"

FgcMode::FgcMode(socd::SocdType socdType, state::InputState &rInputState,
                 CommunicationBackend *communicationBackend)
    : ControllerMode(socdType, rInputState, communicationBackend) {}

void FgcMode::UpdateDigitalOutputs() {
  // Directions
  mOutputState.dpadLeft = mrInputState.left;
  mOutputState.dpadRight = mrInputState.right;
  mOutputState.dpadDown = mrInputState.down;
  mOutputState.dpadUp = mrInputState.mod_x;

  // Menu keys
  mOutputState.start = mrInputState.start;
  mOutputState.select = mrInputState.c_left;
  mOutputState.home = mrInputState.c_down;

  // Right hand bottom row
  mOutputState.a = mrInputState.b;
  mOutputState.b = mrInputState.x;
  mOutputState.triggerRDigital = mrInputState.z;
  mOutputState.triggerLDigital = mrInputState.up;

  // Right hand top row
  mOutputState.x = mrInputState.r;
  mOutputState.y = mrInputState.y;
  mOutputState.buttonR = mrInputState.lightshield;
  mOutputState.buttonL = mrInputState.midshield;
}

void FgcMode::UpdateAnalogOutputs() {
  mOutputState.leftStickX = 128;
  mOutputState.leftStickY = 128;
  mOutputState.rightStickX = 128;
  mOutputState.rightStickY = 128;
}
