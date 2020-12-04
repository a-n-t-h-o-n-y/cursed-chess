#ifndef CHESS_STATE_HPP
#define CHESS_STATE_HPP
#include <mutex>
#include <unordered_map>

#include <signals_light/signal.hpp>

#include "piece.hpp"
#include "position.hpp"
#include "side.hpp"

namespace chess {

struct Board_state {
    using Pieces_t    = std::unordered_map<Position, Piece>;
    using Positions_t = std::vector<Position>;

    /// Holds the state with a Position for each Piece currently on the board.
    Pieces_t pieces;

    /// Provided for locking over multiple operations. Game loop modifies the
    /// state from a thread other than the main thread.
    mutable std::recursive_mutex mtx;

    /// Return a copy of the Piece at Position p. Throws std::out_of_range if
    /// no Piece at \p p. Thread safe.
    Piece at(Position p) const;

    /// Return true if there is a Piece on the board at \p p. Thread safe.
    bool has_piece_at(Position p) const;

    /// Finds all Positions where the Figure appears on the board(Side not used)
    Positions_t find_all(Figure fig) const;

    /// Finds all Positions where the Side appears on the board.
    Positions_t find_all(Side side) const;

    /// Finds all Positions where the Figure and Side appear together in board.
    Positions_t find_all(Figure fig, Side side) const;
};

struct State {
    /// Set up all of the Pieces on the board.
    State();

    /// Holds the Pieces and their positions in a map, with a provided mutex.
    Board_state board;

    /// This is the Side that will be able to make the next move.
    Side current_side;

    bool game_over{false};

    /// Set the Pieces back to their original positions and the current_side.
    void reset();

    /// Signal is called when the board is reset to its initial state.
    sl::Signal<void()> board_reset;
};

}  // namespace chess
#endif  // CHESS_STATE_HPP
