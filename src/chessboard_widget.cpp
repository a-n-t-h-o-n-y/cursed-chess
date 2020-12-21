#include "chessboard_widget.hpp"

#include <algorithm>
#include <iterator>
#include <mutex>

#include <signals_light/signal.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/painter/painter.hpp>
#include <cppurses/system/mouse.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/pipe.hpp>

#include "chess_move_request_event.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "shared_user_input.hpp"
#include "side.hpp"

using namespace cppurses;

namespace {
using namespace chess;

auto piece_to_glyph(Piece piece) -> Glyph
{
    auto symbol = Glyph{};
    switch (piece.figure) {
        case Figure::Bishop: symbol = L'♝'; break;
        case Figure::King: symbol = L'♚'; break;
        case Figure::Knight: symbol = L'♞'; break;
        case Figure::Pawn: symbol = L'♟'; break;
        case Figure::Queen: symbol = L'♛'; break;
        case Figure::Rook: symbol = L'♜'; break;
    }
    // Side
    if (piece.side == Side::Black)
        symbol | fg(Color::Black);
    else if (piece.side == Side::White)
        symbol | fg(Color::White);
    return symbol;
}

auto board_to_screen_position(Position board_position) -> Position
{
    auto const y = 8 - board_position.row;
    auto const x = 1 + (board_position.column - 1) * 3;
    return {x, y};
}

auto screen_to_board_position(Position screen_position) -> Position
{
    auto const row    = 8 - screen_position.column;
    auto const column = (screen_position.row / 3) + 1;
    return {row, column};
}

}  // namespace

namespace chess {

Chessboard_widget::Chessboard_widget()
{
    using namespace cppurses::pipe;
    *this | fixed_height(8);
    *this | fixed_width(24);

    engine_.move_made.connect([this](Move m) { this->move_made(m); });
    engine_.move_made.connect([this](Move) { this->update(); });
    engine_.capture.connect([this](Piece p) { this->capture(p); });
    engine_.invalid_move.connect(
        [this](const Move& m) { this->invalid_move(m); });
    engine_.checkmate.connect([this](Side s) { this->checkmate(s); });
    engine_.check.connect([this](Side s) { this->check(s); });
    engine_.state().board_reset.connect([this] { this->board_reset(); });
}

void Chessboard_widget::take_turn()
{
    auto m = Move{};
    try {
        if (engine_.state().current_side == Side::Black)
            m = engine_.player_black().get_move(engine_);
        else
            m = engine_.player_white().get_move(engine_);
    }
    catch (Chess_loop_exit_request e) {
        this->exit_game_loop();
        return;
    }
    System::post_event(chess_move_request_event(*this, m));
}

auto Chessboard_widget::paint_event() -> bool
{
    auto const cell1 = L"   " | bg(Color::Light_gray);
    auto const cell2 = L"   " | bg(Color::Dark_blue);

    // Checkerboard
    auto p = Painter{*this};
    for (auto i = 0; i < 4; ++i) {
        p.put(cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2, 0,
              i * 2);
        p.put(cell2 + cell1 + cell2 + cell1 + cell2 + cell1 + cell2 + cell1, 0,
              i * 2 + 1);
    }

    // Valid Moves
    auto const& state = engine_.state();
    if (show_moves_ && selected_position_.has_value() &&
        state.board.has_piece_at(*selected_position_) &&
        state.board.at(*selected_position_).side == state.current_side) {
        auto const valid_moves =
            engine_.get_valid_positions(*selected_position_);
        auto const highlight = L"   " | bg(Color::Light_green);
        for (Position possible_position : valid_moves) {
            auto const where = board_to_screen_position(possible_position);
            p.put(highlight, where.row - 1, where.column);
        }
    }

    {
        auto const lock = std::lock_guard{state.board.mtx};
        for (auto& pos_piece : state.board.pieces) {
            auto const piece_position = pos_piece.first;
            auto const piece_visual   = piece_to_glyph(pos_piece.second) |
                                      bg(get_tile_color(piece_position));
            // TODO change to be Point.
            auto const where = board_to_screen_position(piece_position);
            p.put(piece_visual, where.row, where.column);
        }
    }
    return Widget::paint_event();
}

auto Chessboard_widget::mouse_press_event(const Mouse& m) -> bool
{
    auto const loc_x       = static_cast<int>(m.local.x);
    auto const loc_y       = static_cast<int>(m.local.y);
    auto const clicked_pos = screen_to_board_position({loc_x, loc_y});
    selected_position_     = clicked_pos;

    auto const& state = engine_.state();
    if (state.board.has_piece_at(clicked_pos) &&
        state.board.at(clicked_pos).side == state.current_side) {
        first_position_ = clicked_pos;
    }
    else if (first_position_.has_value()) {
        Shared_user_input::move.set(Move{*first_position_, clicked_pos});
        first_position_    = std::nullopt;
        selected_position_ = std::nullopt;
    }
    this->update();
    return Widget::mouse_press_event(m);
}

auto Chessboard_widget::enable_event() -> bool
{
    this->start();
    return Widget::enable_event();
}

auto Chessboard_widget::disable_event() -> bool
{
    this->pause();
    return Widget::disable_event();
}

auto Chessboard_widget::get_tile_color(Position p) -> Color
{
    auto const& state = engine_.state();
    if (show_moves_ && selected_position_.has_value() &&
        state.board.has_piece_at(*selected_position_) &&
        state.board.at(*selected_position_).side == state.current_side) {
        auto valid_moves = engine_.get_valid_positions(*selected_position_);
        auto at = std::find(std::begin(valid_moves), std::end(valid_moves), p);
        if (at != std::end(valid_moves))
            return Color::Light_green;
    }
    if (p.row % 2 == 0) {
        if (p.column % 2 == 0)
            return Color::Dark_blue;
        return Color::Light_gray;
    }
    if (p.row % 2 != 0) {
        if (p.column % 2 == 0)
            return Color::Light_gray;
        return Color::Dark_blue;
    }
    return Color::Orange;
}

}  // namespace chess
