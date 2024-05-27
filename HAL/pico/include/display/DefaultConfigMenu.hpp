#ifndef _DISPLAY_DEFAULTCONFIGMENU_HPP
#define _DISPLAY_DEFAULTCONFIGMENU_HPP

#include "display/ConfigMenu.hpp"

class DefaultConfigMenu : public ConfigMenu {
  public:
    DefaultConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count);
    ~DefaultConfigMenu();

  protected:
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

    MenuPage _usb_backends_page;
    MenuPage _gamemode_options_page;

  private:
    static constexpr uint8_t padding = 2;
    static constexpr uint8_t max_visible_lines = 6;
    static constexpr char highlight_string[] = ">";
};

#endif