#ifndef RULES_HPP
#define RULES_HPP
#include "position.hpp"

#include <vector>

class State;
struct Move;

class Rules {
   public:
    using Positions = std::vector<Position>;

    bool validate(const State& state, const Move& move) const;
    Positions get_valid_positions(const State& state, Position position) const;

    bool check(const State& state) const;
    bool checkmate(const State& state) const;
    bool stalemate(const State& state) const;

   private:
    Positions get_bishop_moves(const State& state, Position position) const;
    Positions get_king_moves(const State& state, Position position) const;
    Positions get_knight_moves(const State& state, Position position) const;
    Positions get_pawn_moves(const State& state, Position position) const;
    Positions get_queen_moves(const State& state, Position position) const;
    Positions get_rook_moves(const State& state, Position position) const;
};

#endif  // RULES_HPP
