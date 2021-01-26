#ifndef PINOUT_GCCPCB2_H_YGFBJHAA
#define PINOUT_GCCPCB2_H_YGFBJHAA

namespace pinout {
enum pinout {
  SWITCH = 11, // USB Multiplexer

  GCC_DATA = 17,

  L = 2,
  LEFT = 10,
  DOWN = 13,
  RIGHT = 5,
  MODX = 7,
  MODY = 3,

  SELECT = 9,
  START = 6,
  HOME = 8,

  CLEFT = A1,
  CUP = A3,
  CDOWN = A0,
  A = A2,
  CRIGHT = A4,

  B = A5,
  X = 14,
  Z = 16,
  UP = 15,

  R = 12,
  Y = 4,
  LIGHTSHIELD = 1,
  MIDSHIELD = 0,
};
} // namespace pinout

#endif /* end of include guard: PINOUT_GCCPCB2_H_YGFBJHAA */
