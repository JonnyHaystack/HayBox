#ifndef STATE_H_9YPYM7WM
#define STATE_H_9YPYM7WM

#include <stdbool.h>
#include <stdint.h>

namespace state {

// Button state.
typedef struct {
  bool start;
  bool select;
  bool home;
  bool y;
  bool x;
  bool b;
  bool a;
  bool l;
  bool r;
  bool z;
  bool lightshield;
  bool midshield;
  bool up;
  bool down;
  bool right;
  bool left;
  bool mod_x;
  bool mod_y;
  bool c_left;
  bool c_right;
  bool c_up;
  bool c_down;
  bool e_1;
  bool e_2;
  bool e_3;
  bool e_4;
  bool e_5;
  bool e_6;
} InputState;

// State describing stick direction at the quadrant level.
typedef struct {
  bool horizontal;
  bool vertical;
  bool diagonal;
  int8_t directionX;
  int8_t directionY;
  int8_t directionCX;
  int8_t directionCY;
} VectorState;

// Output state.
typedef struct {
  // Digital outputs.
  bool a;
  bool b;
  bool x;
  bool y;
  bool buttonL;
  bool buttonR;
  bool triggerLDigital;
  bool triggerRDigital;
  bool start;
  bool select;
  bool home;
  bool dpadUp;
  bool dpadDown;
  bool dpadLeft;
  bool dpadRight;
  bool leftStickClick;
  bool rightStickClick;

  // Analog outputs.
  uint8_t leftStickX;
  uint8_t leftStickY;
  uint8_t rightStickX;
  uint8_t rightStickY;
  uint8_t triggerRAnalog;
  uint8_t triggerLAnalog;
} OutputState;

} // namespace state

#endif /* end of include guard: STATE_H_9YPYM7WM */
