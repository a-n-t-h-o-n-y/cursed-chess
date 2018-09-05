#include "chess_engine.hpp"

#include <mutex>
#include <unordered_map>

#include "figure.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "rules.hpp"
#include "side.hpp"
#include "state.hpp"

using namespace chess;

const State& Chess_engine::state() const {
    return state_;
}

chess::State& Chess_engine::state() {
    return state_;
}

Player* Chess_engine::player_black() const {
    return player_black_.get();
}

Player* Chess_engine::player_white() const {
    return player_white_.get();
}

bool Chess_engine::make_move(Move move) {
    if (rules_->checkmate(state_)) {
        state_.game_over = true;
        return true;
    }
    if (rules_->validate(state_, move)) {
        std::lock_guard<std::recursive_mutex> lock{state_.board.mtx};
        auto iter = state_.board.pieces.find(move.from);
        if (iter != std::end(state_.board.pieces)) {
            state_.board.pieces[move.to] = iter->second;
            state_.board.pieces.erase(move.from);
        }
        this->toggle_current_side();
        this->move_made(move);
        if (rules_->checkmate(state_)) {
            checkmate(state_.current_side);
            state_.game_over = true;
        } else if (rules_->check(state_)) {
            check(state_.current_side);
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
    std::lock_guard<std::recursive_mutex> lock{this->mtx};
    return rules_->get_valid_positions(state_, position);
}

void Chess_engine::toggle_current_side() {
    state_.current_side = opponent(state_.current_side);
}
