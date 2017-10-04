#include "chess_widget.hpp"

using namespace cppurses;

Chess_widget::Chess_widget() {
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(8);
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(24);

    manager_.move_made_sig.connect(slot::next_player_move(*this));
}

void Chess_widget::make_move(Position one, Position two) {
    this->manager_.make_move(p1, p2);
    this->update();
}

void Chess_widget::toggle_show_moves() {
    show_moves_ = !show_moves_;
    this->update();
}

void Chess_widget::reset_game(Push_button& button) {
    if (reset_first_click_) {
        button.set_text("ᴿ ᵘ ˢᵘʳᵉ?");
        reset_first_click_ = false;
    } else {
        button.set_text("ᴿᵉˢᵉᵗ");
        reset_first_click_ = true;
        manager_.reset_game();
        this->update();
    }
}

void Chess_widget::change_ai(Side player, std::string ai) {
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
}

void Chess_widget::next_player_move() {
    System::send_event(this, Paint_event());
    System::paint_engine()->flush(true);
    if (manager_.game_over()) {
        return;
    }
    auto& next_player = this->get_current_player();
    next_player->make_move();
};

bool Chess_widget::paint_event() {
    Painter p{this};
    // Cell type 1
    Glyph_string cell1{"   ", background(Color::Light_gray)};

    // Cell type 2
    Glyph_string cell2{"   ", background(Color::Brown)};

    // Paint
    for (int i{0}; i < 4; ++i) {
        p.put(cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2, 0,
              i * 2);
        p.put(cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1, 0,
              i * 2 + 1);
    }

    // Possible Moves
    if (show_moves_ && selected_piece_ != nullptr) {
        auto moves = manager_.get_moves(selected_piece_->get_position());
        Glyph_string highlight{"   ", background(Color::Light_green)};
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
        Glyph visual{piece->get_display()};
        if (piece->get_side() == Side::Black) {
            visual.brush().set_foreground(Color::Black);
        } else {
            visual.brush().set_foreground(Color::White);
        }
        // background color
        visual.brush().set_background(
            this->get_background(piece->get_position()));
        // position
        auto board_pos = board_to_screen_index(piece->get_position());
        p.put(visual, board_pos.row, board_pos.column);
    }
    return Widget::paint_event();
}

bool Chess_widget::mouse_press_event(Mouse_button button,
                                     std::size_t global_x,
                                     std::size_t global_y,
                                     std::size_t local_x,
                                     std::size_t local_y,
                                     std::uint8_t device_id) {
    auto loc_x = static_cast<int>(local_x);
    auto loc_y = static_cast<int>(local_y);
    auto board_index = screen_to_board_index(Position{loc_x, loc_y});
    auto* piece = manager_.find_piece(board_index);
    if (first_ ||
        (piece != nullptr && piece->get_side() == manager_.current_side())) {
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

std::unique_ptr<Player>& Chess_widget::get_current_player() {
    auto current_side = manager_.current_side();
    if (current_side == Side::White) {
        return player_white_;
    }
    return player_black_;
}

Color Chess_widget::get_background(Position p) {
    if (show_moves_ && selected_piece_ != nullptr) {
        auto moves = manager_.get_moves(selected_piece_->get_position());
        auto iter = std::find(std::begin(moves), std::end(moves), p);
        if (iter != std::end(moves)) {
            return Color::Light_green;
        }
    }
    if (p.row % 2 == 0) {
        if (p.column % 2 == 0) {
            return Color::Brown;
        }
        return Color::Light_gray;
    }
    if (p.row % 2 != 0) {
        if (p.column % 2 == 0) {
            return Color::Light_gray;
        }
        return Color::Brown;
    }
    return Color::Orange;
}

Position Chess_widget::screen_to_board_index(Position i) {
    int row = 8 - i.column;
    int column = (i.row / 3) + 1;
    return Position{row, column};
}

Position Chess_widget::board_to_screen_index(Position i) {
    int y = 8 - i.row;
    int x = 1 + (i.column - 1) * 3;
    return Position{x, y};
}

void Chess_widget::send_move() {
    if (!manager_.make_move(first_position_, second_position_)) {
        return;
    }
    this->update();
}

namespace slot {

sig::Slot<void(Position, Position)> make_move(Chess_widget& cw) {
    sig::Slot<void(Position, Position)> slot{
        [&cw](auto p1, auto p2) { cw.make_move(p1, p2); }};
    slot.track(cw.destroyed);
    return slot;
}

sig::Slot<void()> toggle_show_moves(Chess_widget& cw) {
    sig::Slot<void()> slot{[&cw] { cw.toggle_show_moves(); }};
    slot.track(cw.destroyed);
    return slot;
}

sig::Slot<void(Push_button&)> reset_game(Chess_widget& cw) {
    sig::Slot<void(Push_button&)> slot{
        [&cw](auto& pb) { cw.toggle_show_moves(pb); }};
    slot.track(cw.destroyed);
    return slot;
}

sig::Slot<void(Side, std::string)> change_ai(Chess_widget& cw) {
    sig::Slot<void(Side, std::string)> slot{
        [&cw](auto s, auto ai) { cw.change_ai(s, ai); }};
    slot.track(cw.destroyed);
    return slot;
}

sig::Slot<void()> next_player_move(Chess_widget& cw) {
    sig::Slot<void()> slot{[&cw] { cw.next_player_move(); }};
    slot.track(cw.destroyed);
    return slot;
}

}  // namespace slot
