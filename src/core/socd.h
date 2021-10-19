#ifndef SOCD_H_FQXOHPZ4
#define SOCD_H_FQXOHPZ4

#include <stdbool.h>
#include <stdint.h>

namespace socd {

typedef enum {
  SOCD_NEUTRAL,
  SOCD_2IP,
  SOCD_2IP_NO_REAC,
  SOCD_KEYBOARD,
} SocdType;

typedef struct {
  bool *button_low;
  bool *button_high;
} SocdPair;

typedef struct {
  bool was_low;
  bool was_high;
  bool lock_low;
  bool lock_high;
} SocdState;

SocdState fTwoIPNoReactivate(bool &isLOW, bool &isHIGH, SocdState socdState);

SocdState fTwoIP(bool &isLOW, bool &isHIGH, SocdState socdState);

void fNeutral(bool &isLOW, bool &isHIGH);

} // namespace socd

#endif /* end of include guard: SOCD_H_FQXOHPZ4 */
