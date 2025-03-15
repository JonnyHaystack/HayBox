#ifndef _COMMS_INTEGRATEDDISPLAY_HPP
#define _COMMS_INTEGRATEDDISPLAY_HPP

#include "core/CommunicationBackend.hpp"
#include "display/DisplayMode.hpp"

#include <Adafruit_GFX.h>
#include <config.pb.h>

typedef struct _DisplayControls {
    Button back;
    Button down;
    Button up;
    Button enter;
} DisplayControls;

class IntegratedDisplay : public CommunicationBackend {
  public:
    static constexpr uint8_t controls_count = 4;
    static constexpr uint32_t button_cooldown_ms = 150;
    static constexpr uint8_t font_width = 6;
    static constexpr uint8_t font_height = 8;
    static constexpr uint8_t default_color = 1;

    IntegratedDisplay(
        InputState &inputs,
        Adafruit_GFX &display,
        void (*clear_display)(),
        void (*update_display)(),
        const DisplayControls controls,
        DisplayMode **display_modes,
        size_t display_modes_count
    );
    ~IntegratedDisplay();
    virtual void SendReport();
    void SetDisplayMode(DisplayModeId display_mode);

  protected:
    Adafruit_GFX &_display;
    void (*_clear_display)();
    void (*_update_display)();
    const DisplayControls _controls;
    const Button _controls_array[controls_count];
    DisplayModeId _display_mode = DISPLAY_MODE_VIEWER;
    absolute_time_t _button_cooldown_end = 0;

  private:
    DisplayMode **_display_modes;
    size_t _display_modes_count;

    void HandleControls(DisplayMode *active_mode);

    /**
     * @brief Get the current active DisplayMode instance.
     *
     * @return A pointer to the first DisplayMode instance in the _display_modes array whose
     * DisplayModeId is the same as the current value of _display_mode in this class.
     */
    DisplayMode *GetActiveDisplayMode();
};

#endif