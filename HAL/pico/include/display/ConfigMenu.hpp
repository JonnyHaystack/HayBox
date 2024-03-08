#ifndef _DISPLAY_CONFIGMENU_HPP
#define _DISPLAY_CONFIGMENU_HPP

#include "comms/IntegratedDisplay.hpp"
#include "display/DisplayMode.hpp"

#include <config.pb.h>

class ConfigMenu;

// clang-format off

typedef struct _MenuPage {
    typedef struct _MenuItem {
        char text[20];
        uint8_t key = 0;
        void (*action)(
            IntegratedDisplay *instance,
            ConfigMenu *menu,
            Config &config,
            uint8_t key
        ) = nullptr;
        struct _MenuPage *page = nullptr;
    } MenuItem;

    struct _MenuPage *parent = nullptr;
    const MenuItem *items = nullptr;
    size_t items_count = 0;
} MenuPage;

// clang-format on

class ConfigMenu : public DisplayMode {
  public:
    ConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count);
    ~ConfigMenu();
    DisplayModeId GetId();
    virtual void HandleControls(
        IntegratedDisplay *instance,
        const DisplayControls &controls,
        Button button
    );
    virtual void UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display);

  private:
    static constexpr uint8_t padding = 2;
    static constexpr uint8_t max_visible_lines = 6;
    static constexpr char highlight_string[] = ">";

    Config &_config;
    CommunicationBackend **_backends;
    size_t _backends_count;

    MenuPage _usb_backends_page;
    MenuPage _gamemode_options_page;
    MenuPage _top_level_page;

    MenuPage *_current_menu_page = &_top_level_page;
    uint8_t _highlighted_menu_item = 0;
    int _current_menu_offset = 0;

    static void SetDefaultMode(
        IntegratedDisplay *instance,
        ConfigMenu *menu,
        Config &config,
        uint8_t mode_config_index
    );
    static void SetDefaultUsbBackend(
        IntegratedDisplay *instance,
        ConfigMenu *menu,
        Config &config,
        uint8_t backend_config_index
    );
    static void SetSocdType(
        IntegratedDisplay *instance,
        ConfigMenu *menu,
        Config &config,
        uint8_t socd_type
    );
};

#endif