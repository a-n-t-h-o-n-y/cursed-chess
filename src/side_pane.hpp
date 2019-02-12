#ifndef SIDE_PANE_HPP
#define SIDE_PANE_HPP
#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>
#include <cppurses/widget/widgets/log.hpp>
#include <cppurses/widget/widgets/push_button.hpp>
#include <cppurses/widget/widgets/status_bar.hpp>

#include "chessboard_widget.hpp"
#include "move.hpp"
#include "move_input.hpp"
#include "piece.hpp"
#include "side.hpp"

struct Side_pane : public cppurses::layout::Vertical {
    Side_pane();

    cppurses::layout::Horizontal& hl_status{
        this->make_child<cppurses::layout::Horizontal>()};
    cppurses::Status_bar& status{hl_status.make_child<cppurses::Status_bar>(
        cppurses::Glyph_string{" W", cppurses::Attribute::Bold,
                               foreground(cppurses::Color::White)})};
    cppurses::Push_button& settings_btn{
        hl_status.make_child<cppurses::Push_button>("Settings")};
    cppurses::Fixed_height& blank_space{
        hl_status.make_child<cppurses::Fixed_height>(2)};

    cppurses::Log& chess_log{this->make_child<cppurses::Log>()};

    cppurses::layout::Horizontal& hl{
        this->make_child<cppurses::layout::Horizontal>()};
    Move_input& move_input{hl.make_child<Move_input>("Type Move")};

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
