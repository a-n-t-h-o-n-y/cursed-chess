#include "chess_event_loop.hpp"

#include <cppurses/system/event_loop.hpp>

#include "chessboard_widget.hpp"
#include "shared_user_input.hpp"

namespace chess {

Chess_event_loop::Chess_event_loop(Chessboard_widget& cbw)
    : chessboard_widget_{cbw}
{}

auto Chess_event_loop::loop_function() -> bool
{
    if (chessboard_widget_.engine().state().game_over)
        this->exit(0);
    chessboard_widget_.take_turn();
    return true;
}

void Chess_event_loop::exit(int return_code)
{
    Shared_user_input::exit_requested = true;
    Event_loop::exit(return_code);
}

}  // namespace chess
