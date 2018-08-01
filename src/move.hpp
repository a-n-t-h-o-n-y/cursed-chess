#ifndef MOVE_HPP
#define MOVE_HPP
#include <string>

#include "position.hpp"

struct Move {
    Position from;
    Position to;
};

std::string move_to_text(const Move& m);

#endif  // MOVE_HPP
