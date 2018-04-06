#include "chess_engine.hpp"

#include <vector>

#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "rules.hpp"

Chess_engine::Chess_engine() {
    this->reset();
    state_.move_made.connect([this](const Move& m) { this->move_made(m); });
    state_.board_reset.connect([this] { this->board_reset(); });
}

void Chess_engine::reset() {
    state_.reset();
}

bool Chess_engine::make_move(Move move) {
    if (rules_->checkmate(state_)) {
        return true;
    }
    if (rules_->validate(state_, move)) {
        Piece to_piece{this->at(move.to)};
        if (to_piece.side == opponent(this->current_side())) {
            this->capture(to_piece);
        }
        ::make_move(state_, move);
        if (rules_->checkmate(state_)) {
            checkmate(this->current_side());
        } else if (rules_->check(state_)) {
            check(this->current_side());
        } else if (rules_->stalemate(state_)) {
            stalemate();
        }
        return true;
    }
    invalid_move(move);
    return false;
}

Chess_engine::Positions Chess_engine::get_valid_positions(
    Position position) const {
    return rules_->get_valid_positions(state_, position);
}

Piece Chess_engine::at(Position position) const {
    return state_.at(position);
}

Chess_engine::Positions Chess_engine::find_positions(Piece piece) const {
    return state_.find_positions(piece);
}

Side Chess_engine::current_side() const {
    return state_.current_side();
}
