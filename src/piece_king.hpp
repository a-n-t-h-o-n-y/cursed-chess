#ifndef PIECE_KING_HPP
#define PIECE_KING_HPP

#include "piece.hpp"
#include <functional>

class King : public Piece {
   public:
    King(Side s, Position initial_pos) : Piece(s, initial_pos) {
        display_ = "♚";
    }
    // void update_valid_moves(
    //     std::function<Side(Position)> get_side_at) override {
    //     valid_moves_.clear();
    //     auto pos = this->get_position();
    //     auto next_pos = pos;
    //     // up
    //     ++next_pos.row;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // down
    //     next_pos = pos;
    //     --next_pos.row;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // left
    //     next_pos = pos;
    //     --next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // right
    //     next_pos = pos;
    //     ++next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // Top left
    //     next_pos = pos;
    //     ++next_pos.row;
    //     --next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // Top right
    //     next_pos = pos;
    //     ++next_pos.row;
    //     ++next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // Bottom Left
    //     next_pos = pos;
    //     --next_pos.row;
    //     --next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    //     // Bottom Right
    //     next_pos = pos;
    //     --next_pos.row;
    //     ++next_pos.column;
    //     if (get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //     }
    // }
};

#endif  // PIECE_KING_HPP
