#ifndef CHESSBOARD_BORDERS_HPP
#define CHESSBOARD_BORDERS_HPP
#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/tuple.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include "chessboard_widget.hpp"

namespace chess {

struct Column_label : cppurses::Text_display {
    Column_label() : Text_display{L"  ᵃ  ᵇ  ᶜ  ᵈ  ᵉ  ᶠ  ᵍ  ʰ"}
    {
        using namespace cppurses::pipe;
        using cppurses::Color;
        *this | fixed_width(26) | fixed_height(1) | bg(Color::White) |
            fg(Color::Dark_blue);
    }
};

struct Row_label : cppurses::Text_display {
    Row_label() : Text_display{L"⁸⁷⁶⁵⁴³²¹"}
    {
        using namespace cppurses::pipe;
        using cppurses::Color;
        *this | fixed_width(1) | fixed_height(8) | bg(Color::White) |
            fg(Color::Dark_blue);
    }
};

using Board_middle = cppurses::Tuple<cppurses::layout::Horizontal<>,
                                     Row_label,
                                     Chessboard_widget,
                                     Row_label>;

class Bordered_chessboard : public cppurses::Tuple<cppurses::layout::Vertical<>,
                                                   Column_label,
                                                   Board_middle,
                                                   Column_label> {
   public:
    Chessboard_widget& chessboard = this->get<1>().get<1>();
};

}  // namespace chess
#endif  // CHESSBOARD_BORDERS_HPP
