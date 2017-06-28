#ifndef CHESS_WIDGET_HPP
#define CHESS_WIDGET_HPP

#include "manager.hpp"
#include "player.hpp"
#include "player_human.hpp"
#include "player_random_ai.hpp"
#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>
#include <string>

class Chess_widget : public cppurses::Widget {
   public:
    Chess_widget() {
        this->size_policy().vertical_policy = cppurses::Size_policy::Fixed;
        this->geometry().set_height_hint(8);
        this->size_policy().horizontal_policy = cppurses::Size_policy::Fixed;
        this->geometry().set_width_hint(24);

        // Slots
        make_move.track(this->destroyed);
        make_move = [this](Position p1, Position p2) {
            this->manager_.make_move(p1, p2);
            this->update();
        };

        toggle_show_moves.track(this->destroyed);
        toggle_show_moves = [this] {
            show_moves_ = !show_moves_;
            this->update();
        };

        reset_game.track(this->destroyed);
        reset_game = [this](auto* button) {
            if (reset_first_click_) {
                button->set_text("ᴿ ᵘ ˢᵘʳᵉ?");
                reset_first_click_ = false;
            } else {
                button->set_text("ᴿᵉˢᵉᵗ");
                reset_first_click_ = true;
                manager_.reset_game();
                this->update();
            }
        };

        next_player_move_.track(this->destroyed);
        next_player_move_ = [this] {
            cppurses::System::send_event(this, cppurses::Paint_event());
            cppurses::System::paint_engine()->flush(true);
            if (manager_.game_over()) {
                return;
            }
            auto& next_player = this->get_current_player();
            next_player->make_move();
        };

        change_ai.track(this->destroyed);
        change_ai = [this](Side p, std::string ai) {
            std::unique_ptr<Player> ai_ptr;
            if (ai == "human") {
                ai_ptr = std::make_unique<Player_human>(&manager_);
            } else if (ai == "ai_random") {
                ai_ptr = std::make_unique<Player_random_ai>(&manager_);
            }
            if (p == Side::White) {
                player_white_ = std::move(ai_ptr);
            } else if (p == Side::Black) {
                player_black_ = std::move(ai_ptr);
            }
        };
        manager_.move_made_sig.connect(next_player_move_);
    }

    bool paint_event(const cppurses::Paint_event& event) override {
        cppurses::Painter p{this};
        // Paint Board
        // Cell type 1
        cppurses::Glyph_string cell1{"   ",
                                     background(cppurses::Color::Light_gray)};

        // Cell type 2
        cppurses::Glyph_string cell2{"   ", background(cppurses::Color::Brown)};

        // Paint
        for (int i{0}; i < 4; ++i) {
            p.put(cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2,
                  0, i * 2);
            p.put(cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1,
                  0, i * 2 + 1);
        }

        // Possible Moves
        if (show_moves_ && selected_piece_ != nullptr) {
            auto moves = manager_.get_moves(selected_piece_->get_position());
            cppurses::Glyph_string highlight{
                "   ", cppurses::background(cppurses::Color::Light_green)};
            for (const auto& move : moves) {
                if (move.row > 0 && move.row < 9 && move.column > 0 &&
                    move.column < 9) {
                    auto screen_pos = board_to_screen_index(move);
                    p.put(highlight, screen_pos.row - 1, screen_pos.column);
                }
            }
        }

        // Paint Pieces
        const auto& pieces = manager_.get_pieces();
        for (const auto& piece : pieces) {
            // color
            cppurses::Glyph visual{piece->get_display()};
            if (piece->get_side() == Side::Black) {
                visual.brush().set_foreground(cppurses::Color::Black);
            } else {
                visual.brush().set_foreground(cppurses::Color::White);
            }
            // background color
            visual.brush().set_background(
                this->get_background(piece->get_position()));
            // position
            auto board_pos = board_to_screen_index(piece->get_position());
            p.put(visual, board_pos.row, board_pos.column);
        }
        return Widget::paint_event(event);
    }

    bool mouse_press_event(const cppurses::Mouse_event& event) override {
        auto loc_x = static_cast<int>(event.local_x());
        auto loc_y = static_cast<int>(event.local_y());
        auto board_index = screen_to_board_index(Position{loc_x, loc_y});
        auto* piece = manager_.find_piece(board_index);
        if (first_ || (piece != nullptr &&
                       piece->get_side() == manager_.current_side())) {
            if (piece == nullptr) {
                return true;
            }
            if (piece->get_side() != manager_.current_side()) {
                return true;
            }
            first_position_ = board_index;
            selected_piece_ = piece;
            first_ = false;
            this->update();
        } else {
            second_position_ = board_index;
            this->send_move();
            selected_piece_ = nullptr;
            first_ = true;
        }
        return true;
    }

    sig::Slot<void(Position, Position)> make_move;
    sig::Slot<void()> toggle_show_moves;
    sig::Slot<void(cppurses::Push_button*)> reset_game;
    sig::Slot<void(Side, std::string)> change_ai;
    bool reset_first_click_{true};

   private:
    Manager manager_;
    Position first_position_;
    Position second_position_;
    bool first_{true};
    bool show_moves_{false};
    Piece* selected_piece_{nullptr};
    sig::Slot<void()> next_player_move_;
    std::unique_ptr<Player> player_white_{
        std::make_unique<Player_human>(&manager_)};
    std::unique_ptr<Player> player_black_{
        std::make_unique<Player_human>(&manager_)};

    std::unique_ptr<Player>& get_current_player() {
        auto current_side = manager_.current_side();
        if (current_side == Side::White) {
            return player_white_;
        }
        return player_black_;
    }

    cppurses::Color get_background(Position p) {
        if (show_moves_ && selected_piece_ != nullptr) {
            auto moves = manager_.get_moves(selected_piece_->get_position());
            auto iter = std::find(std::begin(moves), std::end(moves), p);
            if (iter != std::end(moves)) {
                return cppurses::Color::Light_green;
            }
        }
        if (p.row % 2 == 0) {
            if (p.column % 2 == 0) {
                return cppurses::Color::Brown;
            }
            return cppurses::Color::Light_gray;
        }
        if (p.row % 2 != 0) {
            if (p.column % 2 == 0) {
                return cppurses::Color::Light_gray;
            }
            return cppurses::Color::Brown;
        }
        return cppurses::Color::Orange;
    }

    Position screen_to_board_index(Position i) {
        // 8 - screen.y
        int row = 8 - i.column;

        // floor(screen.x / 3) + 1
        int column = (i.row / 3) + 1;
        return Position{row, column};
    }
    //   board                       screen
    // 8  oxooxooxooxooxooxooxooxo   0 oxooxooxooxooxooxooxooxo
    // 7  oxooxooxooxooxooxooxooxo   1 oxooxooxooxooxooxooxooxo
    // 6  oxooxooxooxooxooxooxooxo   2 oxooxooxooxooxooxooxooxo
    // 5  oxooxooxooxooxooxooxooxo   3 oxooxooxooxooxooxooxooxo
    // 4  oxooxooxooxooxooxooxooxo   4 oxooxooxooxooxooxooxooxo
    // 3  oxooxooxooxooxooxooxooxo   5 oxooxooxooxooxooxooxooxo
    // 2  oxooxooxooxooxooxooxooxo   6 oxooxooxooxooxooxooxooxo
    // 1  oxooxooxooxooxooxooxooxo   7 oxooxooxooxooxooxooxooxo
    //     1  2  3  4  5  6  7  8       1  4  7  10 13 16 19 22

    // column = x, row = y
    Position board_to_screen_index(Position i) {
        // 8 - board.y
        int y = 8 - i.row;

        // 1 + (board.x - 1) * 3
        int x = 1 + (i.column - 1) * 3;
        return Position{x, y};
    }

    void send_move() {
        if (!manager_.make_move(first_position_, second_position_)) {
            return;
        }
        this->update();
    }

   public:
    sig::Signal<void(std::string)>& move_message{manager_.move_message_sig};
    sig::Signal<void(std::string)>& status_message{manager_.status_message_sig};
};

#endif  // CHESS_WIDGET_HPP
