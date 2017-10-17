#include "settings_pane.hpp"

#include <cppurses/cppurses.hpp>

Settings_pane::Settings_pane() {
    enable_border(*this);
    // once you fix border overprinting you can get rid of noth_east and just
    // disable noth_east
    this->border.north_east = "─";
    this->border.north_west = "╭";
    this->border.south_west = "╰";
    this->border.south_enabled = false;
    this->border.east_enabled = false;
    this->border.south_east_enabled = false;
    set_background_recursive(*this, Color::White);
    set_foreground_recursive(*this, Color::Blue);

    // Black AI
    black_ai.add_option("Human");
    black_ai.add_option("Random");

    // White AI
    white_ai.add_option("Human");
    white_ai.add_option("Random");

    // Ruleset
    ruleset.add_option("Standard Chess");
    ruleset.add_option("No Rules");

    // Return Button
    return_btn.set_alignment(Alignment::Center);
    set_background(return_btn, Color::Blue);
    set_foreground(return_btn, Color::White);
}
