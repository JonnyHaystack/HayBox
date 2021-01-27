#ifndef DINPUTBACKEND_H_QR073UNQ
#define DINPUTBACKEND_H_QR073UNQ

#include "src/ArduinoJoystickLibrary/src/Joystick.h"

#include "CommunicationBackend.h"

class DInputBackend : public CommunicationBackend {
public:
  DInputBackend();
  ~DInputBackend();
  void SendOutputs(state::OutputState outputState);

private:
  int16_t GetDpadAngle(bool left, bool right, bool down, bool up);
  Joystick_ *mpJoystick;
};

#endif /* end of include guard: DINPUTBACKEND_H_QR073UNQ */
