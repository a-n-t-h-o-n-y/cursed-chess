#include "rules.hpp"
#include "move.hpp"
#include "state.hpp"

namespace {

Color side_at(const State& state, const Position& position) {
    return state.at(position).side;
}

Color opponent(Color side) {
    switch (side) {
        case Color::White:
            return Color::Black;

        case Color::Black:
            return Color::White;

        default:
            return Color::None;
    }

    bool is_valid(const Position& p) {
        bool row_valid{p.row >= 1 && p.row <= 9};
        bool column_valid{p.column >= 1 && p.column <= 9};
        return row_valid && column_valid;
    }
}

}  // namespace

bool Rules::validate(const State& state, const Move& move) {
    auto positions = get_valid_positions(state, move.from);
    auto pos = std::find(std::begin(positions), std::end(positions), move.to);
    return pos != std::end(positions);
}

typename Rules::Positions Rules::get_valid_positions(const State& state,
                                                     const Position& position) {
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
    }
    return std::vector<Position>{};
}

// Piece Rules - - - - - - -
// BISHOP - - - - - - -
typename Rules::Positions Rules::get_bishop_moves(const State& state,
                                                  const Position& position) {
    Positions valid_moves;
    Color bishop_side{side_at(state, position)};

    // Up-Left
    Position next{position};
    ++next.row;
    --next.column;
    Color next_side{side_at(state, next)};
    while (is_valid(next) && next_side != bishop_side) {
        valid_moves.push_back(next);
        if (next_side == opponent(bishop_side)) {
            break;
        }
        ++next.row;
        --next.column;
        next_side = side_at(state, next);
    }
    // Up-Right
    next = position;
    ++next.row;
    ++next.column;
    next_side = side_at(state, next);
    while (is_valid(next) && next_side != bishop_side) {
        valid_moves.push_back(next);
        if (next_side == opponent(bishop_side)) {
            break;
        }
        ++next.row;
        ++next.column;
        next_side = side_at(state, next);
    }
    // Down-Left
    next = position;
    --next.row;
    --next.column;
    next_side = side_at(state, next);
    while (is_valid(next) > 0 && next_side != bishop_side) {
        valid_moves.push_back(next);
        if (next_side == opponent(bishop_side)) {
            break;
        }
        --next.row;
        --next.column;
        next_side = side_at(state, next);
    }
    // Down-Right
    next = position;
    --next.row;
    ++next.column;
    next_side = side_at(state, next);
    while (is_valid(next) < 9 && next_side != bishop_side) {
        valid_moves.push_back(next);
        if (next_side == opponent(bishop_side)) {
            break;
        }
        --next.row;
        ++next.column;
        next_side = side_at(state, next);
    }
    return valid_moves;
}

// KING - - - - - - -
typename Rules::Positions Rules::get_king_moves(const State& state,
                                                const Position& position) {
    Positions valid_moves;
    Color king_side{side_at(state, position)};
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
                                                  const Position& position) {
    Positions valid_moves;
    Color knight_side{side_at(state, position)};
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
                                                const Position& position) {
    Positions valid_moves;
    Color pawn_side{side_at(state, position)};
    int direction{pawn_side == Color::Black ? -1 : 1};
    Position next{position};

    // First Move - Row can move 2 spaces.
    if ((pawn_side == Color::White && position.row == 2) ||
        pawn_side == Color::Black && position.row == 7) {
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
                                                 const Position& position) {
    Positions valid_moves;
    Color queen_side{side_at(state, position)};
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
    next = pos;
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
    next = pos;
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
    next = pos;
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
    next = pos;
    ++next.row;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        ++next.row;
    }
    // Down
    next = pos;
    --next.row;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.row;
    }
    // Left
    next = pos;
    --next.column;
    while (is_valid(next) && side_at(state, next) != queen_side) {
        valid_moves.push_back(next);
        if (side_at(state, next) == opponent(queen_side)) {
            break;
        }
        --next.column;
    }
    // Right
    next = pos;
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
                                                const Position& position) {
    Positions valid_moves;
    Color rook_side{side_at(state, position)};
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
