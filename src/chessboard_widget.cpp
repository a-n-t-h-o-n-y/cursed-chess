#include "chessboard_widget.hpp"

using namespace cppurses;

namespace {

Glyph piece_to_glyph(Piece piece) {
    Glyph symbol;
    // Character
    if (piece.figure == Figure::Bishop) {
        symbol = "♝";
    } else if (piece.figure == Figure::King) {
        symbol = "♚";
    } else if (piece.figure == Figure::Knight) {
        symbol = "♞";
    } else if (piece.figure == Figure::Pawn) {
        symbol = "♟";
    } else if (piece.figure == Figure::Queen) {
        symbol = "♛";
    } else if (piece.figure == Figure::Rook) {
        symbol = "♜";
    }
    // Side
    if (piece.side == Side::Black) {
        symbol.brush().set_foreground(cppurses::Color::Black);
    } else if (piece.side == Side::White) {
        symbol.brush().set_foreground(cppurses::Color::White);
    }
    return symbol;
}

Position board_to_screen_position(Position board_position) {
    int y = 8 - board_position.row;
    int x = 1 + (board_position.column - 1) * 3;
    return Position{x, y};
}

Position screen_to_board_position(Position screen_position) {
    int row = 8 - screen_position.column;
    int column = (screen_position.row / 3) + 1;
    return Position{row, column};
}

}  // namespace

Chessboard_widget::Chessboard_widget() {
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(8);
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(24);

    engine_.move_made.connect(::slot::trigger_next_move(*this));
}

void Chessboard_widget::toggle_show_moves() {
    show_moves_ = !show_moves_;
    this->update();
}

void Chessboard_widget::reset_game() {
    engine_.reset();
    this->update();
}

void Chessboard_widget::trigger_next_move() {
    Move next_move{Position{-1, -1}, Position{-1, -1}};
    if (engine_.current_side() == Side::Black) {
        next_move = player_black_->get_move();
    } else if (engine_.current_side() == Side::White) {
        next_move = player_white_->get_move();
    }
    if (next_move.from.row == -1) {
        return;
    }
    if (!engine_.make_move(next_move)) {
        this->trigger_next_move();
    }
}

namespace slot {

sig::Slot<void(const Move&)> trigger_next_move(Chessboard_widget& cbw) {
    sig::Slot<void(const Move&)> slot{
        [&cbw](const Move& throw_away) { cbw.trigger_next_move(); }};
    slot.track(cbw.destroyed);
    return slot;
}

sig::Slot<void()> toggle_show_moves(Chessboard_widget& cbw) {
    sig::Slot<void()> slot{[&cbw] { cbw.toggle_show_moves(); }};
    slot.track(cbw.destroyed);
    return slot;
}

sig::Slot<void()> reset_game(Chessboard_widget& cbw) {
    sig::Slot<void()> slot{[&cbw] { cbw.reset_game(); }};
    slot.track(cbw.destroyed);
    return slot;
}

}  // namespace slot

bool Chessboard_widget::paint_event() {
    Painter p{this};
    // Light Gray Tiles
    Glyph_string cell1{"   ", background(cppurses::Color::Light_gray)};
    // Brown Tiles
    Glyph_string cell2{"   ", background(cppurses::Color::Brown)};

    // Checkerboard
    for (int i{0}; i < 4; ++i) {
        p.put(cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2, 0,
              i * 2);
        p.put(cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1, 0,
              i * 2 + 1);
    }

    // Valid Moves
    if (show_moves_ && selected_position_ != opt::none &&
        engine_.at(*selected_position_).side == engine_.current_side()) {
        auto valid_moves = engine_.get_valid_positions(*selected_position_);
        Glyph_string highlight{"   ", background(cppurses::Color::Light_green)};
        for (Position possible_position : valid_moves) {
            Position where = board_to_screen_position(possible_position);
            p.put(highlight, where.row - 1, where.column);
        }
    }

    // Pieces
    auto piece_positions =
        engine_.find_positions(Piece{Figure::None, Side::None});
    for (Position piece_position : piece_positions) {
        Glyph piece_visual{piece_to_glyph(engine_.at(piece_position))};
        piece_visual.brush().set_background(get_tile_color(piece_position));
        Position where = board_to_screen_position(piece_position);
        p.put(piece_visual, where.row, where.column);
    }
    return Widget::paint_event();
}

bool Chessboard_widget::mouse_press_event(Mouse_button button,
                                          std::size_t global_x,
                                          std::size_t global_y,
                                          std::size_t local_x,
                                          std::size_t local_y,
                                          std::uint8_t device_id) {
    int loc_x = static_cast<int>(local_x);
    int loc_y = static_cast<int>(local_y);
    Position clicked_pos = screen_to_board_position(Position{loc_x, loc_y});
    Piece piece_clicked{engine_.at(clicked_pos)};
    selected_position_ = clicked_pos;

    if (piece_clicked.side == engine_.current_side()) {
        first_position_ = clicked_pos;
    } else if (first_position_ != opt::none) {
        engine_.make_move(Move{*first_position_, clicked_pos});
        first_position_ = opt::none;
        selected_position_ = opt::none;
    }
    this->update();
    return Widget::mouse_press_event(button, global_x, global_y, local_x,
                                     local_y, device_id);
}

cppurses::Color Chessboard_widget::get_tile_color(Position p) {
    if (show_moves_ && selected_position_ != opt::none &&
        engine_.at(*selected_position_).side == engine_.current_side()) {
        auto valid_moves = engine_.get_valid_positions(*selected_position_);
        auto at = std::find(std::begin(valid_moves), std::end(valid_moves), p);
        if (at != std::end(valid_moves)) {
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

// void Chessboard_widget::next_player_move() {
//     System::send_event(Paint_event(this));
//     System::paint_buffer()->flush(true);
//     if (engine_.game_over()) {
//         return;
//     }
//     auto& next_player = this->get_current_player();
//     next_player->make_move();
// };

// std::unique_ptr<Player>& Chessboard_widget::get_current_player() {
//     auto current_side = engine_.current_side();
//     if (current_side == Side::White) {
//         return player_white_;
//     }
//     return player_black_;
// }

// Color Chessboard_widget::get_background(Position p) {
//     if (show_moves_ && selected_piece_ != opt::none) {
//         auto moves = engine_.get_moves(selected_piece_->get_position());
//         auto iter = std::find(std::begin(moves), std::end(moves), p);
//         if (iter != std::end(moves)) {
//             return Color::Light_green;
//         }
//     }
//     if (p.row % 2 == 0) {
//         if (p.column % 2 == 0) {
//             return Color::Brown;
//         }
//         return Color::Light_gray;
//     }
//     if (p.row % 2 != 0) {
//         if (p.column % 2 == 0) {
//             return Color::Light_gray;
//         }
//         return Color::Brown;
//     }
//     return Color::Orange;
// }

// Position Chessboard_widget::screen_to_board_index(Position i) {
//     int row = 8 - i.column;
//     int column = (i.row / 3) + 1;
//     return Position{row, column};
// }

// Position Chessboard_widget::board_to_screen_index(Position i) {
//     int y = 8 - i.row;
//     int x = 1 + (i.column - 1) * 3;
//     return Position{x, y};
// }

// void Chessboard_widget::send_move() {
//     if (!engine_.make_move(first_position_, second_position_)) {
//         return;
//     }
//     this->update();
// }

// namespace slot {

// sig::Slot<void(Position, Position)> make_move(Chessboard_widget& cw) {
//     sig::Slot<void(Position, Position)> slot{
//         [&cw](auto p1, auto p2) { cw.make_move(p1, p2); }};
//     slot.track(cw.destroyed);
//     return slot;
// }

// sig::Slot<void()> toggle_show_moves(Chessboard_widget& cw) {
//     sig::Slot<void()> slot{[&cw] { cw.toggle_show_moves(); }};
//     slot.track(cw.destroyed);
//     return slot;
// }

// sig::Slot<void(Push_button*)> reset_game(Chessboard_widget& cw) {
//     sig::Slot<void(Push_button*)> slot{[&cw](auto* pb) { cw.reset_game(*pb);
//     }};
//     slot.track(cw.destroyed);
//     return slot;
// }

// sig::Slot<void(Side, std::string)> change_ai(Chessboard_widget& cw) {
//     sig::Slot<void(Side, std::string)> slot{
//         [&cw](auto s, auto ai) { cw.change_ai(s, ai); }};
//     slot.track(cw.destroyed);
//     return slot;
// }

// sig::Slot<void()> next_player_move(Chessboard_widget& cw) {
//     sig::Slot<void()> slot{[&cw] { cw.next_player_move(); }};
//     slot.track(cw.destroyed);
//     return slot;
// }

// }  // namespace slot
