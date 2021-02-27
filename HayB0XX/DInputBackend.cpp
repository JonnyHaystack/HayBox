#include "src/ArduinoJoystickLibrary/src/Joystick.h"

#include "CommunicationBackend.h"
#include "DInputBackend.h"
#include "state.h"

DInputBackend::DInputBackend() : CommunicationBackend() {
  mpJoystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                             17, 1, // Button Count, Hat Switch Count
                             true, true, false, // X and Y, but no Z Axis
                             true, true, false, // Rx, Ry, no Rz
                             true, true,       // Rudder, throttle
                             false, false,
                             false); // No accelerator, no brake, no steering

  mpJoystick->begin(false);
  mpJoystick->setXAxisRange(0, 255);
  mpJoystick->setYAxisRange(0, 255);
  mpJoystick->setRxAxisRange(0, 255);
  mpJoystick->setRyAxisRange(0, 255);
  mpJoystick->setThrottleRange(0, 255);
  mpJoystick->setRudderRange(0, 255);
}

DInputBackend::~DInputBackend() {
  mpJoystick->end();
  delete mpJoystick;
}

void DInputBackend::SendOutputs(state::OutputState outputState) {
  // Digital outputs
  mpJoystick->setButton(0, outputState.b);
  mpJoystick->setButton(1, outputState.a);
  mpJoystick->setButton(2, outputState.y);
  mpJoystick->setButton(3, outputState.x);
  mpJoystick->setButton(4, outputState.buttonR);
  mpJoystick->setButton(5, outputState.triggerRDigital);
  mpJoystick->setButton(6, outputState.buttonL);
  mpJoystick->setButton(7, outputState.triggerLDigital);
  mpJoystick->setButton(8, outputState.select);
  mpJoystick->setButton(9, outputState.start);
  mpJoystick->setButton(10, outputState.rightStickClick);
  mpJoystick->setButton(11, outputState.leftStickClick);

  // Analog outputs
  mpJoystick->setXAxis(outputState.leftStickX);
  mpJoystick->setYAxis(256 - outputState.leftStickY);
  mpJoystick->setRxAxis(outputState.rightStickX);
  mpJoystick->setRyAxis(256 - outputState.rightStickY);
  mpJoystick->setThrottle(outputState.triggerLAnalog + 1);
  mpJoystick->setRudder(outputState.triggerRAnalog + 1);

  // D-pad Hat Switch
  mpJoystick->setHatSwitch(
      0, GetDpadAngle(outputState.dpadLeft, outputState.dpadRight,
                      outputState.dpadDown, outputState.dpadUp));

  mpJoystick->sendState();
}

int16_t DInputBackend::GetDpadAngle(bool left, bool right, bool down, bool up) {
  int16_t angle = -1;
  if (right && !left) {
    angle = 90;
    if (down)
      angle = 135;
    if (up)
      angle = 45;
  } else if (left && !right) {
    angle = 270;
    if (down)
      angle = 225;
    if (up)
      angle = 315;
  } else if (down && !up) {
    angle = 180;
  } else if (up && !down) {
    angle = 0;
  }
  return angle;
}
