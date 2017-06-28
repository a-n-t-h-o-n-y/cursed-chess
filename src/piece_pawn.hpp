#ifndef PIECE_PAWN_HPP
#define PIECE_PAWN_HPP

#include "piece.hpp"
#include <functional>

class Pawn : public Piece {
   public:
    Pawn(Side s, Position initial_pos) : Piece(s, initial_pos) {
        display_ = "♟";
    }
    void update_valid_moves(
        std::function<Side(Position)> get_side_at) override {
        valid_moves_.clear();
        int sign{0};
        if (this->side_ == Side::White) {
            sign = 1;
        } else {
            sign = -1;
        }

        // First move +2 in column
        if (hasnt_moved()) {
            Position new_pos{pos_.row + (2 * sign), pos_.column};
            if (get_side_at(new_pos) == Side::None) {
                valid_moves_.push_back(new_pos);
            }
        }

        // +1 in row
        Position new_pos{pos_.row + (1 * sign), pos_.column};
        if (get_side_at(new_pos) == Side::None) {
            valid_moves_.push_back(new_pos);
        }

        // +1row  -1column if opponent at that piece
        new_pos = Position{pos_.row + (1 * sign), pos_.column - 1};
        if (get_side_at(new_pos) == this->opponent()) {
            valid_moves_.push_back(new_pos);
        }

        // +1row  +1column if opponent at that piece
        new_pos = Position{pos_.row + (1 * sign), pos_.column + 1};
        if (get_side_at(new_pos) == this->opponent()) {
            valid_moves_.push_back(new_pos);
        }
    }

   private:
    bool hasnt_moved() const {
        if (side_ == Side::White && pos_.row == 2) {
            return true;
        }
        if (side_ == Side::Black && pos_.row == 7) {
            return true;
        }
        return false;
    }
};

#endif  // PIECE_PAWN_HPP
