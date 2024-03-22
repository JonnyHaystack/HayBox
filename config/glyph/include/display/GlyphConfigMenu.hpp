#ifndef _DISPLAY_GLYPHCONFIGMENU_HPP
#define _DISPLAY_GLYPHCONFIGMENU_HPP

#include "display/ConfigMenu.hpp"

class GlyphConfigMenu : public ConfigMenu {
  public:
    GlyphConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count);
};

#endif