#ifndef RULES_HPP
#define RULES_HPP
#include <vector>

#include "piece.hpp"
#include "position.hpp"

class State;
struct Move;

class Rules {
   public:
    using Positions = std::vector<Position>;

    virtual ~Rules() = default;

    bool validate(const State& state, const Move& move) const;
    Positions get_valid_positions(const State& state, Position position) const;

    bool check(const State& state) const;
    bool checkmate(const State& state) const;
    bool stalemate(const State& state) const;

   protected:
    virtual Positions get_bishop_moves(const State& state,
                                       Position position) const = 0;
    virtual Positions get_king_moves(const State& state,
                                     Position position) const = 0;
    virtual Positions get_knight_moves(const State& state,
                                       Position position) const = 0;
    virtual Positions get_pawn_moves(const State& state,
                                     Position position) const = 0;
    virtual Positions get_queen_moves(const State& state,
                                      Position position) const = 0;
    virtual Positions get_rook_moves(const State& state,
                                     Position position) const = 0;
};

// Helper Functions
Side opponent(Side side);
bool is_valid(Position p);

#endif  // RULES_HPP
