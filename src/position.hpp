#ifndef BOARD_POSITION_HPP
#define BOARD_POSITION_HPP
#include <string>

struct Position {
    int row;     // Bottom to Top[1,8]
    int column;  // Left to Right[1,8]
};

std::string to_text(const Position& pos);

bool operator==(const Position& p1, const Position& p2);

#endif  // BOARD_POSITION_HPP
