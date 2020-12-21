#ifndef MOVE_HPP
#define MOVE_HPP
#include <string>

#include "position.hpp"

namespace chess {

struct Move {
    Position from;
    Position to;
};

std::string move_to_text(const Move& m);

}  // namespace chess
#endif  // MOVE_HPP
