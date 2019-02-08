#include "settings_pane.hpp"

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/border.hpp>
#include <cppurses/widget/widget_free_functions.hpp>

using namespace cppurses;

Settings_pane::Settings_pane() {
    this->set_name("Settings_pane");
    this->border.enabled = true;
    this->border.north_east = L'─';
    this->border.north_west = L'╭';
    this->border.south_west = L'╰';
    this->border.south_enabled = false;
    this->border.east_enabled = false;
    this->border.south_east_enabled = false;
    set_background_recursive(*this, Color::White);
    set_foreground_recursive(*this, Color::Blue);

    show_moves_box.set_name("show_move_box in Settings_pane");
    hide_log_box.set_name("hide_log_box in Settings_pane");
    black_ai.set_name("black_ai in Settings_pane");
    white_ai.set_name("white_ai in Settings_pane");
    ruleset.set_name("ruleset in Settings_pane");
    reset_btn.set_name("reset_btn in Settings_pane");
    return_btn.set_name("return_btn in Settings_pane");

    // Return Button
    return_btn.set_alignment(Alignment::Center);
    set_background(return_btn, Color::Blue);
    set_foreground(return_btn, Color::White);
}
