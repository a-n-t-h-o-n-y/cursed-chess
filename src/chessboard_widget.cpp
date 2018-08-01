#include "chessboard_widget.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>

// #include <cppurses/painter/detail/flush.hpp>

using namespace cppurses;

namespace {

Glyph piece_to_glyph(Piece piece) {
    Glyph symbol;
    // Character
    if (piece.figure == Figure::Bishop) {
        symbol = L'♝';
    } else if (piece.figure == Figure::King) {
        symbol = L'♚';
    } else if (piece.figure == Figure::Knight) {
        symbol = L'♞';
    } else if (piece.figure == Figure::Pawn) {
        symbol = L'♟';
    } else if (piece.figure == Figure::Queen) {
        symbol = L'♛';
    } else if (piece.figure == Figure::Rook) {
        symbol = L'♜';
    }
    // Side
    if (piece.side == Side::Black) {
        symbol.brush.set_foreground(cppurses::Color::Black);
    } else if (piece.side == Side::White) {
        symbol.brush.set_foreground(cppurses::Color::White);
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
    this->trigger_next_move();

    engine_.move_made.connect(::slot::trigger_next_move(*this));
    engine_.move_made.connect([this](Move m) { this->move_made(m); });
    engine_.capture.connect([this](Piece p) { this->capture(p); });
    engine_.invalid_move.connect(
        [this](const Move& m) { this->invalid_move(m); });
    engine_.checkmate.connect([this](Side s) { this->checkmate(s); });
    engine_.check.connect([this](Side s) { this->check(s); });
    engine_.board_reset.connect([this] { this->board_reset(); });
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
    // TODO what is going on with the send and flush?
    System::send_event(Paint_event(this));
    System::paint_buffer().flush(System::find_event_loop().staged_changes());
    Move next_move{Position{-1, -1}, Position{-1, -1}};
    if (engine_.current_side() == Side::Black) {
        next_move = player_black_->get_move();
    } else if (engine_.current_side() == Side::White) {
        next_move = player_white_->get_move();
    }
    if (next_move.from.row == -1 || next_move.from.row == 0) {
        return;
    }
    if (!engine_.make_move(next_move)) {
        this->trigger_next_move();
    }
}

void Chessboard_widget::make_move(const Move& move) {
    engine_.make_move(move);
}

Side Chessboard_widget::current_side() const {
    return engine_.current_side();
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

sig::Slot<void(Move)> make_move(Chessboard_widget& cbw) {
    sig::Slot<void(Move)> slot{[&cbw](Move m) { cbw.make_move(m); }};
    slot.track(cbw.destroyed);
    return slot;
}

}  // namespace slot

bool Chessboard_widget::paint_event() {
    // Light Gray Tiles
    Glyph_string cell1{"   ", background(cppurses::Color::Light_gray)};
    // Gray Tiles
    Glyph_string cell2{"   ", background(cppurses::Color::Dark_blue)};

    // Checkerboard
    Painter p{this};
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
        piece_visual.brush.set_background(get_tile_color(piece_position));
        Position where = board_to_screen_position(piece_position);
        p.put(piece_visual, where.row, where.column);
    }
    return Widget::paint_event();
}

bool Chessboard_widget::mouse_press_event(Mouse_button button,
                                          Point global,
                                          Point local,
                                          std::uint8_t device_id) {
    int loc_x = static_cast<int>(local.x);
    int loc_y = static_cast<int>(local.y);
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
    return Widget::mouse_press_event(button, global, local, device_id);
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
            return cppurses::Color::Dark_blue;
        }
        return cppurses::Color::Light_gray;
    }
    if (p.row % 2 != 0) {
        if (p.column % 2 == 0) {
            return cppurses::Color::Light_gray;
        }
        return cppurses::Color::Dark_blue;
    }
    return cppurses::Color::Orange;
}
