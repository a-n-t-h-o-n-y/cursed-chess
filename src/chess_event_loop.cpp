#include "chess_event_loop.hpp"

#include <cppurses/system/event_loop.hpp>

#include "chessboard_widget.hpp"

namespace chess {

void Chess_event_loop::loop_function()
{
    if (chessboard_widget_.engine().state().game_over)
        this->exit(0);
    chessboard_widget_.take_turn();
}

}  // namespace chess
