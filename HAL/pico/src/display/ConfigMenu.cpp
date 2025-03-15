#include "display/ConfigMenu.hpp"

#include "core/Persistence.hpp"
#include "core/mode_selection.hpp"
#include "reboot.hpp"
#include "util/config_util.hpp"

ConfigMenu::ConfigMenu(Config &config, CommunicationBackend **backends, size_t backends_count)
    : _config(config),
      _backends(backends),
      _backends_count(backends_count) {}

DisplayModeId ConfigMenu::GetId() {
    return DISPLAY_MODE_CONFIG;
}

void ConfigMenu::HandleControls(
    IntegratedDisplay *instance,
    const DisplayControls &controls,
    Button button
) {
    if (_current_menu_page == nullptr) {
        _current_menu_page = _top_level_page;
        _current_menu_offset = 0;
        return;
    }

    if (button == controls.up) {
        _highlighted_menu_item = max(0, _highlighted_menu_item - 1);
    } else if (button == controls.down) {
        _highlighted_menu_item =
            min(_current_menu_page->items_count - 1, _highlighted_menu_item + 1);
    } else if (button == controls.enter) {
        // Bounds check.
        if (_highlighted_menu_item > _current_menu_page->items_count) {
            _highlighted_menu_item = 0;
            return;
        }

        const MenuPage::MenuItem &selected_item = _current_menu_page->items[_highlighted_menu_item];

        // If there's an action defined, perform it.
        if (selected_item.action != nullptr) {
            selected_item.action(instance, this, _config, selected_item.key);

            // Go back up a level.
            if (_current_menu_page->parent != nullptr) {
                _current_menu_page = _current_menu_page->parent;
                _current_menu_offset = 0;
            }
            _highlighted_menu_item = 0;
            return;
        }

        // If there's a child page defined, drill into it.
        if (selected_item.page != nullptr) {
            _current_menu_page = selected_item.page;
            _current_menu_offset = 0;
            _highlighted_menu_item = 0;
            return;
        }
    } else if (button == controls.back) {
        // If at top-level page, go back to input viewer.
        if (_current_menu_page->parent == nullptr) {
            // Restore gamemode.
            if (_backends[0] != nullptr) {
                _backends[0]->SetGameMode(instance->CurrentGameMode());
            }
            ReturnToDashboard(instance);
            return;
        }

        _current_menu_page = _current_menu_page->parent;
        _highlighted_menu_item = 0;
    }
}

void ConfigMenu::UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) {
    // Unset gamemode to prevent menu button presses being sent to console.
    if (_backends[0] != nullptr && _backends[0]->CurrentGameMode() != nullptr) {
        instance->SetGameMode(_backends[0]->CurrentGameMode());
        _backends[0]->SetGameMode(nullptr);
    }

    uint8_t font_width = instance->font_width;
    uint8_t font_height = instance->font_height;

    if (_highlighted_menu_item - _current_menu_offset > max_visible_lines - 1) {
        _current_menu_offset++;
    } else if (_highlighted_menu_item < _current_menu_offset) {
        _current_menu_offset--;
    }
    uint8_t last_item_to_display =
        min(_current_menu_page->items_count - _current_menu_offset, max_visible_lines + 1);
    for (size_t i = 0; i < last_item_to_display; i++) {
        if (i + _current_menu_offset == _highlighted_menu_item) {
            display.setCursor(0, i * (font_height + padding));
            display.print(highlight_string);
        }
        display.setCursor(font_width + padding, i * (font_height + padding));
        display.print(_current_menu_page->items[i + _current_menu_offset].text);
    }
}

void ConfigMenu::ReturnToDashboard(IntegratedDisplay *instance) {
    instance->SetDisplayMode(DISPLAY_MODE_VIEWER);
}