#include "display/GlyphConfigMenu.hpp"

#include "core/Persistence.hpp"
#include "core/config_utils.hpp"
#include "core/mode_selection.hpp"
#include "reboot.hpp"

GlyphConfigMenu::GlyphConfigMenu(
    Config &config,
    CommunicationBackend **backends,
    size_t backends_count
)
    : ConfigMenu(config, backends, backends_count) {
    /* Build gamemodes page */
    size_t gamemode_options_count = 0;
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        MenuPage::MenuItem &current_option = _gamemode_options_page.items[gamemode_options_count];

        // Only show gamemodes that are applicable to the current comms backend.
        if (mode_config.applicable_backends_count > 0 && _backends_count > 0 &&
            _backends[0] != nullptr) {
            CommunicationBackendId primary_backend_id = _backends[0]->BackendId();
            bool applicable = false;
            for (size_t i = 0; i < mode_config.applicable_backends_count; i++) {
                if (primary_backend_id == mode_config.applicable_backends[i]) {
                    applicable = true;
                    break;
                }
            }
            if (!applicable) {
                continue;
            }
        }

        if (strnlen(mode_config.name, sizeof(mode_config.name)) > 0) {
            strlcpy(current_option.text, mode_config.name, sizeof(current_option.text));
        } else {
            strlcpy(
                current_option.text,
                gamemode_name(mode_config.mode_id),
                sizeof(current_option.text)
            );
        }
        current_option.key = i;
        current_option.action = &SetDefaultMode;
        gamemode_options_count++;
    }
    _gamemode_options_page.items_count = gamemode_options_count;
}