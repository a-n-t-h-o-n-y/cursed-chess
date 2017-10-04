#ifndef CHESS_GAME_WIDGET_HPP
#define CHESS_GAME_WIDGET_HPP
#include <cppurses/cppurses.hpp>

using namespace cppurses;

struct Side_pane : public Vertical_layout {
    Log_widget& display_info;
    Push_button& settings_btn;
}

class Chess_game_widget : public Horizontal_layout {
   public:
   private:
    Widget_stack& stack{this->make_child<Widget_stack>()};
    Chessboard_widget& board{this->make_child<Chessboard_widget>()};
    Settings_pane& settings{this->make_child<Settings_pane>()};

    Side_pane& side_pane{this->make_child<Side_pane>()};
};

#endif  // CHESS_GAME_WIDGET_HPP
