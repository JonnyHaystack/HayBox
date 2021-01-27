#include "src/Nintendo/src/Nintendo.h"

#include "N64Backend.h"

N64Backend::N64Backend(int pollingRate, int gccDataPin)
    : CommunicationBackend() {
  mpN64Console = new CN64Console(gccDataPin);
  mN64Data = defaultN64Data;

  // Delay used between input updates to postpone them until right before the
  // next poll, while also leaving time (850us) for processing to finish.
  mDelay = (1000000 / pollingRate) - 850;
}

N64Backend::~N64Backend() { delete mpN64Console; }

void N64Backend::SendOutputs(state::OutputState outputState) {
  // Digital outputs
  mN64Data.report.a = outputState.a;
  mN64Data.report.b = outputState.b;
  mN64Data.report.z = outputState.buttonR;
  mN64Data.report.l = outputState.triggerLDigital;
  mN64Data.report.r = outputState.triggerRDigital;
  mN64Data.report.start = outputState.start;
  mN64Data.report.dleft = outputState.dpadLeft;
  mN64Data.report.dright = outputState.dpadRight;
  mN64Data.report.ddown = outputState.dpadDown;
  mN64Data.report.dup = outputState.dpadUp;
  // Somewhat ugly way of mapping right stick to C-Pad
  mN64Data.report.cleft = outputState.rightStickX < 128;
  mN64Data.report.cright = outputState.rightStickX > 128;
  mN64Data.report.cdown = outputState.rightStickY < 128;
  mN64Data.report.cup = outputState.rightStickY > 128;

  // Analog outputs - converted from unsigned to signed 8-bit integers
  mN64Data.report.xAxis = outputState.leftStickX - 128;
  mN64Data.report.yAxis = outputState.leftStickY - 128;

  // Send outputs to console.
  mpN64Console->write(mN64Data);

  delayMicroseconds(mDelay);
}
