#include "no_rules.hpp"

#include "state.hpp"

namespace {

Side side_at(const State& state, Position position) {
    return state.at(position).side;
}

typename Rules::Positions empty_or_opponent_positions(const State& state,
                                                      Position position) {
    typename Rules::Positions valid_moves;
    Side piece_side{side_at(state, position)};
    for (int row{1}; row < 9; ++row) {
        for (int column{1}; column < 9; ++column) {
            if (side_at(state, Position{row, column}) != piece_side) {
                valid_moves.push_back(Position{row, column});
            }
        }
    }
    return valid_moves;
}

}  // namespace

// Piece Rules - - - - - - -
typename Rules::Positions No_rules::get_bishop_moves(const State& state,
                                                     Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_king_moves(const State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_knight_moves(const State& state,
                                                     Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_pawn_moves(const State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_queen_moves(const State& state,
                                                    Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_rook_moves(const State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}
