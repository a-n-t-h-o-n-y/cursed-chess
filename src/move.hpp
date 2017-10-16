#ifndef MOVE_HPP
#define MOVE_HPP
#include "position.hpp"

#include <string>

struct Move {
    Position from;
    Position to; 
};

std::string move_to_text(const Move& m);

#endif // MOVE_HPP
