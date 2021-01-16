#include "DefaultKeyboardMode.h"
#include "KeyboardMode.h"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socdType,
                                         state::InputState &rInputState)
    : KeyboardMode(socdType, rInputState) {}

void DefaultKeyboardMode::SendKeys() {
  Press('a', mrInputState.l);
  Press('b', mrInputState.left);
  Press('c', mrInputState.down);
  Press('d', mrInputState.right);
  Press('e', mrInputState.mod_x);
  Press('f', mrInputState.mod_y);
  Press('g', mrInputState.select);
  Press('h', mrInputState.start);
  Press('i', mrInputState.home);
  Press('j', mrInputState.r);
  Press('k', mrInputState.y);
  Press('l', mrInputState.lightshield);
  Press('m', mrInputState.midshield);
  Press('n', mrInputState.b);
  Press('o', mrInputState.x);
  Press('p', mrInputState.z);
  Press('q', mrInputState.up);
  Press('r', mrInputState.c_up);
  Press('s', mrInputState.c_left);
  Press('t', mrInputState.c_right);
  Press('u', mrInputState.a);
  Press('v', mrInputState.c_down);
}
