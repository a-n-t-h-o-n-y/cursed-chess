#ifndef PIECE_QUEEN_HPP
#define PIECE_QUEEN_HPP

#include "piece.hpp"

class Queen : public Piece {
   public:
    Queen(Side s, Position initial_pos) : Piece(s, initial_pos) {
        display_ = "♛";
    }
    // void update_valid_moves(
    //     std::function<Side(Position)> get_side_at) override {
    //     valid_moves_.clear();
    //     auto pos = this->get_position();
    //     // Up-Left
    //     Position next_pos{pos};
    //     ++next_pos.row;
    //     --next_pos.column;
    //     while (next_pos.row < 9 && next_pos.column > 0 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         ++next_pos.row;
    //         --next_pos.column;
    //     }
    //     // Up-Right
    //     next_pos = pos;
    //     ++next_pos.row;
    //     ++next_pos.column;
    //     while (next_pos.row < 9 && next_pos.column < 9 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         ++next_pos.row;
    //         ++next_pos.column;
    //     }
    //     // Down-Left
    //     next_pos = pos;
    //     --next_pos.row;
    //     --next_pos.column;
    //     while (next_pos.row > 0 && next_pos.column > 0 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         --next_pos.row;
    //         --next_pos.column;
    //     }
    //     // Down-Right
    //     next_pos = pos;
    //     --next_pos.row;
    //     ++next_pos.column;
    //     while (next_pos.row > 0 && next_pos.column < 9 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         --next_pos.row;
    //         ++next_pos.column;
    //     }
    //     // Up
    //     next_pos = pos;
    //     ++next_pos.row;
    //     while (next_pos.row < 9 && get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         ++next_pos.row;
    //     }
    //     // Down
    //     next_pos = pos;
    //     --next_pos.row;
    //     while (next_pos.row > 0 && get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         --next_pos.row;
    //     }
    //     // Left
    //     next_pos = pos;
    //     --next_pos.column;
    //     while (next_pos.column > 0 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         --next_pos.column;
    //     }
    //     // Right
    //     next_pos = pos;
    //     ++next_pos.column;
    //     while (next_pos.column < 9 &&
    //            get_side_at(next_pos) != this->get_side()) {
    //         valid_moves_.push_back(next_pos);
    //         if (get_side_at(next_pos) == this->opponent()) {
    //             break;
    //         }
    //         ++next_pos.column;
    //     }
    // }
};

#endif  // PIECE_QUEEN_HPP
