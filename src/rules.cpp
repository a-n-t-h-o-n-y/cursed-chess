#include "rules.hpp"
#include "move.hpp"
#include "state.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace {

Position current_king_pos(const State& state) {
    Piece king{Figure::King, state.current_side()};
    Rules::Positions king_positions{state.find_positions(king)};
    if (king_positions.empty()) {
        return Position{-1, -1};
    }
    return king_positions.at(0);
}

}  // namespace

bool Rules::validate(const State& state, const Move& move) const {
    if (!is_valid(move.from) || !is_valid(move.to)) {
        return false;
    }
    auto positions = get_valid_positions(state, move.from);
    auto pos = std::find(std::begin(positions), std::end(positions), move.to);
    return pos != std::end(positions);
}

typename Rules::Positions Rules::get_valid_positions(const State& state,
                                                     Position position) const {
    Piece piece{state.at(position)};
    switch (piece.figure) {
        case Figure::Bishop:
            return get_bishop_moves(state, position);

        case Figure::King:
            return get_king_moves(state, position);

        case Figure::Knight:
            return get_knight_moves(state, position);

        case Figure::Pawn:
            return get_pawn_moves(state, position);

        case Figure::Queen:
            return get_queen_moves(state, position);

        case Figure::Rook:
            return get_rook_moves(state, position);

        case Figure::None:
            return std::vector<Position>{};
    }
}

bool Rules::check(const State& state) const {
    Position king_pos{current_king_pos(state)};
    if (king_pos == Position{-1, -1}) {
        return false;
    }
    Piece opponent_piece{Figure::None, opponent(state.current_side())};
    Positions opponent_pieces{state.find_positions(opponent_piece)};
    for (Position p : opponent_pieces) {
        Positions valid_positions{get_valid_positions(state, p)};
        auto pos = std::find(std::begin(valid_positions),
                             std::end(valid_positions), king_pos);
        if (pos != std::end(valid_positions)) {
            return true;
        }
    }
    return false;
}

bool Rules::checkmate(const State& state) const {
    if (!this->check(state)) {
        return false;
    }
    Position king_pos{current_king_pos(state)};
    Positions king_moves{this->get_valid_positions(state, king_pos)};
    if (king_moves.empty()) {
        return true;
    }
    return false;
}

bool Rules::stalemate(const State& state) const {
    return false;
}

// Helper Functions
Side opponent(Side side) {
    switch (side) {
        case Side::White:
            return Side::Black;

        case Side::Black:
            return Side::White;

        default:
            return Side::None;
    }
}

bool is_valid(Position p) {
    bool row_valid{p.row >= 1 && p.row <= 8};
    bool column_valid{p.column >= 1 && p.column <= 8};
    return row_valid && column_valid;
}
