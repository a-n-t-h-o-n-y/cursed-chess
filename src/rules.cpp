#include "rules.hpp"
#include "move.hpp"
#include "state.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace {

Side side_at(const State& state, Position position) {
    return state.at(position).side;
}

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

// Piece Rules - - - - - - -
// BISHOP - - - - - - -
typename Rules::Positions Rules::get_bishop_moves(const State& state,
                                                  Position position) const {
    Positions valid_moves;
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
typename Rules::Positions Rules::get_king_moves(const State& state,
                                                Position position) const {
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
typename Rules::Positions Rules::get_knight_moves(const State& state,
                                                  Position position) const {
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
typename Rules::Positions Rules::get_pawn_moves(const State& state,
                                                Position position) const {
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
typename Rules::Positions Rules::get_queen_moves(const State& state,
                                                 Position position) const {
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
typename Rules::Positions Rules::get_rook_moves(const State& state,
                                                Position position) const {
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
