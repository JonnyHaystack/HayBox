#ifndef _DISPLAY_GLYPHCONFIGMENU_HPP
#define _DISPLAY_GLYPHCONFIGMENU_HPP

#include "display/DefaultConfigMenu.hpp"

/**
 * @brief Modified version of ConfigMenu that only shows gamemodes marked as applicable for current
 * CommunicationBackend.
 */
class GlyphConfigMenu : public DefaultConfigMenu {
  public:
    GlyphConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count);
    void ReturnToDashboard(IntegratedDisplay *instance);

  private:
    static void SetDashboardOption(
        IntegratedDisplay *instance,
        ConfigMenu *menu,
        Config &config,
        uint8_t mode_config_index
    );
};

#endif