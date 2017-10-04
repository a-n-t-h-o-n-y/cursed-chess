#ifndef BOARD_POSITION_HPP
#define BOARD_POSITION_HPP
#include <string>

struct Position {
    int row;
    int column;
};

std::string to_text(const Position& pos);
bool operator==(Position p1, Position p2);

#endif  // BOARD_POSITION_HPP
