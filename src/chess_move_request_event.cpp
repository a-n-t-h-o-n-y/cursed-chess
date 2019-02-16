#include "chess_move_request_event.hpp"

#include <cppurses/widget/widget.hpp>

#include "chessboard_widget.hpp"

namespace chess {

Chess_move_request_event::Chess_move_request_event(Chessboard_widget& cbw,
                                                   struct Move m)
    : cppurses::Event{cppurses::Event::Custom, cbw}, move_{m} {}

bool Chess_move_request_event::send() const {
    static_cast<Chessboard_widget&>(receiver_).move_request_event(move_);
    return true;
}

bool Chess_move_request_event::filter_send(
    cppurses::Widget& /* filter */) const {
    return false;
}
}  // namespace chess
