#include "rules.hpp"

#include <algorithm>
#include <iterator>
#include <mutex>
#include <vector>

#include "figure.hpp"
#include "move.hpp"
#include "side.hpp"
#include "state.hpp"

using namespace chess;

namespace {

Position current_king_pos(const chess::State& state) {
    Rules::Positions king_positions{
        state.board.find_all(Figure::King, state.current_side)};
    if (king_positions.empty()) {
        return Position{-1, -1};
    }
    return king_positions.at(0);
}

}  // namespace

bool Rules::validate(const chess::State& state, const Move& move) const {
    if (!is_valid(move.from) || !is_valid(move.to)) {
        return false;
    }
    auto positions = get_valid_positions(state, move.from);
    auto pos = std::find(std::begin(positions), std::end(positions), move.to);
    return pos != std::end(positions);
}

typename Rules::Positions Rules::get_valid_positions(const chess::State& state,
                                                     Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }

    Piece piece{state.board.at(position)};
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
    }
}

bool Rules::check(const chess::State& state) const {
    Position king_pos{current_king_pos(state)};
    if (king_pos == Position{-1, -1}) {
        return false;
    }
    Positions opponent_pieces{
        state.board.find_all(opponent(state.current_side))};
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

// TODO Does not work.
bool Rules::checkmate(const chess::State& state) const {
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

bool Rules::stalemate(const chess::State& /* state */) const {
    // TODO Implement
    return false;
}

// Helper Functions
/// Return the oponent Side to \p side.
Side opponent(Side side) {
    if (side == Side::None) {
        return Side::None;
    }
    return side == Side::White ? Side::Black : Side::White;
}

/// Validates bounds of a position.
// TODO change name.
bool is_valid(Position p) {
    bool row_valid{p.row >= 1 && p.row <= 8};
    bool column_valid{p.column >= 1 && p.column <= 8};
    return row_valid && column_valid;
}
