#ifndef SIDE_PANE_HPP
#define SIDE_PANE_HPP
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/painter/trait.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/button.hpp>
#include <cppurses/widget/widgets/log.hpp>
#include <cppurses/widget/widgets/status_bar.hpp>

#include "chessboard_widget.hpp"
#include "move.hpp"
#include "move_input.hpp"
#include "piece.hpp"
#include "side.hpp"

class Side_pane : public cppurses::layout::Vertical<> {
    struct Status : cppurses::layout::Horizontal<> {
        Status();

        cppurses::Status_bar& status = this->make_child<cppurses::Status_bar>(
            L" W" | cppurses::Trait::Bold | foreground(cppurses::Color::White));

        cppurses::Button& settings_btn =
            this->make_child<cppurses::Button>("Settings");

        Widget& blank_space =
            this->make_child() | cppurses::pipe::fixed_height(2);
    };

   public:
    Side_pane();

    Status& status{this->make_child<Status>()};
    cppurses::Log& chess_log{this->make_child<cppurses::Log>()};
    Move_input& move_input{this->make_child<Move_input>("Type Move")};

    void toggle_status(const Chessboard_widget& board);
    void post_move_message(const Move& m);
    void post_capture_message(Piece p);
    void post_invalid_move_message(const Move& m);
    void post_checkmate_message(chess::Side s);
    void post_check_message(chess::Side s);
    void post_stalemate_message();
};

namespace slot {

sig::Slot<void(Move)> post_move_message(Side_pane& sp);
sig::Slot<void(Move)> toggle_status(Side_pane& sp,
                                    const Chessboard_widget& board);
sig::Slot<void(Piece)> post_capture_message(Side_pane& sp);
sig::Slot<void(const Move&)> post_invalid_move_message(Side_pane& sp);
sig::Slot<void(chess::Side)> post_checkmate_message(Side_pane& sp);
sig::Slot<void(chess::Side)> post_check_message(Side_pane& sp);
sig::Slot<void(chess::Side)> post_stalemate_message(Side_pane& sp);

}  // namespace slot
#endif  // SIDE_PANE_HPP
