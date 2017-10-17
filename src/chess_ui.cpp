#include "chess_ui.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"

#include <signals/slot.hpp>

#include <string>

Chess_UI::Chess_UI() {
    stack.set_active_page(0);
    vl.width_policy.type(Size_policy::Fixed);
    vl.width_policy.hint(26);
    stack.height_policy.type(Size_policy::Fixed);
    stack.height_policy.hint(10);

    // Stack
    stack.sets_focus_on_change(false);

    // Settings Pane
    settings.return_btn.clicked.connect(
        cppurses::slot::set_active_page(stack, 0));

    settings.show_moves_box.toggled.connect(
        ::slot::toggle_show_moves(board.chessboard));

    settings.hide_log_box.toggled.connect(::slot::toggle_logs(*this));

    settings.black_ai.option_changed.connect(
        ::slot::set_player(board.chessboard, Side::Black));

    settings.white_ai.option_changed.connect(
        ::slot::set_player(board.chessboard, Side::White));

    // Reset Button
    settings.reset_btn.clicked.connect(::slot::reset_game(board.chessboard));

    // Side Pane
    side_pane.settings_btn.clicked.connect(
        cppurses::slot::set_active_page(stack, 1));

    board.chessboard.move_made.connect(::slot::post_move_message(side_pane));
    board.chessboard.move_made.connect(
        ::slot::toggle_status(side_pane, board.chessboard));
    board.chessboard.capture.connect(::slot::post_capture_message(side_pane));
    board.chessboard.invalid_move.connect(
        ::slot::post_invalid_move_message(side_pane));
    board.chessboard.checkmate.connect(
        ::slot::post_checkmate_message(side_pane));
    board.chessboard.check.connect(::slot::post_check_message(side_pane));

    side_pane.move_input.reset_requested.connect(
        ::slot::reset_game(board.chessboard));
    side_pane.move_input.move_requested.connect(
        ::slot::make_move(board.chessboard));
    board.chessboard.board_reset.connect(
        [this] { side_pane.toggle_status(board.chessboard); });
    board.chessboard.board_reset.connect(
        cppurses::slot::clear(side_pane.chess_log));

    // Lower Pane
    board.chessboard.move_made.connect(
        ::slot::toggle_status(lower_pane, board.chessboard));
    lower_pane.settings_btn.clicked.connect(
        cppurses::slot::set_active_page(stack, 1));

    lower_pane.set_visible(false);
    blank_.height_policy.type(Size_policy::Expanding);
    lower_pane.move_input.reset_requested.connect(
        ::slot::reset_game(board.chessboard));
    lower_pane.move_input.move_requested.connect(
        ::slot::make_move(board.chessboard));

    board.chessboard.board_reset.connect(
        [this] { lower_pane.toggle_status(board.chessboard); });
}

void Chess_UI::toggle_logs() {
    bool side_on{true};
    if (lower_pane.visible()) {
        side_on = false;
        settings.border.south_west = "╰";
        settings.border.north_east = "─";
    } else {
        settings.border.south_west = "│";
        settings.border.north_east = "╮";
    }
    side_pane.set_visible(!side_on);
    side_pane.set_enabled(!side_on);
    lower_pane.set_visible(side_on);
    lower_pane.set_enabled(side_on);
    Focus::clear_focus();
    this->update();
}

namespace slot {

sig::Slot<void()> toggle_logs(Chess_UI& cfui) {
    sig::Slot<void()> slot{[&cfui] { cfui.toggle_logs(); }};
    slot.track(cfui.destroyed);
    return slot;
}

sig::Slot<void(const std::string&)> set_player(Chessboard_widget& board,
                                               Side side) {
    sig::Slot<void(const std::string&)> slot{
        [&board, side](const std::string& name) {
            if (name == "Human") {
                board.set_player<Player_human>(side);
            } else if (name == "Random") {
                board.set_player<Player_random_ai>(side);
            }
        }};
    slot.track(board.destroyed);
    return slot;
}

}  // namespace slot
