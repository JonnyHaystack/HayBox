#include "src/Nintendo/src/Nintendo.h"

#include "GamecubeBackend.h"

GamecubeBackend::GamecubeBackend(int pollingRate, int gccDataPin)
    : CommunicationBackend() {
  mpGamecubeConsole = new CGamecubeConsole(gccDataPin);
  mGamecubeData = defaultGamecubeData;

  // Delay used between input updates to postpone them until right before the
  // next poll, while also leaving time (850us) for processing to finish.
  mDelay = (1000000 / pollingRate) - 850;
}

GamecubeBackend::~GamecubeBackend() { delete mpGamecubeConsole; }

void GamecubeBackend::SendOutputs(state::OutputState outputState) {
  // Digital outputs
  mGamecubeData.report.a = outputState.a;
  mGamecubeData.report.b = outputState.b;
  mGamecubeData.report.x = outputState.x;
  mGamecubeData.report.y = outputState.y;
  mGamecubeData.report.z = outputState.buttonR;
  mGamecubeData.report.l = outputState.triggerLDigital;
  mGamecubeData.report.r = outputState.triggerRDigital;
  mGamecubeData.report.start = outputState.start;
  mGamecubeData.report.dleft = outputState.dpadLeft;
  mGamecubeData.report.dright = outputState.dpadRight;
  mGamecubeData.report.ddown = outputState.dpadDown;
  mGamecubeData.report.dup = outputState.dpadUp;

  // Analog outputs
  mGamecubeData.report.xAxis = outputState.leftStickX;
  mGamecubeData.report.yAxis = outputState.leftStickY;
  mGamecubeData.report.cxAxis = outputState.rightStickX;
  mGamecubeData.report.cyAxis = outputState.rightStickY;
  mGamecubeData.report.left = outputState.triggerLAnalog + 31;
  mGamecubeData.report.right = outputState.triggerRAnalog + 31;

  // Send outputs to console.
  mpGamecubeConsole->write(mGamecubeData);

  delayMicroseconds(mDelay);
}
