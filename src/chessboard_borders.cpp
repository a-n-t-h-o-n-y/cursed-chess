#include "chessboard_borders.hpp"

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/size_policy.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

using namespace cppurses;

Row_listing::Row_listing() : Text_display{"⁸⁷⁶⁵⁴³²¹"} {
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(1);
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(8);
    this->brush.set_foreground(Color::Dark_blue);
    this->brush.set_background(Color::White);
    this->disable_word_wrap();
}

Column_listing::Column_listing() : Text_display{"  ᵃ  ᵇ  ᶜ  ᵈ  ᵉ  ᶠ  ᵍ  ʰ"} {
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(26);
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);
    this->brush.set_foreground(Color::Dark_blue);
    this->brush.set_background(Color::White);
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
