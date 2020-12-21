#ifndef CHESSBOARD_WIDGET_HPP
#define CHESSBOARD_WIDGET_HPP
#include <optional>

#include <signals_light/signal.hpp>

#include <cppurses/system/mouse.hpp>
#include <cppurses/widget/detail/link_lifetimes.hpp>
#include <cppurses/widget/widget.hpp>

#include "chess_engine.hpp"
#include "chess_event_loop.hpp"
#include "move.hpp"
#include "position.hpp"
#include "shared_user_input.hpp"
#include "side.hpp"

namespace chess {

/// Responsible for displaying the state of the game and for taking user input.
class Chessboard_widget : public cppurses::Widget {
   public:
    Chessboard_widget();

    ~Chessboard_widget() { this->exit_game_loop(); }

   public:
    void toggle_show_moves()
    {
        show_moves_ = !show_moves_;
        this->update();
    }

    void reset_game()
    {
        engine_.state().reset();
        this->update();
    }

    void make_move(Move const& move) { engine_.make_move(move); }

    auto current_side() const -> chess::Side
    {
        return engine_.state().current_side;
    }

    /// Pause the game loop.
    void pause() { this->exit_game_loop(); }

    /// Start the game loop, possibly after a pause.
    void start()
    {
        Shared_user_input::exit_requested = false;
        game_loop_.wait();
        game_loop_.run_async();
    }

    /// Ask the current player for a move, then sends to this->make_move()
    void take_turn();

    /// Called on Chess_move_request_event.
    void move_request_event(Move m) { engine_.make_move(m); }

    // TODO Try not to provide this.
    auto engine() -> Chess_engine& { return engine_; }

    auto engine() const -> Chess_engine const& { return engine_; }

    // Signals
    sl::Signal<void(Move)> move_made;
    sl::Signal<void(Piece)> capture;
    sl::Signal<void(Move const&)> invalid_move;
    sl::Signal<void(chess::Side)> checkmate;
    sl::Signal<void(chess::Side)> check;
    sl::Signal<void()> board_reset;

   protected:
    auto paint_event() -> bool override;

    auto mouse_press_event(cppurses::Mouse const& m) -> bool override;

    auto enable_event() -> bool override;

    auto disable_event() -> bool override;

   private:
    Chess_engine engine_;
    chess::Chess_event_loop game_loop_{*this};

    std::optional<Position> first_position_;
    std::optional<Position> selected_position_;
    bool show_moves_ = false;

   private:
    auto get_tile_color(Position p) -> cppurses::Color;

    void exit_game_loop()
    {
        Shared_user_input::exit_requested = true;
        game_loop_.exit(0);
        game_loop_.wait();
    }
};

}  // namespace chess

namespace chess::slot {

inline auto toggle_show_moves(Chessboard_widget& cbw) -> sl::Slot<void()>
{
    return cppurses::slot::link_lifetimes([&cbw] { cbw.toggle_show_moves(); },
                                          cbw);
}

inline auto reset_game(Chessboard_widget& cbw) -> sl::Slot<void()>
{
    return cppurses::slot::link_lifetimes([&cbw] { cbw.reset_game(); }, cbw);
}

inline auto make_move(Chessboard_widget& cbw) -> sl::Slot<void(Move)>
{
    return cppurses::slot::link_lifetimes([&cbw](Move m) { cbw.make_move(m); },
                                          cbw);
}

}  // namespace chess::slot
#endif  // CHESSBOARD_WIDGET_HPP
