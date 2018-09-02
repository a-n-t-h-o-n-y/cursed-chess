#include "no_rules.hpp"

#include "side.hpp"
#include "state.hpp"

using namespace chess;

namespace {

// Side side_at(const chess::State& state, Position position) {
//     return state.at(position).side;
// }

typename Rules::Positions empty_or_opponent_positions(const chess::State& state,
                                                      Position position) {
    typename Rules::Positions valid_moves;
    // Side piece_side{side_at(state, position)};
    Side piece_side{state.board.at(position).side};
    for (int row{1}; row < 9; ++row) {
        for (int column{1}; column < 9; ++column) {
            if (!state.board.has_piece_at(Position{row, column})) {
                valid_moves.push_back(Position{row, column});
            } else if (state.board.at(Position{row, column}).side !=
                       piece_side) {
                valid_moves.push_back(Position{row, column});
            }
        }
    }
    return valid_moves;
}  // namespace

}  // namespace

// Piece Rules - - - - - - -
typename Rules::Positions No_rules::get_bishop_moves(const chess::State& state,
                                                     Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_king_moves(const chess::State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_knight_moves(const chess::State& state,
                                                     Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_pawn_moves(const chess::State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_queen_moves(const chess::State& state,
                                                    Position position) const {
    return empty_or_opponent_positions(state, position);
}

typename Rules::Positions No_rules::get_rook_moves(const chess::State& state,
                                                   Position position) const {
    return empty_or_opponent_positions(state, position);
}
