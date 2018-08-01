#ifndef SIDE_PANE_HPP
#define SIDE_PANE_HPP
#include <cppurses/cppurses.hpp>

#include "chessboard_widget.hpp"
#include "move.hpp"
#include "move_input.hpp"
#include "piece.hpp"

using namespace cppurses;

struct Side_pane : public Vertical_layout {
    Side_pane();

    Horizontal_layout& hl_status{this->make_child<Horizontal_layout>()};
    Status_bar& status{hl_status.make_child<Status_bar>(
        Glyph_string{" W", Attribute::Bold, foreground(Color::White)})};
    Push_button& settings_btn{hl_status.make_child<Push_button>("Settings")};
    Blank_height& blank_space{hl_status.make_child<Blank_height>(2)};

    Log& chess_log{this->make_child<Log>()};

    Horizontal_layout& hl{this->make_child<Horizontal_layout>()};
    Move_input& move_input{hl.make_child<Move_input>("Type Move")};

    void toggle_status(const Chessboard_widget& board);
    void post_move_message(const Move& m);
    void post_capture_message(Piece p);
    void post_invalid_move_message(const Move& m);
    void post_checkmate_message(Side s);
    void post_check_message(Side s);
    void post_stalemate_message();
};

namespace slot {

sig::Slot<void(Move)> post_move_message(Side_pane& sp);
sig::Slot<void(Move)> toggle_status(Side_pane& sp,
                                    const Chessboard_widget& board);
sig::Slot<void(Piece)> post_capture_message(Side_pane& sp);
sig::Slot<void(const Move&)> post_invalid_move_message(Side_pane& sp);
sig::Slot<void(Side)> post_checkmate_message(Side_pane& sp);
sig::Slot<void(Side)> post_check_message(Side_pane& sp);
sig::Slot<void(Side)> post_stalemate_message(Side_pane& sp);

}  // namespace slot

#endif  // SIDE_PANE_HPP
