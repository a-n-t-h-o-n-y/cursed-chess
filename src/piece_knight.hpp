#ifndef PIECE_KNIGHT_HPP
#define PIECE_KNIGHT_HPP

#include "piece.hpp"

class Knight : public Piece {
   public:
    Knight(Side s, Position initial_pos) : Piece(s, initial_pos) {
        display_ = "♞";
    }
    // void update_valid_moves(
    //     std::function<Side(Position)> get_side_at) override {
    //     valid_moves_.clear();
    //     auto pos = this->get_position();
    //     // top left
    //     Position new_pos = Position{pos.row + 2, pos.column - 1};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }
    //     new_pos = Position{pos.row + 1, pos.column - 2};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }

    //     // top right
    //     new_pos = Position{pos.row + 2, pos.column + 1};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }
    //     new_pos = Position{pos.row + 1, pos.column + 2};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }

    //     // bottom left
    //     new_pos = Position{pos.row - 1, pos.column - 2};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }
    //     new_pos = Position{pos.row - 2, pos.column - 1};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }

    //     // bottom right
    //     new_pos = Position{pos.row - 1, pos.column + 2};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }
    //     new_pos = Position{pos.row - 2, pos.column + 1};
    //     if (get_side_at(new_pos) != this->get_side()) {
    //         valid_moves_.push_back(new_pos);
    //     }
    // }
};

#endif  // PIECE_KNIGHT_HPP
