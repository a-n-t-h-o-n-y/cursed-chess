#ifndef RULES_HPP
#define RULES_HPP
#include <mutex>
#include <vector>

#include "piece.hpp"
#include "position.hpp"
#include "state.hpp"

struct Move;

class Rules {
   public:
    using Positions = std::vector<Position>;

    virtual ~Rules() = default;

    /// Validates whether or not a move is valid according to the rules of the
    /// piece and the state of the board.
    bool validate(const chess::State& state, const Move& move) const;

    /// return a vector of potential end positions from start positions.
    Positions get_valid_positions(const chess::State& state,
                                  Position position) const;

    /// Check whether or not the game is in check.
    bool check(const chess::State& state) const;

    /// Check whether or not the game is in checkmate.
    bool checkmate(const chess::State& state) const;

    /// Check whether or not the game is in stalemate.
    bool stalemate(const chess::State& state) const;

   protected:
    /// Return a list of valid `move to` positions for a bishop at \p position.
    virtual Positions get_bishop_moves(const chess::State& state,
                                       Position position) const = 0;

    /// Return a list of valid `move to` positions for a king at \p position.
    virtual Positions get_king_moves(const chess::State& state,
                                     Position position) const = 0;

    /// Return a list of valid `move to` positions for a knight at \p position.
    virtual Positions get_knight_moves(const chess::State& state,
                                       Position position) const = 0;

    /// Return a list of valid `move to` positions for a pawn at \p position.
    virtual Positions get_pawn_moves(const chess::State& state,
                                     Position position) const = 0;

    /// Return a list of valid `move to` positions for a queen at \p position.
    virtual Positions get_queen_moves(const chess::State& state,
                                      Position position) const = 0;

    /// Return a list of valid `move to` positions for a rook at \p position.
    virtual Positions get_rook_moves(const chess::State& state,
                                     Position position) const = 0;
};

// Helper Functions
chess::Side opponent(chess::Side side);
bool is_valid(Position p);

#endif  // RULES_HPP
