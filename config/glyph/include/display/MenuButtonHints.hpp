#ifndef _DISPLAY_MENUBUTTONHINTS_HPP
#define _DISPLAY_MENUBUTTONHINTS_HPP

#include "display/DisplayMode.hpp"

class MenuButtonHints : public DisplayMode {
  public:
    MenuButtonHints(CommunicationBackendId backend_id);
    DisplayModeId GetId();
    void HandleControls(
        IntegratedDisplay *instance,
        const DisplayControls &controls,
        Button button
    );
    void UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display);

  protected:
    CommunicationBackendId _backend_id;
};

#endif