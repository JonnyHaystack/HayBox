#include "display/DefaultConfigMenu.hpp"

#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "reboot.hpp"
#include "util/config_util.hpp"

DefaultConfigMenu::DefaultConfigMenu(
    Config &config,
    CommunicationBackend **backends,
    size_t backends_count
)
    : ConfigMenu(config, backends, backends_count) {
    /* Build default USB backends page */
    MenuPage::MenuItem *usb_backend_options =
        new MenuPage::MenuItem[config.communication_backend_configs_count];

    size_t usb_backend_options_count = 0;
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        CommunicationBackendConfig &backend_config = config.communication_backend_configs[i];
        MenuPage::MenuItem &current_option = usb_backend_options[usb_backend_options_count];

        if (backend_config.backend_id != COMMS_BACKEND_XINPUT &&
            backend_config.backend_id != COMMS_BACKEND_DINPUT &&
            backend_config.backend_id != COMMS_BACKEND_NINTENDO_SWITCH &&
            backend_config.backend_id != COMMS_BACKEND_CONFIGURATOR) {
            continue;
        }

        strlcpy(
            current_option.text,
            backend_name(backend_config.backend_id),
            sizeof(current_option.text)
        );
        current_option.key = i;
        current_option.action = &SetUsbBackend;
        usb_backend_options_count++;
    }

    _usb_backends_page = {
        .items = usb_backend_options,
        .items_count = usb_backend_options_count,
    };

    /* Build gamemodes page */
    MenuPage::MenuItem *gamemode_options = new MenuPage::MenuItem[config.game_mode_configs_count];

    size_t gamemode_options_count = 0;
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        MenuPage::MenuItem &current_option = gamemode_options[gamemode_options_count];

        // Don't show keyboard modes as a gamemode option unless using DInputBackend.
        if (_backends_count > 0 && _backends[0] != nullptr) {
            CommunicationBackendId primary_backend_id = _backends[0]->BackendId();
            if (primary_backend_id != COMMS_BACKEND_DINPUT &&
                mode_config.mode_id == MODE_KEYBOARD) {
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

    _gamemode_options_page = {
        .items = gamemode_options,
        .items_count = gamemode_options_count,
    };

    /* Build SOCD types page */
    static MenuPage::MenuItem socd_options[_SocdType_MAX] = {};
    for (uint8_t socd_type = SOCD_NEUTRAL; socd_type < _SocdType_ARRAYSIZE; socd_type++) {
        MenuPage::MenuItem &current_option = socd_options[socd_type - 1];
        strlcpy(current_option.text, socd_name((SocdType)socd_type), sizeof(current_option.text));
        current_option.key = socd_type;
        current_option.action = &SetSocdType;
    }

    static MenuPage socd_page = {
        .items = socd_options,
        .items_count = sizeof(socd_options) / sizeof(MenuPage::MenuItem),
    };

    /* Build top-level page */
    // clang-format off
    static MenuPage::MenuItem top_level_items[] = {
        {
            .text = "Profile",
            .page = &_gamemode_options_page,
        },
        {
            .text = "USB Mode",
            .page = &_usb_backends_page,
        },
        {
            .text = "SOCD Mode",
            .page = &socd_page,
        },
        {
            .text = "RGB Brightness",
            .action = [](
                IntegratedDisplay *display_backend,
                ConfigMenu *menu,
                Config &config,
                uint8_t key
            ) {
                display_backend->SetDisplayMode(DISPLAY_MODE_RGB_BRIGHTNESS);
            },
        },
        {
            .text = "Input Viewer",
            .action = [](
                IntegratedDisplay *display_backend,
                ConfigMenu *menu,
                Config &config,
                uint8_t key
            ) {
                // Restore gamemode.
                DefaultConfigMenu *config_menu = (DefaultConfigMenu*)menu; 
                if (config_menu->_backends[0] != nullptr) {
                    config_menu->_backends[0]->SetGameMode(display_backend->CurrentGameMode());
                }
                display_backend->SetDisplayMode(DISPLAY_MODE_VIEWER);
            },
        },
        {
            .text = "Save changes",
            .action = [](
                IntegratedDisplay *display_backend,
                ConfigMenu *menu,
                Config &config,
                uint8_t key
            ) {
                persistence.SaveConfig(config);
                reboot_firmware();
            },
        },
        {
            .text = "Discard changes",
            .action = [](
                IntegratedDisplay *display_backend,
                ConfigMenu *menu,
                Config &config,
                uint8_t key
            ) {
                reboot_firmware();
            },
        },
        {
            .text = "Firmware update",
            .action = [](
                IntegratedDisplay *display_backend,
                ConfigMenu *menu,
                Config &config,
                uint8_t key
            ) {
                reboot_bootloader();
            }
        }
    };
    // clang-format on

    static MenuPage top_level_page = {
        .items = top_level_items,
        .items_count = sizeof(top_level_items) / sizeof(MenuPage::MenuItem),
    };
    _top_level_page = &top_level_page;

    _usb_backends_page.parent = _top_level_page;
    _gamemode_options_page.parent = _top_level_page;
    socd_page.parent = _top_level_page;

    // Set initial page.
    _current_menu_page = _top_level_page;
}

DefaultConfigMenu::~DefaultConfigMenu() {
    delete[] _usb_backends_page.items;
    delete[] _gamemode_options_page.items;
}

void DefaultConfigMenu::SetDefaultMode(
    IntegratedDisplay *display_backend,
    ConfigMenu *menu,
    Config &config,
    uint8_t mode_config_index
) {
    if (mode_config_index < 0 || mode_config_index >= config.game_mode_configs_count) {
        return;
    }

    // Overwrite default game mode for all backend configs.
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        config.communication_backend_configs[i].default_mode_config = mode_config_index + 1;
    }

    // Update mode for all backends.
    DefaultConfigMenu *config_menu = (DefaultConfigMenu *)menu;
    for (size_t i = 0; i < config_menu->_backends_count; i++) {
        set_mode(config_menu->_backends[i], config.game_mode_configs[mode_config_index], config);
    }
    set_mode(display_backend, config.game_mode_configs[mode_config_index], config);
}

void DefaultConfigMenu::SetUsbBackend(
    IntegratedDisplay *display_backend,
    ConfigMenu *menu,
    Config &config,
    uint8_t backend_config_index
) {
    if (backend_config_index < 0 ||
        backend_config_index >= config.communication_backend_configs_count) {
        return;
    }
    // Set backend in watchdog SCRATCH0 register and reboot.
    watchdog_hw->scratch[0] = backend_config_index + 1;
    reboot_firmware();
}

void DefaultConfigMenu::SetSocdType(
    IntegratedDisplay *display_backend,
    ConfigMenu *menu,
    Config &config,
    uint8_t socd_type
) {
    if (socd_type <= SOCD_UNSPECIFIED || socd_type > _SocdType_MAX) {
        return;
    }

    // Overwrite SOCD type for all SOCD pairs of current gamemode's config.
    GameModeConfig *mode_config = display_backend->CurrentGameMode()->GetConfig();
    if (mode_config != nullptr) {
        for (size_t i = 0; i < mode_config->socd_pairs_count; i++) {
            mode_config->socd_pairs[i].socd_type = (SocdType)socd_type;
        }
    }
}