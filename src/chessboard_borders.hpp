#ifndef CHESSBOARD_BORDERS_HPP
#define CHESSBOARD_BORDERS_HPP
#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include "chessboard_widget.hpp"

class Chessboard_with_borders : public cppurses::layout::Vertical<> {
    struct Column_labels : cppurses::Text_display {
        Column_labels() : Text_display{"  ᵃ  ᵇ  ᶜ  ᵈ  ᵉ  ᶠ  ᵍ  ʰ"}
        {
            this->width_policy.fixed(26);
            this->height_policy.fixed(1);
            this->brush.set_foreground(cppurses::Color::Dark_blue);
            this->brush.set_background(cppurses::Color::White);
            this->disable_word_wrap();
        }
    };

    struct Board_middle : cppurses::layout::Horizontal<> {
        struct Row_labels : cppurses::Text_display {
            Row_labels() : Text_display{"⁸⁷⁶⁵⁴³²¹"}
            {
                this->width_policy.fixed(1);
                this->height_policy.fixed(8);
                this->brush.set_foreground(cppurses::Color::Dark_blue);
                this->brush.set_background(cppurses::Color::White);
                this->disable_word_wrap();
            }
        };

        Row_labels& left_labels{this->make_child<Row_labels>()};
        Chessboard_widget& chessboard{this->make_child<Chessboard_widget>()};
        Row_labels& right_labels{this->make_child<Row_labels>()};
    };

   public:
    Chessboard_with_borders()
    {
        this->width_policy.fixed(26);
        middle.height_policy.fixed(8);
    }

    Column_labels& top_labels{this->make_child<Column_labels>()};
    Board_middle& middle{this->make_child<Board_middle>()};
    Column_labels& bottom_labels{this->make_child<Column_labels>()};
};
#endif  // CHESSBOARD_BORDERS_HPP
