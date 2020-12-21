#ifndef NO_RULES_HPP
#define NO_RULES_HPP
#include "position.hpp"
#include "rules.hpp"
#include "state.hpp"

namespace chess {

class No_rules : public Rules {
   protected:
    Positions get_bishop_moves(const chess::State& state,
                               Position position) const override;
    Positions get_king_moves(const chess::State& state,
                             Position position) const override;
    Positions get_knight_moves(const chess::State& state,
                               Position position) const override;
    Positions get_pawn_moves(const chess::State& state,
                             Position position) const override;
    Positions get_queen_moves(const chess::State& state,
                              Position position) const override;
    Positions get_rook_moves(const chess::State& state,
                             Position position) const override;
};

}  // namespace chess
#endif  // NO_RULES_HPP
