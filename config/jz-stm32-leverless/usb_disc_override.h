#pragma once

#ifndef __ASSEMBLER__
#include <board/board.h>
// PC12 belongs to B22, so USB initialization must not drive it.
#undef BOARD_USB_DISC_DEV
#define BOARD_USB_DISC_DEV NULL
#endif
