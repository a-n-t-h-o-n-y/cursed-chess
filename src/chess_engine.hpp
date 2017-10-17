#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP
#include "piece.hpp"
#include "position.hpp"
#include "state.hpp"
#include "rules.hpp"

#include <signals/signal.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

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
    const Rules rules_;
};

#endif  // CHESS_ENGINE_HPP
