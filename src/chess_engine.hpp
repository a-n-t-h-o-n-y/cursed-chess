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

    private:
        State state_;
        Rules rules_;
};

class Chess_engine {
   public:
    Chess_engine();

    bool make_move(Position pos1, Position pos2);

    std::vector<Position> get_moves(Position pos);

    std::vector<std::pair<Position, Position>> get_all_potential_moves(
        Side side);

    Side current_side() const;
    const std::vector<std::unique_ptr<Piece>>& get_pieces() const;

    bool game_over() const;
    void reset_game();

    std::string status_string() const;

    // Signal
    sig::Signal<void(std::string)> move_message_sig;
    sig::Signal<void(std::string)> status_message_sig;
    sig::Signal<void()> move_made_sig;

    friend class Chessboard_widget;

   private:
    Side current_side_{Side::White};
    std::vector<std::unique_ptr<Piece>> pieces_;
    int move_count_{0};

    void flip_side();
    void remove_piece(Position p);
    Side get_side(Position p) const;
    Piece* find_piece(Position p) const;
    bool check_move(Position pos1, Position pos2);
    static std::string int_to_tiny(const std::string& number);
    void report_move(Position p1, Position p2);
    void reset_board();
};

#endif  // CHESS_ENGINE_HPP
