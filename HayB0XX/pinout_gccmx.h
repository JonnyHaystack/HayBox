#ifndef PINOUT_GCCMX_H_L9PBO40K
#define PINOUT_GCCMX_H_L9PBO40K

namespace pinout {
enum pinout {
  SWITCH = 11, // USB Multiplexer

  GCC_DATA = 17,

  L = 15,
  LEFT = 16,
  DOWN = 14,
  RIGHT = 3,
  MODX = 2,
  MODY = 0,

  SELECT = 1,
  START = 4,
  HOME = 12,

  CLEFT = 8,
  CUP = 10,
  CDOWN = 6,
  A = 9,
  CRIGHT = 5,

  B = A2,
  X = A1,
  Z = A0,
  UP = 13,

  R = 7,
  Y = A5,
  LIGHTSHIELD = A4,
  MIDSHIELD = A3,
};
} // namespace pinout

#endif /* end of include guard: PINOUT_GCCMX_H_L9PBO40K */
