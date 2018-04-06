#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP
#include <memory>
#include <vector>

#include <signals/signal.hpp>

#include "piece.hpp"
#include "position.hpp"
#include "rules.hpp"
#include "standard_rules.hpp"
#include "state.hpp"

class Chess_engine {
   public:
    using Positions = std::vector<Position>;
    Chess_engine();

    void reset();
    bool make_move(Move move);

    Positions get_valid_positions(Position position) const;
    Piece at(Position p) const noexcept(false);
    Positions find_positions(Piece piece) const;
    Side current_side() const;

    template <typename Rule_t, typename... Args>
    void set_ruleset(Args&&... args);

    // Signals
    sig::Signal<void(const Move&)> move_made;
    sig::Signal<void(Piece)> capture;
    sig::Signal<void(const Move&)> invalid_move;
    sig::Signal<void(Side)> checkmate;
    sig::Signal<void(Side)> check;
    sig::Signal<void()> stalemate;
    sig::Signal<void()> board_reset;

   private:
    State state_;
    std::unique_ptr<const Rules> rules_{std::make_unique<Standard_rules>()};
};

// Template Implementations - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename Rule_t, typename... Args>
void Chess_engine::set_ruleset(Args&&... args) {
    rules_ = std::make_unique<Rule_t>(std::forward<Args>(args)...);
}

#endif  // CHESS_ENGINE_HPP
