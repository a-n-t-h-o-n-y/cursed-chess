#include "state.hpp"

#include <mutex>
#include <unordered_map>

#include "piece.hpp"
#include "position.hpp"

namespace {
using namespace chess;
typename Board_state::Pieces_t initial_state{
    {{2, 1}, {Figure::Pawn, Side::White}},
    {{2, 2}, {Figure::Pawn, Side::White}},
    {{2, 3}, {Figure::Pawn, Side::White}},
    {{2, 4}, {Figure::Pawn, Side::White}},
    {{2, 5}, {Figure::Pawn, Side::White}},
    {{2, 6}, {Figure::Pawn, Side::White}},
    {{2, 7}, {Figure::Pawn, Side::White}},
    {{2, 8}, {Figure::Pawn, Side::White}},
    {{1, 1}, {Figure::Rook, Side::White}},
    {{1, 2}, {Figure::Knight, Side::White}},
    {{1, 3}, {Figure::Bishop, Side::White}},
    {{1, 4}, {Figure::Queen, Side::White}},
    {{1, 5}, {Figure::King, Side::White}},
    {{1, 6}, {Figure::Bishop, Side::White}},
    {{1, 7}, {Figure::Knight, Side::White}},
    {{1, 8}, {Figure::Rook, Side::White}},
    {{7, 1}, {Figure::Pawn, Side::Black}},
    {{7, 2}, {Figure::Pawn, Side::Black}},
    {{7, 3}, {Figure::Pawn, Side::Black}},
    {{7, 4}, {Figure::Pawn, Side::Black}},
    {{7, 5}, {Figure::Pawn, Side::Black}},
    {{7, 6}, {Figure::Pawn, Side::Black}},
    {{7, 7}, {Figure::Pawn, Side::Black}},
    {{7, 8}, {Figure::Pawn, Side::Black}},
    {{8, 1}, {Figure::Rook, Side::Black}},
    {{8, 2}, {Figure::Knight, Side::Black}},
    {{8, 3}, {Figure::Bishop, Side::Black}},
    {{8, 4}, {Figure::Queen, Side::Black}},
    {{8, 5}, {Figure::King, Side::Black}},
    {{8, 6}, {Figure::Bishop, Side::Black}},
    {{8, 7}, {Figure::Knight, Side::Black}},
    {{8, 8}, {Figure::Rook, Side::Black}}};

}  // namespace

namespace chess {

Piece Board_state::at(Position p) const {
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    return this->pieces.at(p);
}

bool Board_state::has_piece_at(Position p) const {
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    return this->pieces.count(p) == 1;
}

Board_state::Positions_t Board_state::find_all(Figure fig) const {
    Positions_t results;
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    for (const auto& pos_piece : this->pieces) {
        if (pos_piece.second.figure == fig) {
            results.push_back(pos_piece.first);
        }
    }
    return results;
}

Board_state::Positions_t Board_state::find_all(Side side) const {
    Positions_t results;
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    for (const auto& pos_piece : this->pieces) {
        if (pos_piece.second.side == side) {
            results.push_back(pos_piece.first);
        }
    }
    return results;
}

Board_state::Positions_t Board_state::find_all(Figure fig, Side side) const {
    Positions_t results;
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    for (const auto& pos_piece : this->pieces) {
        if (pos_piece.second.figure == fig && pos_piece.second.side == side) {
            results.push_back(pos_piece.first);
        }
    }
    return results;
}

State::State() {
    this->reset();
}

void State::reset() {
    current_side = Side::White;
    board.pieces = initial_state;
    this->board_reset();
    if (game_over) {
        // TODO restart chess event loop
    }
    game_over = false;
}

}  // namespace chess
