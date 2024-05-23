#ifndef _DISPLAY_DISPLAYMODE_HPP
#define _DISPLAY_DISPLAYMODE_HPP

#include <Adafruit_GFX.h>
#include <config.pb.h>

typedef struct _DisplayControls DisplayControls;
class IntegratedDisplay;

typedef enum _DisplayModeId {
    DISPLAY_MODE_VIEWER,
    DISPLAY_MODE_CONFIG,
    DISPLAY_MODE_RGB_BRIGHTNESS,
    DISPLAY_MODE_BUTTON_HINTS
} DisplayModeId;

class DisplayMode {
  public:
    DisplayMode(){};
    virtual ~DisplayMode(){};

    virtual DisplayModeId GetId() = 0;
    virtual void HandleControls(
        IntegratedDisplay *instance,
        const DisplayControls &controls,
        Button button
    ) = 0;
    virtual void UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) = 0;
};

#endif
