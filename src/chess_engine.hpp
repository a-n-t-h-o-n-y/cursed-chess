#ifndef CHESS_CHESS_ENGINE_HPP
#define CHESS_CHESS_ENGINE_HPP
#include <memory>
#include <mutex>
#include <vector>

#include <signals_light/signal.hpp>

#include <cppurses/widget/detail/link_lifetimes.hpp>

#include "piece.hpp"
#include "player.hpp"
#include "player_human.hpp"
#include "position.hpp"
#include "rules.hpp"
#include "side.hpp"
#include "standard_rules.hpp"
#include "state.hpp"

namespace chess {

/// Responsible for the organizing the state and logic of the chess game. This
/// includes the current state of the board, the players, and the rules.
class Chess_engine {
   public:
    sl::Signal<void(const Move&)> move_made;
    sl::Signal<void(const Move&)> invalid_move;
    sl::Signal<void(Piece)> capture;
    sl::Signal<void(Side)> checkmate;
    sl::Signal<void(Side)> check;
    sl::Signal<void()> stalemate;

    mutable std::recursive_mutex mtx;

    sl::Lifetime lifetime;

   public:
    using Positions = std::vector<Position>;

    void set_player(Side s, Player p)
    {
        switch (s) {
            case Side::Black: player_black_ = std::move(p); break;
            case Side::White: player_white_ = std::move(p); break;
            case Side::None: break;  // TODO remove this
        }
    }

    template <typename Rule_t, typename... Args>
    void set_ruleset(Args&&... args)
    {
        rules_ = std::make_unique<Rule_t>(std::forward<Args>(args)...);
    }

    /// Return a container of positions and pieces. Useful for GUI dispaly.
    auto state() const -> State const& { return state_; }

    /// Return a container of positions and pieces. Useful for GUI dispaly.
    auto state() -> State& { return state_; }

    /// Return a point to the Black Side Player.
    auto player_black() const -> Player const& { return player_black_; }

    /// Return a point to the Black Side Player.
    auto player_white() const -> Player const& { return player_white_; }

    /// Verifies the move with the Ruleset, if valid will update the State.
    auto make_move(Move move) -> bool;

    /// Return potential ending positions if given a starting position.
    /// Calculated from the Ruleset. Should be free function that takes in
    /// Ruleset and State.
    auto get_valid_positions(Position position) const -> Positions
    {
        auto lock = std::lock_guard<std::recursive_mutex>{this->mtx};
        return rules_->get_valid_positions(state_, position);
    }

   private:
    State state_;
    std::unique_ptr<const Rules> rules_ = std::make_unique<Standard_rules>();

    Player player_black_ = player::human();
    Player player_white_ = player::human();

    // Alternate between the two Sides.
    void toggle_current_side()
    {
        state_.current_side = opponent(state_.current_side);
    }
};

}  // namespace chess

namespace chess::slot {

template <typename Rule_t, typename... Args>
auto set_ruleset(Chess_engine& engine, Args&&... args) -> sl::Slot<void()>
{
    return cppurses::slot::link_lifetimes(
        [&engine, args...] {
            engine.set_ruleset<Rule_t>(std::forward<Args>(args)...);
        },
        engine);
}

}  // namespace chess::slot
#endif  // CHESS_CHESS_ENGINE_HPP
