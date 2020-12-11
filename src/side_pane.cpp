#include "side_pane.hpp"

#include <sstream>

#include <signals_light/signal.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/trait.hpp>
#include <cppurses/widget/border.hpp>
#include <cppurses/widget/detail/link_lifetimes.hpp>
#include <cppurses/widget/focus_policy.hpp>

#include "chessboard_widget.hpp"
#include "figure.hpp"

using namespace chess;
using namespace cppurses;

Side_pane::Status::Status()
{
    this->height_policy.fixed(2);
    for (auto& child : this->get_children()) {
        child.brush.set_background(Color::Dark_blue);
        child.brush.set_foreground(Color::Light_gray);
    }

    status.width_policy.fixed(4);
    status.height_policy.fixed(2);
    status.border.enable();
    status.border.segments.disable_all();
    status.border.segments.west.enable();
    status.border.segments.west = L'╮'_g | fg(Color::Blue);
    status.border.segments.south.enable();
    status.border.segments.south.brush.set_foreground(Color::Blue);
    status.border.segments.south_west.enable();
    status.border.segments.south_west = L'├'_g | fg(Color::Blue);

    settings_btn.width_policy.preferred(12);
    settings_btn.border.enable();
    settings_btn.border.segments.disable_all();
    settings_btn.border.segments.south.enable();
    settings_btn.border.segments.south.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.west.enable();
    settings_btn.border.segments.west.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.east.enable();
    settings_btn.border.segments.east.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.south_west.enable();
    settings_btn.border.segments.south_west = L'┴'_g | fg(Color::Blue);
    settings_btn.border.segments.south_east.enable();
    settings_btn.border.segments.south_east = L'┴'_g | fg(Color::Blue);

    blank_space.border.enable();
    blank_space.border.segments.disable_all();
    blank_space.border.segments.south.enable();
    blank_space.border.segments.south.brush.set_foreground(Color::Blue);
}

Side_pane::Side_pane()
{
    this->width_policy.maximum(26);
    this->width_policy.max(26);

    // log
    chess_log.cursor.disable();
    chess_log.focus_policy = Focus_policy::None;
    chess_log.height_policy.fixed(6);
    chess_log.brush.set_background(Color::Dark_blue);
    chess_log.brush.set_foreground(Color::Light_gray);

    chess_log.border.enable();
    chess_log.border.segments.disable_all();
    chess_log.border.segments.west.enable();
    chess_log.border.segments.west.brush.set_foreground(Color::Blue);

    move_input.brush.set_background(Color::Dark_blue);
    move_input.brush.set_foreground(Color::Light_gray);
    move_input.height_policy.fixed(2);
    move_input.border.enable();
    move_input.border.segments.disable_all();
    move_input.border.segments.west.enable();
    move_input.border.segments.west.brush.set_foreground(Color::Blue);
    move_input.border.segments.north.enable();
    move_input.border.segments.north.brush.set_foreground(Color::Blue);
    move_input.border.segments.north_west.enable();
    move_input.border.segments.north_west = L'├'_g | fg(Color::Blue);
}

void Side_pane::toggle_status(const Chessboard_widget& board)
{
    if (board.current_side() == Side::Black)
        status.status.set_text(L" B" | Trait::Bold | fg(Color::Black));
    else
        status.status.set_text(L" W" | Trait::Bold | fg(Color::White));
}

void Side_pane::post_move_message(const Move& m)
{
    chess_log.post_message(move_to_text(m));
}

void Side_pane::post_capture_message(Piece p)
{
    // temporary until checkmate counts moving king into check as checkmate
    if (p.figure == Figure::King) {
        this->post_checkmate_message(p.side);
        return;
    }
    Glyph_string side_str;
    if (p.side == Side::Black)
        side_str = Glyph_string{"Black", fg(Color::Black)};
    else
        side_str = Glyph_string{"White", fg(Color::White)};
    Glyph_string figure_str{figure_to_text(p.figure)};
    chess_log.post_message(side_str + " " + figure_str + " " + "Captured");
}

void Side_pane::post_invalid_move_message(const Move& m)
{
    chess_log.post_message("Invalid Move " + move_to_text(m));
}

void Side_pane::post_checkmate_message(Side s)
{
    Glyph_string side;
    if (s == Side::Black)
        side = Glyph_string{"Black", fg(Color::Black)};
    else
        side = Glyph_string{"White", fg(Color::White)};
    chess_log.post_message("Checkmate");
    chess_log.post_message(side + " Looses");
}

void Side_pane::post_check_message(Side s)
{
    Glyph_string side;
    if (s == Side::Black) {
        side = Glyph_string{"Black", fg(Color::Black)};
    }
    else {
        side = Glyph_string{"White", fg(Color::White)};
    }
    chess_log.post_message(side + " in Check");
}

void Side_pane::post_stalemate_message()
{
    chess_log.post_message("Stalemate");
}

namespace slot {

using cppurses::slot::link_lifetimes;

auto post_move_message(Side_pane& sp) -> sl::Slot<void(Move)>
{
    return link_lifetimes([&sp](Move m) { sp.post_move_message(m); }, sp);
}

auto toggle_status(Side_pane& sp, const Chessboard_widget& board)
    -> sl::Slot<void(Move)>
{
    return link_lifetimes([&sp, &board](Move) { sp.toggle_status(board); }, sp,
                          board);
}

auto post_capture_message(Side_pane& sp) -> sl::Slot<void(Piece)>
{
    return link_lifetimes([&sp](Piece p) { sp.post_capture_message(p); }, sp);
}

auto post_invalid_move_message(Side_pane& sp) -> sl::Slot<void(const Move&)>
{
    return link_lifetimes(
        [&sp](const Move& m) { sp.post_invalid_move_message(m); }, sp);
}

auto post_checkmate_message(Side_pane& sp) -> sl::Slot<void(Side)>
{
    return link_lifetimes([&sp](Side s) { sp.post_checkmate_message(s); }, sp);
}

auto post_check_message(Side_pane& sp) -> sl::Slot<void(Side)>
{
    return link_lifetimes([&sp](Side s) { sp.post_check_message(s); }, sp);
}

}  // namespace slot
