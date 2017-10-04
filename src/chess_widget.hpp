#ifndef CHESS_WIDGET_HPP
#define CHESS_WIDGET_HPP
#include "manager.hpp"
#include "player.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>

#include <string>

using namespace cppurses;

class Chess_widget : public Widget {
   public:
    Chess_widget();

    void make_move(Position one, Position two);
    void toggle_show_moves();
    void reset_game(Push_button& button);
    void change_ai(Side player, std::string ai);
    void next_player_move();

    // Signals
    sig::Signal<void(std::string)>& move_message{manager_.move_message_sig};
    sig::Signal<void(std::string)>& status_message{manager_.status_message_sig};

   protected:
    bool paint_event() override;
    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override;

   private:
    Manager manager_;
    Position first_position_;
    Position second_position_;
    bool first_{true};
    bool show_moves_{false};
    Piece* selected_piece_{nullptr};
    bool reset_first_click_{true};

    std::unique_ptr<Player> player_white_{
        std::make_unique<Player_human>(&manager_)};
    std::unique_ptr<Player> player_black_{
        std::make_unique<Player_human>(&manager_)};

    std::unique_ptr<Player>& get_current_player();
    Color get_background(Position p);

    Position screen_to_board_index(Position i);
    Position board_to_screen_index(Position i);
    void send_move();
};

namespace slot {

sig::Slot<void(Position, Position)> make_move(Chess_widget& cw);
sig::Slot<void()> toggle_show_moves(Chess_widget& cw);
sig::Slot<void(Push_button*)> reset_game(Chess_widget& cw);
sig::Slot<void(Side, std::string)> change_ai(Chess_widget& cw);
sig::Slot<void()> next_player_move(Chess_widget& cw);

}  // namespace slot

#endif  // CHESS_WIDGET_HPP
