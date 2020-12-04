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

/// Responsible for the organizing the state and logic of the chess game. This
/// includes the current state of the board, the players, and the rules.
class Chess_engine {
   public:
    using Positions = std::vector<Position>;

    template <typename T, typename... Args>
    void set_player(chess::Side side, Args... args);

    template <typename Rule_t, typename... Args>
    void set_ruleset(Args&&... args);

    /// Return a container of positions and pieces. Useful for GUI dispaly.
    const chess::State& state() const;

    /// Return a container of positions and pieces. Useful for GUI dispaly.
    chess::State& state();

    /// Return a point to the Black Side Player.
    Player* player_black() const;

    /// Return a point to the Black Side Player.
    Player* player_white() const;

    /// Verifies the move with the Ruleset, if valid will update the State.
    bool make_move(Move move);

    /// Return potential ending positions if given a starting position.
    /// Calculated from the Ruleset. Should be free function that takes in
    /// Ruleset and State.
    Positions get_valid_positions(Position position) const;

    // Signals
    sl::Signal<void(const Move&)> move_made;
    sl::Signal<void(const Move&)> invalid_move;
    sl::Signal<void(Piece)> capture;
    sl::Signal<void(chess::Side)> checkmate;
    sl::Signal<void(chess::Side)> check;
    sl::Signal<void()> stalemate;

    mutable std::recursive_mutex mtx;

    sl::Lifetime lifetime;

   private:
    chess::State state_;
    std::unique_ptr<const Rules> rules_{std::make_unique<Standard_rules>()};

    std::unique_ptr<Player> player_black_{
        std::make_unique<chess::Player_human>(*this)};
    std::unique_ptr<Player> player_white_{
        std::make_unique<chess::Player_human>(*this)};

    // Alternate between the two Sides.
    void toggle_current_side();
};

// Template Implementations - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename... Args>
void Chess_engine::set_player(chess::Side side, Args... args)
{
    if (side == chess::Side::Black) {
        player_black_ = std::make_unique<T>(*this, std::forward<Args>(args)...);
    }
    else if (side == chess::Side::White) {
        player_white_ = std::make_unique<T>(*this, std::forward<Args>(args)...);
    }
}

template <typename Rule_t, typename... Args>
void Chess_engine::set_ruleset(Args&&... args)
{
    rules_ = std::make_unique<Rule_t>(std::forward<Args>(args)...);
}

namespace slot {

template <typename Player_t, typename... Args>
auto set_player(Chess_engine& engine, chess::Side side, Args&&... args)
    -> sl::Slot<void()>
{
    return cppurses::slot::link_lifetimes(
        [&engine, side, args...] {
            engine.set_player<Player_t>(side, std::forward<Args>(args)...);
        },
        engine);
}

template <typename Rule_t, typename... Args>
auto set_ruleset(Chess_engine& engine, Args&&... args) -> sl::Slot<void()>
{
    return cppurses::slot::link_lifetimes(
        [&engine, args...] {
            engine.set_ruleset<Rule_t>(std::forward<Args>(args)...);
        },
        engine);
}

}  // namespace slot
#endif  // CHESS_CHESS_ENGINE_HPP
