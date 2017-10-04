#ifndef CHESSBOARD_WIDGET_HPP
#define CHESSBOARD_WIDGET_HPP
#include "chess_engine.hpp"
#include "position.hpp"
#include "player.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>

#include <string>

using namespace cppurses;

class Chessboard_widget : public Widget {
   public:
    Chessboard_widget();

    void make_move(Position one, Position two);
    void toggle_show_moves();
    void reset_game(Push_button& button);
    void change_ai(Side player, std::string ai);
    void next_player_move();

    // Signals
    sig::Signal<void(std::string)>& move_message{
        Chess_engine_.move_message_sig};
    sig::Signal<void(std::string)>& status_message{
        Chess_engine_.status_message_sig};

   protected:
    bool paint_event() override;
    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override;

   private:
    Chess_engine Chess_engine_;
    Position first_position_;
    Position second_position_;
    bool first_{true};
    bool show_moves_{false};
    Piece* selected_piece_{nullptr};
    bool reset_first_click_{true};

    std::unique_ptr<Player> player_white_{
        std::make_unique<Player_human>(&Chess_engine_)};
    std::unique_ptr<Player> player_black_{
        std::make_unique<Player_human>(&Chess_engine_)};

    std::unique_ptr<Player>& get_current_player();
    Color get_background(Position p);

    Position screen_to_board_index(Position i);
    Position board_to_screen_index(Position i);
    void send_move();
};

namespace slot {

sig::Slot<void(Position, Position)> make_move(Chessboard_widget& cw);
sig::Slot<void()> toggle_show_moves(Chessboard_widget& cw);
sig::Slot<void(Push_button*)> reset_game(Chessboard_widget& cw);
sig::Slot<void(Side, std::string)> change_ai(Chessboard_widget& cw);
sig::Slot<void()> next_player_move(Chessboard_widget& cw);

}  // namespace slot

#endif  // CHESSBOARD_WIDGET_HPP
