#include "chess_ui.hpp"

#include <signals/slot.hpp>

#include <cppurses/system/focus.hpp>

#include "no_rules.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"
#include "side.hpp"
#include "standard_rules.hpp"

using namespace chess;
using namespace cppurses;

Left_side::Left_side() {
    stack.set_name("stack in Left_side");
    board.set_name("board in Left_side");

    this->set_name("Left_side");
    this->width_policy.fixed(26);

    // Stack
    stack.set_active_page(0);
    stack.height_policy.fixed(10);
    stack.give_focus_on_change(false);

    // Settings Pane
    settings.return_btn.clicked.connect(
        cppurses::slot::set_active_page(stack, 0));
    settings.show_moves_box.toggled.connect(
        ::slot::toggle_show_moves(board.chessboard));

    // Black AI
    settings.black_ai.cycle_box.add_option("Human").connect(
        ::slot::set_player<chess::Player_human>(board.chessboard.engine(),
                                                Side::Black));
    settings.black_ai.cycle_box.add_option("Random").connect(
        ::slot::set_player<Player_random_ai>(board.chessboard.engine(),
                                             Side::Black));
    // White AI
    settings.white_ai.cycle_box.add_option("Human").connect(
        ::slot::set_player<chess::Player_human>(board.chessboard.engine(),
                                                Side::White));
    settings.white_ai.cycle_box.add_option("Random").connect(
        ::slot::set_player<Player_random_ai>(board.chessboard.engine(),
                                             Side::White));
    // Rulesets
    settings.ruleset.cycle_box.add_option("Standard")
        .connect(
            ::slot::set_ruleset<Standard_rules>(board.chessboard.engine()));
    settings.ruleset.cycle_box.add_option("No Rules")
        .connect(::slot::set_ruleset<No_rules>(board.chessboard.engine()));

    // Reset Button
    settings.reset_btn.clicked.connect(::slot::reset_game(board.chessboard));

    // Lower_pane
    lower_pane.settings_btn.clicked.connect(
        cppurses::slot::set_active_page(stack, 1));
    lower_pane.move_input.reset_requested.connect(
        ::slot::reset_game(board.chessboard));
    board.chessboard.move_made.connect(
        ::slot::toggle_status(lower_pane, board.chessboard));
    board.chessboard.board_reset.connect(
        [this] { lower_pane.toggle_status(board.chessboard); });
}

void Left_side::enable(bool enable, bool post_child_polished_event) {
    this->enable_and_post_events(enable, post_child_polished_event);
    stack.enable(enable, post_child_polished_event);
    if (lower_pane_enabled) {
        lower_pane.enable(enable, post_child_polished_event);
    } else {
        lower_pane.disable(true, post_child_polished_event);
    }
}

Chess_UI::Chess_UI() {
    this->set_name("Chess_UI - main chess widget");

    // left_side.settings
    left_side.settings.hide_log_box.toggled.connect(::slot::toggle_logs(*this));

    // left_side.board
    left_side.board.chessboard.move_made.connect(
        ::slot::post_move_message(right_side));
    left_side.board.chessboard.move_made.connect(
        ::slot::toggle_status(right_side, left_side.board.chessboard));
    left_side.board.chessboard.capture.connect(
        ::slot::post_capture_message(right_side));
    left_side.board.chessboard.invalid_move.connect(
        ::slot::post_invalid_move_message(right_side));
    left_side.board.chessboard.checkmate.connect(
        ::slot::post_checkmate_message(right_side));
    left_side.board.chessboard.check.connect(
        ::slot::post_check_message(right_side));
    left_side.board.chessboard.board_reset.connect(
        [this] { right_side.toggle_status(left_side.board.chessboard); });
    left_side.board.chessboard.board_reset.connect(
        [this]() { right_side.chess_log.clear(); });

    // right_side
    right_side.settings_btn.clicked.connect(
        cppurses::slot::set_active_page(left_side.stack, 1));
    right_side.move_input.reset_requested.connect(
        ::slot::reset_game(left_side.board.chessboard));
}

void Chess_UI::toggle_logs() {
    left_side.lower_pane_enabled = !left_side.lower_pane_enabled;
    if (left_side.lower_pane_enabled) {
        left_side.settings.border.segments.south_west = L'│';
        left_side.settings.border.segments.north_east = L'╮';
    } else {
        left_side.settings.border.segments.south_west = L'╰';
        left_side.settings.border.segments.north_east = L'─';
    }
    this->enable(this->enabled());
    Focus::clear();
    this->update();
}

void Chess_UI::enable(bool enable, bool post_child_polished_event) {
    this->enable_and_post_events(enable, post_child_polished_event);
    left_side.enable(enable, post_child_polished_event);
    if (left_side.lower_pane_enabled) {
        right_side.disable(true, post_child_polished_event);
    } else {
        right_side.enable(enable, post_child_polished_event);
    }
}

namespace slot {

sig::Slot<void()> toggle_logs(Chess_UI& cfui) {
    sig::Slot<void()> slot{[&cfui] { cfui.toggle_logs(); }};
    slot.track(cfui.destroyed);
    return slot;
}

}  // namespace slot
