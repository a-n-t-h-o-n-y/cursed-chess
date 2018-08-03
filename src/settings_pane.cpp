#include "settings_pane.hpp"

#include <cppurses/cppurses.hpp>

Settings_pane::Settings_pane() {
    enable_border(*this);
    this->border.north_west = L'╭';
    this->border.south_west = L'╰';
    this->border.south_enabled = false;
    this->border.east_enabled = false;
    this->border.south_east_enabled = false;
    this->border.north_east_enabled = false;
    set_background_recursive(*this, Color::White);
    set_foreground_recursive(*this, Color::Blue);

    // Return Button
    return_btn.set_alignment(Alignment::Center);
    set_background(return_btn, Color::Blue);
    set_foreground(return_btn, Color::White);
}
