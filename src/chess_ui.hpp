#ifndef CHESS_UI_HPP
#define CHESS_UI_HPP
#include <cppurses/widget/detail/link_lifetimes.hpp>
#include <cppurses/widget/layouts/float.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/stack.hpp>
#include <cppurses/widget/layouts/vertical.hpp>

#include <signals_light/signal.hpp>

#include "bordered_chessboard.hpp"
#include "lower_pane.hpp"
#include "settings_pane.hpp"

namespace chess {

class Left_side : public cppurses::layout::Vertical<> {
   private:
    class Board_and_settings
        : public cppurses::layout::Stack<cppurses::layout::Vertical<>> {
       public:
        Board_and_settings();

       public:
        Bordered_chessboard& board = make_page<Bordered_chessboard>();
        Settings_pane& settings    = make_page<Settings_pane>();
    };

   public:
    Board_and_settings& board_and_settings = make_child<Board_and_settings>();
    Lower_pane& lower_pane                 = make_child<Lower_pane>();

   public:
    Left_side();
};

using Chess_UI =
    cppurses::Float<cppurses::Float<Left_side, cppurses::layout::Horizontal>,
                    cppurses::layout::Vertical>;

}  // namespace chess
#endif  // CHESS_UI_HPP
