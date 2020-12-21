#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <functional>
#include <string>

#include "move.hpp"

namespace chess {
class Chess_engine;

/// A Player is a move generator.
// TODO maybe rename it to Move_generator?
struct Player {
    std::string name;
    std::function<Move(Chess_engine const&)> get_move;
};

}  // namespace chess
#endif  // PLAYER_HPP
