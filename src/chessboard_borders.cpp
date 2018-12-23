#include "chessboard_borders.hpp"

#include <cppurses/cppurses.hpp>

Row_listing::Row_listing() : Text_display{"⁸⁷⁶⁵⁴³²¹"} {
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(1);
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(8);
    set_foreground(*this, Color::Dark_blue);
    set_background(*this, Color::White);
    this->disable_word_wrap();
}

Column_listing::Column_listing() : Text_display{"  ᵃ  ᵇ  ᶜ  ᵈ  ᵉ  ᶠ  ᵍ  ʰ"} {
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(26);
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);
    set_foreground(*this, Color::Dark_blue);
    set_background(*this, Color::White);
    this->disable_word_wrap();
}

Chessboard_with_borders::Chessboard_with_borders() {
    top_column.set_name("top_column in Chessboard_with_borders");
    middle_layout.set_name("middle_layout in Chessboard_with_borders");
    left_row.set_name("left_row in Chessboard_with_borders");
    chessboard.set_name("chessboard in Chessboard_with_borders");
    right_row.set_name("right_row in Chessboard_with_borders");
    bottom_column.set_name("bottom_column in Chessboard_with_borders");

    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(26);

    middle_layout.height_policy.type(Size_policy::Fixed);
    middle_layout.height_policy.hint(8);
}
