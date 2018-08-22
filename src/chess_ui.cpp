#include "chess_ui.hpp"

#include <string>

#include <signals/slot.hpp>

#include "no_rules.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"
#include "standard_rules.hpp"

Chess_UI::Chess_UI() {
    this->set_name("Chess_UI - main chess widget");
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

    // Black AI
    settings.black_ai.cycle_box.add_option("Human").connect(
        ::slot::set_player<Player_human>(board.chessboard, Side::Black));
    settings.black_ai.cycle_box.add_option("Random").connect(
        ::slot::set_player<Player_random_ai>(board.chessboard, Side::Black));

    // White AI
    settings.white_ai.cycle_box.add_option("Human").connect(
        ::slot::set_player<Player_human>(board.chessboard, Side::White));
    settings.white_ai.cycle_box.add_option("Random").connect(
        ::slot::set_player<Player_random_ai>(board.chessboard, Side::White));

    // Rulesets
    settings.ruleset.cycle_box.add_option("Standard Chess")
        .connect(::slot::set_ruleset<Standard_rules>(board.chessboard));
    settings.ruleset.cycle_box.add_option("No Rules")
        .connect(::slot::set_ruleset<No_rules>(board.chessboard));

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

    // lower_pane.disable();
    blank_.height_policy.type(Size_policy::Expanding);
    lower_pane.move_input.reset_requested.connect(
        ::slot::reset_game(board.chessboard));
    lower_pane.move_input.move_requested.connect(
        ::slot::make_move(board.chessboard));

    board.chessboard.board_reset.connect(
        [this] { lower_pane.toggle_status(board.chessboard); });
}

void Chess_UI::toggle_logs() {
    if (lower_pane_enabled_) {
        // side_on = false;
        settings.border.south_west = L'╰';
        settings.border.north_east = L'─';
    } else {
        settings.border.south_west = L'│';
        settings.border.north_east = L'╮';
    }
    this->enable();
    Focus::clear_focus();
    this->update();
}

void Chess_UI::enable(bool enable, bool post_child_polished_event) {
    this->enable_and_post_events(enable, post_child_polished_event);
    this->vl.enable(enable, post_child_polished_event);
    if (lower_pane_enabled_) {
        lower_pane.enable(enable, post_child_polished_event);
    } else {
        side_pane.enable(enable, post_child_polished_event);
    }
}

namespace slot {

sig::Slot<void()> toggle_logs(Chess_UI& cfui) {
    sig::Slot<void()> slot{[&cfui] { cfui.toggle_logs(); }};
    slot.track(cfui.destroyed);
    return slot;
}

}  // namespace slot
