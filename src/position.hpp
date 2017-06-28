#ifndef BOARD_POSITION_HPP
#define BOARD_POSITION_HPP

#include <sstream>

struct Position {
    int row;
    int column;
};

std::string to_text(const Position& pos) {
    std::stringstream ss;
    ss << static_cast<char>(pos.column - 1 + 'a') << pos.row;
    return ss.str();
}

bool operator==(Position p1, Position p2) {
    return p1.row == p2.row && p1.column == p2.column;
}

#endif  // BOARD_POSITION_HPP
