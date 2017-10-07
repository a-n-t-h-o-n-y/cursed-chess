#ifndef RULES_HPP
#define RULES_HPP
#include "position.hpp"

#include <vector>

class State;
struct Move;

class Rules {
   public:
    using Positions = std::vector<Position>;

    bool validate(const State& state, const Move& move);
    Positions get_valid_positions(const State& state, const Position& position);

   private:
    Positions get_bishop_moves(const State& state, const Position& position);
    Positions get_king_moves(const State& state, const Position& position);
    Positions get_knight_moves(const State& state, const Position& position);
    Positions get_pawn_moves(const State& state, const Position& position);
    Positions get_queen_moves(const State& state, const Position& position);
    Positions get_rook_moves(const State& state, const Position& position);
};

#endif  // RULES_HPP
