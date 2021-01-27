#include "ControllerMode.h"
#include "CommunicationBackend.h"
#include "InputMode.h"
#include "socd.h"
#include "state.h"

ControllerMode::ControllerMode(socd::SocdType socdType,
                               state::InputState &rInputState,
                               CommunicationBackend *communicationBackend)
    : InputMode(socdType, rInputState) {
  mpCommunicationBackend = communicationBackend;

  // Set up initial state.
  ResetOutputs();
  ResetVectors();
}

void ControllerMode::ResetOutputs() {
  mOutputState = {
      // Digital outputs.
      .a = false,
      .b = false,
      .x = false,
      .y = false,
      .buttonL = false,
      .buttonR = false,
      .triggerLDigital = false,
      .triggerRDigital = false,
      .start = false,
      .select = false,
      .home = false,
      .dpadUp = false,
      .dpadDown = false,
      .dpadLeft = false,
      .dpadRight = false,
      .leftStickClick = false,
      .rightStickClick = false,

      // Analog outputs.
      .leftStickX = 0,
      .leftStickY = 0,
      .rightStickX = 0,
      .rightStickY = 0,
      .triggerRAnalog = 0,
      .triggerLAnalog = 0,
  };
}

void ControllerMode::UpdateOutputs() {
  ResetOutputs();
  HandleSocd();
  UpdateDigitalOutputs();
  UpdateAnalogOutputs(); // Handle modifier logic.

  // Send outputs via communication backend.
  mpCommunicationBackend->SendOutputs(mOutputState);
}

void ControllerMode::ResetVectors() {
  mVectorState = {
      .horizontal = false,
      .vertical = false,
      .diagonal = false,
      .directionX = 0,
      .directionY = 0,
      .directionCX = 0,
      .directionCY = 0,
  };
}

void ControllerMode::HandleVectors(bool lsLeft, bool lsRight, bool lsDown,
                                   bool lsUp, bool rsLeft, bool rsRight,
                                   bool rsDown, bool rsUp,
                                   uint8_t analogStickMin,
                                   uint8_t analogStickNeutral,
                                   uint8_t analogStickMax) {
  ResetVectors();

  mOutputState.leftStickX = analogStickNeutral;
  mOutputState.leftStickY = analogStickNeutral;
  mOutputState.rightStickX = analogStickNeutral;
  mOutputState.rightStickY = analogStickNeutral;

  // Coordinate calculations to make modifier handling simpler.
  if (lsLeft || lsRight) {
    mVectorState.horizontal = true;
    if (lsLeft) {
      mVectorState.directionX = -1;
      mOutputState.leftStickX = analogStickMin;
    } else {
      mVectorState.directionX = 1;
      mOutputState.leftStickX = analogStickMax;
    }
  }
  if (lsDown || lsUp) {
    mVectorState.vertical = true;
    if (lsDown) {
      mVectorState.directionY = -1;
      mOutputState.leftStickY = analogStickMin;
    } else {
      mVectorState.directionY = 1;
      mOutputState.leftStickY = analogStickMax;
    }
  }
  if (mVectorState.horizontal && mVectorState.vertical)
    mVectorState.diagonal = true;

  if (rsLeft || rsRight) {
    if (rsLeft) {
      mVectorState.directionCX = -1;
      mOutputState.rightStickX = analogStickMin;
    } else {
      mVectorState.directionCX = 1;
      mOutputState.rightStickX = analogStickMax;
    }
  }
  if (rsDown || rsUp) {
    if (rsDown) {
      mVectorState.directionCY = -1;
      mOutputState.rightStickY = analogStickMin;
    } else {
      mVectorState.directionCY = 1;
      mOutputState.rightStickY = analogStickMax;
    }
  }
}
