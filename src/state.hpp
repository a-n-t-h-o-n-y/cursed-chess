#ifndef STATE_HPP
#define STATE_HPP
#include <vector>

#include <signals/signal.hpp>

#include "piece.hpp"
#include "position.hpp"

struct Move;

class State {
   public:
    using Positions = std::vector<Position>;

    State();
    void reset();

    Piece& at(Position position) noexcept(false);
    Piece at(Position position) const noexcept(false);

    Positions find_positions(Piece piece) const;

    void set_current_side(Side side);
    Side current_side() const;

    // Signals
    sig::Signal<void(const Move&)> move_made;
    sig::Signal<void()> board_reset;

   private:
    struct Piece_in_play {
        Piece_in_play(Piece piece_, Position position_);  // for emplace_back()
        Piece piece;
        Position position;
    };

    std::vector<Piece_in_play> pieces_;
    Side current_side_{Side::White};

    Piece& find_piece(Position p);
    const Piece& find_piece(Position p) const;
};

// Free Functions

bool add(State& state, Position position, Piece piece);
void remove(State& state, Position position);
void toggle_current_side(State& state);
void make_move(State& state, const Move& move);

#endif  // STATE_HPP
