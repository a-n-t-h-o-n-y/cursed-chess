#include "chessboard_widget.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>

#include <cppurses/system/mouse_data.hpp>
#include <cppurses/system/system.hpp>

#include "chess_move_request_event.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "shared_user_input.hpp"
#include "side.hpp"

using namespace cppurses;
using namespace chess;

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
    this->set_name("Chessboard_widget");

    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(8);
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(24);

    // engine_.move_made.connect(::slot::trigger_next_move(*this));
    engine_.move_made.connect([this](Move m) { this->move_made(m); });
    engine_.move_made.connect([this](Move m) { this->update(); });
    engine_.capture.connect([this](Piece p) { this->capture(p); });
    engine_.invalid_move.connect(
        [this](const Move& m) { this->invalid_move(m); });
    engine_.checkmate.connect([this](Side s) { this->checkmate(s); });
    engine_.check.connect([this](Side s) { this->check(s); });
    engine_.state().board_reset.connect([this] { this->board_reset(); });

    // launch game loop
    this->start();
}

void Chessboard_widget::toggle_show_moves() {
    show_moves_ = !show_moves_;
    this->update();
}

void Chessboard_widget::reset_game() {
    engine_.state().reset();
    this->update();
}

void Chessboard_widget::make_move(const Move& move) {
    engine_.make_move(move);
}

Side Chessboard_widget::current_side() const {
    return engine_.state().current_side;
}

void Chessboard_widget::pause() {
    game_loop_.exit(0);
}

void Chessboard_widget::start() {
    game_loop_.wait();
    game_loop_.run_async();
}

void Chessboard_widget::take_turn() {
    Move m;
    try {
        if (engine_.state().current_side == Side::Black) {
            m = engine_.player_black()->get_move();
        } else {
            m = engine_.player_white()->get_move();
        }
    } catch (System_exit_request e) {
        game_loop_.exit(0);
        return;
    }
    System::post_event<Chess_move_request_event>(*this, m);
}

void Chessboard_widget::move_request_event(Move m) {
    engine_.make_move(m);
}

Chess_engine& Chessboard_widget::engine() {
    return engine_;
}

const Chess_engine& Chessboard_widget::engine() const {
    return engine_;
}

namespace slot {

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
    Painter p{*this};
    for (int i{0}; i < 4; ++i) {
        p.put(cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2, 0,
              i * 2);
        p.put(cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1, 0,
              i * 2 + 1);
    }

    // Valid Moves
    const chess::State& state{engine_.state()};
    if (show_moves_ && selected_position_ != opt::none &&
        state.board.has_piece_at(*selected_position_) &&
        state.board.at(*selected_position_).side == state.current_side) {
        auto valid_moves = engine_.get_valid_positions(*selected_position_);
        Glyph_string highlight{"   ", background(cppurses::Color::Light_green)};
        for (Position possible_position : valid_moves) {
            Position where = board_to_screen_position(possible_position);
            p.put(highlight, where.row - 1, where.column);
        }
    }

    {
        std::lock_guard<std::recursive_mutex> lock{state.board.mtx};
        for (auto& pos_piece : state.board.pieces) {
            Position piece_position{pos_piece.first};
            Glyph piece_visual{piece_to_glyph(pos_piece.second)};
            piece_visual.brush.set_background(get_tile_color(piece_position));
            // TODO change to be Point.
            Position where = board_to_screen_position(piece_position);
            p.put(piece_visual, where.row, where.column);
        }
    }
    return Widget::paint_event();
}

bool Chessboard_widget::mouse_press_event(const Mouse_data& mouse) {
    int loc_x = static_cast<int>(mouse.local.x);
    int loc_y = static_cast<int>(mouse.local.y);
    Position clicked_pos{screen_to_board_position(Position{loc_x, loc_y})};
    selected_position_ = clicked_pos;

    const chess::State& state{engine_.state()};
    if (state.board.has_piece_at(clicked_pos) &&
        state.board.at(clicked_pos).side == state.current_side) {
        first_position_ = clicked_pos;
    } else if (first_position_ != opt::none) {
        // Notifies the game_loop_ thread in engine_.
        Shared_user_input::move.set(Move{*first_position_, clicked_pos});
        first_position_ = opt::none;
        selected_position_ = opt::none;
    }
    this->update();
    return Widget::mouse_press_event(mouse);
}

cppurses::Color Chessboard_widget::get_tile_color(Position p) {
    const State& state{engine_.state()};
    if (show_moves_ && selected_position_ != opt::none &&
        state.board.has_piece_at(*selected_position_) &&
        state.board.at(*selected_position_).side == state.current_side) {
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
