#ifndef _DISPLAY_INPUTDISPLAY_HPP
#define _DISPLAY_INPUTDISPLAY_HPP

#include "display/DisplayMode.hpp"

typedef struct _InputViewerButton {
    Button button;
    uint8_t center_x;
    uint8_t center_y;
    uint8_t radius;
} InputViewerButton;

class InputDisplay : public DisplayMode {
  public:
    InputDisplay(
        InputViewerButton *input_viewer_buttons,
        size_t input_viewer_buttons_count,
        CommunicationBackendId backend_id
    );
    DisplayModeId GetId();
    void HandleControls(
        IntegratedDisplay *instance,
        const DisplayControls &controls,
        Button button
    );
    void UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display);

  protected:
    InputViewerButton *_input_viewer_buttons;
    size_t _input_viewer_buttons_count;
    CommunicationBackendId _backend_id;
};

#endif