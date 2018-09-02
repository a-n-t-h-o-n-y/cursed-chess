#include "standard_rules.hpp"

#include "side.hpp"
#include "state.hpp"

using namespace chess;

namespace {

Side side_at(const chess::State& state, Position position) {
    if (!state.board.has_piece_at(position)) {
        return Side::None;
    }
    return state.board.at(position).side;
}

}  // namespace

// Piece Rules - - - - - - -
// BISHOP - - - - - - -
typename Rules::Positions Standard_rules::get_bishop_moves(
    const chess::State& state,
    Position position) const {
    Positions valid_moves;
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Side bishop_side{side_at(state, position)};

    // Up-Left
    Position next{position};
    ++next.row;
    --next.column;
    while (is_valid(next) && side_at(state, next) != bishop_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(bishop_side)) {
            break;
        }
        ++next.row;
        --next.column;
    }
    // Up-Right
    next = position;
    ++next.row;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != bishop_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(bishop_side)) {
            break;
        }
        ++next.row;
        ++next.column;
    }
    // Down-Left
    next = position;
    --next.row;
    --next.column;
    while (is_valid(next) && side_at(state, next) != bishop_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(bishop_side)) {
            break;
        }
        --next.row;
        --next.column;
    }
    // Down-Right
    next = position;
    --next.row;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != bishop_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(bishop_side)) {
            break;
        }
        --next.row;
        ++next.column;
    }
    return valid_moves;
}

// KING - - - - - - -
typename Rules::Positions Standard_rules::get_king_moves(
    const chess::State& state,
    Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Positions valid_moves;
    Side king_side{side_at(state, position)};
    Position next = position;

    // Up
    ++next.row;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Down
    next = position;
    --next.row;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Left
    next = position;
    --next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Right
    next = position;
    ++next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Top Left
    next = position;
    ++next.row;
    --next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Top Right
    next = position;
    ++next.row;
    ++next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Bottom Left
    next = position;
    --next.row;
    --next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    // Bottom Right
    next = position;
    --next.row;
    ++next.column;
    if (is_valid(next) && side_at(state, next) != king_side) {
        valid_moves.push_back(next);
    }
    return valid_moves;
}

// KNIGHT - - - - - - -
typename Rules::Positions Standard_rules::get_knight_moves(
    const chess::State& state,
    Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Positions valid_moves;
    Side knight_side{side_at(state, position)};
    Position next = position;

    // Top Left
    next.row += 2;
    next.column -= 1;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }
    next = position;
    next.row += 1;
    next.column -= 2;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }

    // Top Right
    next = position;
    next.row += 2;
    next.column += 1;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }
    next = position;
    next.row += 1;
    next.column += 2;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }

    // Bottom Left
    next = position;
    next.row -= 1;
    next.column -= 2;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }
    next = position;
    next.row -= 2;
    next.column -= 1;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }

    // Bottom Right
    next = position;
    next.row -= 1;
    next.column += 2;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }
    next = position;
    next.row -= 2;
    next.column += 1;
    if (is_valid(next) && side_at(state, next) != knight_side) {
        valid_moves.push_back(next);
    }
    return valid_moves;
}

// PAWN - - - - - - -
typename Rules::Positions Standard_rules::get_pawn_moves(
    const chess::State& state,
    Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Positions valid_moves;
    Side pawn_side{side_at(state, position)};
    int direction{pawn_side == Side::Black ? -1 : 1};
    Position next{position};

    // First Move - Row can move 2 spaces.
    Position one_ahead{position.row + direction, position.column};
    bool blocked = (one_ahead.row <= 8 && one_ahead.row >= 1) &&
                   (side_at(state, one_ahead) != Side::None);
    if (!blocked && ((pawn_side == Side::White && position.row == 2) ||
                     (pawn_side == Side::Black && position.row == 7))) {
        next.row += 2 * direction;
        if (side_at(state, next) == Side::None) {
            valid_moves.push_back(next);
        }
    }

    // Normal Move - Forward one space.
    next = position;
    next.row += 1 * direction;
    if (is_valid(next) && side_at(state, next) == Side::None) {
        valid_moves.push_back(next);
    }

    // Capture Opponent - Move diagonally.
    next = position;
    next.row += 1 * direction;
    next.column -= 1;
    if (is_valid(next) && side_at(state, next) == opponent(pawn_side)) {
        valid_moves.push_back(next);
    }

    // Capture Opponent - Move diagonally.
    next = position;
    next.row += 1 * direction;
    next.column += 1;
    if (is_valid(next) && side_at(state, next) == opponent(pawn_side)) {
        valid_moves.push_back(next);
    }
    return valid_moves;
}

// QUEEN - - - - - - -
typename Rules::Positions Standard_rules::get_queen_moves(
    const chess::State& state,
    Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Positions valid_moves;
    Side queen_side{side_at(state, position)};
    Position next{position};

    // Up-Left
    next = position;
    ++next.row;
    --next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        ++next.row;
        --next.column;
    }
    // Up-Right
    next = position;
    ++next.row;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        ++next.row;
        ++next.column;
    }
    // Down-Left
    next = position;
    --next.row;
    --next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.row;
        --next.column;
    }
    // Down-Right
    next = position;
    --next.row;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.row;
        ++next.column;
    }
    // Up
    next = position;
    ++next.row;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        ++next.row;
    }
    // Down
    next = position;
    --next.row;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.row;
    }
    // Left
    next = position;
    --next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.column;
    }
    // Right
    next = position;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        ++next.column;
    }
    return valid_moves;
}

// ROOK - - - - - - -
typename Rules::Positions Standard_rules::get_rook_moves(
    const chess::State& state,
    Position position) const {
    if (!state.board.has_piece_at(position)) {
        return std::vector<Position>{};
    }
    Positions valid_moves;
    Side rook_side{side_at(state, position)};
    Position next{position};

    // Up
    ++next.row;
    while (is_valid(next) && side_at(state, next) != rook_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(rook_side)) {
            break;
        }
        ++next.row;
    }
    // Down
    next = position;
    --next.row;
    while (is_valid(next) && side_at(state, next) != rook_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(rook_side)) {
            break;
        }
        --next.row;
    }
    // Left
    next = position;
    --next.column;
    while (is_valid(next) && side_at(state, next) != rook_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(rook_side)) {
            break;
        }
        --next.column;
    }
    // Right
    next = position;
    ++next.column;
    while (is_valid(next) && side_at(state, next) != rook_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(rook_side)) {
            break;
        }
        ++next.column;
    }
    return valid_moves;
}
