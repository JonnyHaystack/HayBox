#ifndef _COMMS_INTEGRATEDDISPLAY_HPP
#define _COMMS_INTEGRATEDDISPLAY_HPP

#include "core/CommunicationBackend.hpp"
#include "core/Persistence.hpp"
#include "core/config_utils.hpp"
#include "core/mode_selection.hpp"
#include "reboot.hpp"

#include <Adafruit_GFX.h>
#include <Wire.h>
#include <config.pb.h>

typedef enum _DisplayMode {
    DISPLAY_MODE_VIEWER,
    DISPLAY_MODE_CONFIG,
    DISPLAY_MODE_RGB_BRIGHTNESS,
} DisplayMode;

typedef struct _DisplayControls {
    Button back;
    Button down;
    Button up;
    Button enter;
} DisplayControls;

typedef struct _InputViewerButton {
    Button button;
    uint8_t center_x;
    uint8_t center_y;
    uint8_t radius;
} InputViewerButton;

class IntegratedDisplay;

typedef struct _MenuPage {
    typedef struct _MenuItem {
        char text[20];
        uint8_t key = 0;
        void (*action)(IntegratedDisplay *instance, Config &config, uint8_t key) = nullptr;
        struct _MenuPage *page = nullptr;
    } MenuItem;

    struct _MenuPage *parent = nullptr;
    const MenuItem *items = nullptr;
    size_t items_count = 0;
} MenuPage;

class IntegratedDisplay : public CommunicationBackend {
  public:
    IntegratedDisplay(
        InputState &inputs,
        Adafruit_GFX &display,
        void (*clear_display)(),
        void (*update_display)(),
        DisplayControls controls,
        const InputViewerButton *input_viewer_buttons,
        const size_t input_viewer_buttons_count,
        Config &config,
        CommunicationBackendId backend_id,
        CommunicationBackend **backends,
        size_t backends_count
    );
    ~IntegratedDisplay();
    virtual void UpdateOutputs();
    virtual void SetGameMode(ControllerMode *gamemode);
    virtual void SendReport();

  protected:
    static constexpr uint8_t controls_count = 4;
    static constexpr uint8_t padding = 2;
    static constexpr uint32_t button_cooldown_ms = 150;
    static constexpr uint8_t font_width = 6;
    static constexpr uint8_t font_height = 8;
    static constexpr uint8_t max_visible_lines = 6;
    static constexpr char highlight_string[] = ">";

    Adafruit_GFX &_display;
    void (*_clear_display)();
    void (*_update_display)();
    const DisplayControls _controls;
    const Button _controls_array[controls_count];
    const InputViewerButton *_input_viewer_buttons;
    const size_t _input_viewer_buttons_count;
    Config &_config;
    const CommunicationBackendId _backend_id;
    DisplayMode _display_mode = DISPLAY_MODE_VIEWER;
    char _gamemode_text[16] = "";

    absolute_time_t _button_cooldown_end = 0;
    MenuPage *_current_menu_page = &_top_level_page;
    uint8_t _highlighted_menu_item = 0;
    int _current_menu_offset = 0;

  private:
    MenuPage _usb_backends_page;
    MenuPage _gamemode_options_page;
    MenuPage _top_level_page;
    CommunicationBackend **_backends;
    size_t _backends_count;

    static void SetDefaultMode(
        IntegratedDisplay *instance,
        Config &config,
        uint8_t mode_config_index
    );
    static void SetDefaultUsbBackend(
        IntegratedDisplay *instance,
        Config &config,
        uint8_t backend_config_index
    );
    static void SetSocdType(IntegratedDisplay *instance, Config &config, uint8_t socd_type);

    void HandleControlsViewerMode(Button button);
    void HandleControlsConfigMode(Button button);
    void HandleControlsRgbBrightnessMode(Button button);
};

#endif