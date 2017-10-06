#ifndef PIECE_HPP
#define PIECE_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include "position.hpp"

enum Color { None, White, Black };

enum Figure { None, Bishop, King, Knight, Pawn, Queen, Rook };

struct Piece {
    Color side;
    Figure figure;
};

class Piece {
   public:
    Piece(Side s, Position initial_pos) : side_{s}, pos_{initial_pos} {}
    std::string get_display() const { return display_; }
    Side get_side() const { return side_; }
    Position get_position() const { return pos_; }
    void set_position(Position pos) { pos_ = pos; }
    std::vector<Position> valid_moves() const { return valid_moves_; }
    bool can_move(Position p) {
        return std::find(std::begin(valid_moves_), std::end(valid_moves_), p) !=
               std::end(valid_moves_);
    }
    virtual void update_valid_moves(
        std::function<Side(Position)> get_side_at) = 0;
    Side opponent() const {
        if (side_ == Side::Black) {
            return Side::White;
        }
        return Side::Black;
    }

   protected:
    Side side_;
    Position pos_;
    std::vector<Position> valid_moves_;

    std::string display_;
};

#endif  // PIECE_HPP
