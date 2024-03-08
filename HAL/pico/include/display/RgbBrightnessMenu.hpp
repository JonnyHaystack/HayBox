#ifndef _DISPLAY_RGBBRIGHTNESSMENU_HPP
#define _DISPLAY_RGBBRIGHTNESSMENU_HPP

#include "display/DisplayMode.hpp"

class RgbBrightnessMenu : public DisplayMode {
  public:
    RgbBrightnessMenu(Config &config);
    DisplayModeId GetId();
    void HandleControls(
        IntegratedDisplay *instance,
        const DisplayControls &controls,
        Button button
    );
    void UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display);

  private:
    Config &_config;
};

#endif