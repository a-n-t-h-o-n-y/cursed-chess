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

// Responsible for making moves, retrieving potential moves from positions
// Pretty much everything that state and rules do, but brought together
// class Chess_engine {
//    public:
//     Chess_engine();

//     bool make_move(Position pos1, Position pos2);

//     std::vector<Position> get_moves(Position pos);

//     std::vector<std::pair<Position, Position>> get_all_potential_moves(
//         Side side);

//     Side current_side() const;
//     const std::vector<std::unique_ptr<Piece>>& get_pieces() const;

//     bool game_over() const;
//     void reset_game();

//     std::string status_string() const;

//     // Signal
//     sig::Signal<void(std::string)> move_message_sig;
//     sig::Signal<void(std::string)> status_message_sig;
//     sig::Signal<void()> move_made_sig;

//     friend class Chessboard_widget;

//    private:
//     Side current_side_{Side::White};
//     std::vector<std::unique_ptr<Piece>> pieces_;
//     int move_count_{0};

//     void flip_side();
//     void remove_piece(Position p);
//     Side get_side(Position p) const;
//     Piece* find_piece(Position p) const;
//     bool check_move(Position pos1, Position pos2);
//     static std::string int_to_tiny(const std::string& number);
//     void report_move(Position p1, Position p2);
//     void reset_board();
// };

#endif  // CHESS_ENGINE_HPP
