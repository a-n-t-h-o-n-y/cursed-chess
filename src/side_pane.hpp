#ifndef SIDE_PANE_HPP
#define SIDE_PANE_HPP
#include <cppurses/cppurses.hpp>

using namespace cppurses;

struct Move_settings_bar : public Horizontal_layout {
    Move_settings_bar();
    Move_input& move_input{this->make_child<Move_input>()};
    Push_button& settings_btn{this->make_child<Push_button>("Settings")};
};

struct Side_pane : public Vertical_layout {
    Side_pane();
    Log& chess_log{this->make_child<Log>()};
    Move_settings_bar& bottom_bar{this->make_child<Move_settings_bar>()};
};

#endif  // SIDE_PANE_HPP
