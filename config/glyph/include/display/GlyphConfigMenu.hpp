#ifndef _DISPLAY_GLYPHCONFIGMENU_HPP
#define _DISPLAY_GLYPHCONFIGMENU_HPP

#include "display/ConfigMenu.hpp"

/**
 * @brief Modified version of ConfigMenu that only shows gamemodes marked as applicable for current
 * CommunicationBackend.
 */
class GlyphConfigMenu : public ConfigMenu {
  public:
    GlyphConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count);
};

#endif