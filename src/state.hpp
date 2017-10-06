#ifndef STATE_HPP
#define STATE_HPP
#include "piece.hpp"
#include "position.hpp"

#include <utility>
#include <vector>

class State {
   public:
    Color current_player_;
    std::vector<std::pair<Piece, Position>> piece_positions_;
    Piece at(Position p) const; // if position is out of bounds
};

#endif  // STATE_HPP
