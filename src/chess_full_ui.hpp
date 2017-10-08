#ifndef CHESS_FULL_UI_HPP
#define CHESS_FULL_UI_HPP
#include "chessboard_borders.hpp"
#include <cppurses/cppurses.hpp>

using namespace cppurses;

// struct Side_pane : public Vertical_layout {
//     Log_widget& display_info;
//     Push_button& settings_btn;
// }

class Chess_full_UI : public Horizontal_layout {
   public:
       Chess_full_UI();

   private:
    Widget_stack& stack{this->make_child<Widget_stack>()};
    Chessboard_with_borders& board{stack.make_page<Chessboard_with_borders>()};
    // Settings_pane& board{stack.make_page<Settings_pane>()};

    Side_pane& side_pane{this->make_child<Side_pane>()};
};

#endif  // CHESS_FULL_UI_HPP
