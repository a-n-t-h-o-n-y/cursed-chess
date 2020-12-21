#include "chess_ui.hpp"

#include <memory>
#include <utility>

#include <signals_light/signal.hpp>

#include <cppurses/system/system.hpp>
#include <cppurses/widget/pipe.hpp>

#include "no_rules.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"
#include "standard_rules.hpp"

using namespace cppurses;

namespace chess {

Left_side::Left_side()
{
    using namespace cppurses::slot;
    using namespace cppurses::pipe;

    *this | fixed_width(26) | passive_height();

    // Lower_pane
    lower_pane.board_request.connect(set_active_page(board_and_settings, 0));
    lower_pane.settings_request.connect(set_active_page(board_and_settings, 1));

    lower_pane.reset_request.connect(
        slot::reset_game(board_and_settings.board.chessboard));

    board_and_settings.board.chessboard.move_made.connect(
        slot::toggle_status(lower_pane, board_and_settings.board.chessboard));

    board_and_settings.board.chessboard.board_reset.connect([this] {
        lower_pane.toggle_status(board_and_settings.board.chessboard);
    });
}

Left_side::Board_and_settings::Board_and_settings()
{
    using namespace cppurses::pipe;

    *this | active_page(0) | fixed_height(10);
    this->give_focus_on_change(false);

    // Show Moves
    settings.toggle_show_moves.connect(
        slot::toggle_show_moves(board.chessboard));

    // AI
    settings.ai_box.set.connect([this](Side s, Player p) {
        board.chessboard.engine().set_player(s, p);
    });

    // Rulesets
    settings.ruleset.cycle_box.add_option(L"Standard")
        .connect(slot::set_ruleset<Standard_rules>(board.chessboard.engine()));
    settings.ruleset.cycle_box.add_option(L"No Rules")
        .connect(slot::set_ruleset<No_rules>(board.chessboard.engine()));

    // Reset Button
    settings.reset_request.connect(slot::reset_game(board.chessboard));
}

}  // namespace chess
