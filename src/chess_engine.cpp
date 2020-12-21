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

namespace chess {

auto Chess_engine::make_move(Move move) -> bool
{
    if (rules_->checkmate(state_)) {
        state_.game_over = true;
        return true;
    }
    if (rules_->validate(state_, move)) {
        auto lock = std::lock_guard<std::recursive_mutex>{state_.board.mtx};
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
        }
        else if (rules_->check(state_))
            check(state_.current_side);
        else if (rules_->stalemate(state_))
            stalemate();
        return true;
    }
    invalid_move(move);
    return false;
}

}  // namespace chess
