#ifndef CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
#define CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
#include <cppurses/system/event.hpp>
#include <cppurses/system/event_handler.hpp>

#include "move.hpp"

class Chessboard_widget;
namespace chess {

class Chess_move_request_event : public cppurses::Event {
   public:
    // TODO Can remove struct keyword when Move is in chess namespace, must
    // qualify it though with chess::Move because Event::Move is a value.
    Chess_move_request_event(Chessboard_widget* cbw, struct Move m);

    bool send() const override;

    bool filter_send(cppurses::Event_handler* filter) const override;

   private:
    struct Move move_;
};

}  // namespace chess
#endif  // CHESS_CHESS_MOVE_REQUREST_EVENT_HPP
