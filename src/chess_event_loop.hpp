#ifndef CHESS_CHESS_EVENT_LOOP_HPP
#define CHESS_CHESS_EVENT_LOOP_HPP
#include <cppurses/system/event_loop.hpp>

namespace chess {
class Chessboard_widget;

class Chess_event_loop : public cppurses::Event_loop {
   public:
    Chess_event_loop(Chessboard_widget& cbw) : chessboard_widget_{cbw} {}

   protected:
    /// Run on each iteration of the event loop.
    void loop_function() override;

   private:
    Chessboard_widget& chessboard_widget_;
};

}  // namespace chess
#endif  // CHESS_CHESS_EVENT_LOOP_HPP
