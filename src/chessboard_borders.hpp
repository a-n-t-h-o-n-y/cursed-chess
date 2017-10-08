#ifndef CHESSBOARD_BORDERS_HPP
#define CHESSBOARD_BORDERS_HPP
#include "chessboard_widget.hpp"
#include <cppurses/cppurses.hpp>

class Row_listing : public Text_display {
   public:
    Row_listing();
};

class Column_listing : public Text_display {
   public:
    Column_listing();
};

struct Chessboard_with_borders : public Vertical_layout {
    Chessboard_with_borders();

    Column_listing& top_column{this->make_child<Column_listing>()};
    Horizontal_layout& middle_layout{this->make_child<Horizontal_layout>()};
    Row_listing& left_row{middle_layout.make_child<Row_listing>()};
    Chessboard_widget& chessboard{
        middle_layout.make_child<Chessboard_widget>()};
    Row_listing& right_row{middle_layout.make_child<Row_listing>()};
    Column_listing& bottom_column{this->make_child<Column_listing>()};
};

#endif  // CHESSBOARD_BORDERS_HPP
