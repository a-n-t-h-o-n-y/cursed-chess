#ifndef CHESSBOARD_WIDGET_HPP
#define CHESSBOARD_WIDGET_HPP
#include <optional>

#include <cppurses/system/events/mouse.hpp>
#include <cppurses/widget/widget.hpp>

#include <signals/signals.hpp>

#include "chess_engine.hpp"
#include "chess_event_loop.hpp"
#include "move.hpp"
#include "position.hpp"
#include "side.hpp"

/// Responsible for displaying the state of the game and for taking user input.
class Chessboard_widget : public cppurses::Widget {
   public:
    Chessboard_widget();

    void toggle_show_moves();
    void reset_game();
    void make_move(const Move& move);
    chess::Side current_side() const;

    /// Pause the game loop.
    void pause();

    /// Start the game loop, possibly after a pause.
    void start();

    /// Ask the current player for a move, then sends to this->make_move()
    void take_turn();

    /// Called on Chess_move_request_event.
    void move_request_event(Move m);

    // TODO Try not to provide this.
    Chess_engine& engine();
    const Chess_engine& engine() const;

    // Signals
    sig::Signal<void(Move)> move_made;
    sig::Signal<void(Piece)> capture;
    sig::Signal<void(const Move&)> invalid_move;
    sig::Signal<void(chess::Side)> checkmate;
    sig::Signal<void(chess::Side)> check;
    sig::Signal<void()> board_reset;

   protected:
    bool paint_event() override;
    bool mouse_press_event(const cppurses::Mouse::State& mouse) override;
    bool enable_event() override;
    bool disable_event() override;

   private:
    Chess_engine engine_;
    chess::Chess_event_loop game_loop_{*this};

    std::optional<Position> first_position_;
    std::optional<Position> selected_position_;
    bool show_moves_{false};

    cppurses::Color get_tile_color(Position p);
};

namespace slot {

sig::Slot<void()> toggle_show_moves(Chessboard_widget& cbw);
sig::Slot<void()> reset_game(Chessboard_widget& cbw);
sig::Slot<void(Move)> make_move(Chessboard_widget& cbw);

}  // namespace slot
#endif  // CHESSBOARD_WIDGET_HPP
