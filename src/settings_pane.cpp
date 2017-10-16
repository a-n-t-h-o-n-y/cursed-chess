#include "settings_pane.hpp"

#include <cppurses/cppurses.hpp>

Settings_pane::Settings_pane() {
    enable_border(*this);
    // once you fix border overprinting you can get rid of noth_east and just
    // disable noth_east
    this->border.north_east = "─";
    this->border.north_west = "╭";
    // this->border.south_east = "╜";
    this->border.south_west = "╰";
    // this->border.west = "║";
    // this->border.east = "║";
    // this->border.north_enabled = false;
    this->border.south_enabled = false;
    this->border.east_enabled = false;
    this->border.south_east_enabled = false;
    // this->border.north_east_enabled = false;
    set_background_recursive(*this, Color::White);
    set_foreground_recursive(*this, Color::Blue);

    // Title
    // title_.set_alignment(Alignment::Center);
    // set_background(title_, Color::Blue);
    // set_foreground(title_, Color::Dark_blue);

    // space_1_.background_tile = "▓";
    // space_2_.background_tile = "▒";
    // space_3_.background_tile = "░";
    // set_background(space_1_, Color::Light_blue);
    // set_background(space_2_, Color::Light_blue);
    // set_background(space_3_, Color::Light_blue);

    // Black AI
    black_ai.add_option("Human");
    black_ai.add_option("Random");
    // set_background(black_ai, Color::Blue);

    // White AI
    white_ai.add_option("Human");
    white_ai.add_option("Random");
    // set_background(white_ai, Color::Blue);

    // Ruleset
    // set_background(ruleset, Color::Light_blue);
    ruleset.add_option("Standard Chess");
    ruleset.add_option("No Rules");

    // Return Button
    return_btn.set_alignment(Alignment::Center);
    set_background(return_btn, Color::Blue);
    set_foreground(return_btn, Color::White);

    // Reset Button
    // set_background(reset_btn, Color::Blue);
    // set_foreground(reset_btn, Color::White);
}
