#ifndef PINOUT_H_NWSSB0OQ
#define PINOUT_H_NWSSB0OQ

namespace pinout {
enum pinout {
  SWITCH = 7, // USB Multiplexer

  GCC_DATA = 17,

  L = 9,
  LEFT = 15,
  DOWN = 16,
  RIGHT = 14,
  MODX = 8,
  MODY = 6,

  SELECT = 12,
  START = 12,
  HOME = 12,

  CLEFT = A1,
  CUP = A2,
  CDOWN = 5,
  A = 13,
  CRIGHT = 18,

  B = 4,
  X = A5,
  Z = A4,
  UP = A3,

  R = 0,
  Y = 1,
  LIGHTSHIELD = 10,
  MIDSHIELD = 11,
};
} // namespace pinout

#endif /* end of include guard: PINOUT_H_NWSSB0OQ */
