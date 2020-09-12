#ifndef CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
#define CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
#include <cppurses/system/event.hpp>

#include "chessboard_widget.hpp"
#include "move.hpp"

namespace chess {

inline auto chess_move_request_event(Chessboard_widget& cbw, struct Move m)
    -> cppurses::Custom_event
{
    return {[&cbw, m] { cbw.move_request_event(m); }};
}

}  // namespace chess
#endif  // CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
