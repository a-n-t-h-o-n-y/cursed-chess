#ifndef STATE_HPP
#define STATE_HPP
#include "piece.hpp"
#include "position.hpp"

#include <signals/signal.hpp>

#include <utility>
#include <vector>

struct Piece_in_play {
    Piece_in_play(Piece piece_, Position position_);  // for emplace_back()
    Piece piece;
    Position position;
};

class State {
   public:
    State();

    Piece& at(Position p) noexcept(false);
    const Piece& at(Position p) const noexcept(false);

    void reset();
    void set_current_side(Color side);
    Color current_side() const;

    // Signals
    sig::Signal<void(const Move&)> move_made;

   private:
    std::vector<Piece_in_play> pieces_;
    Color current_side_{Color::White};

    Piece& find_piece();
    const Piece& find_piece() const;
};

// Free Functions

bool add(State& state, Position position, Piece piece);
void remove(State& state, Position position);
void toggle_current_side(State& state);
void make_move(State& state, const Move& move);

make_move

#endif  // STATE_HPP
