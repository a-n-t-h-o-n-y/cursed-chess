#include "side_pane.hpp"

#include <sstream>

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/widget/border.hpp>
#include <cppurses/widget/focus_policy.hpp>
#include <cppurses/widget/size_policy.hpp>
#include <cppurses/widget/widget_free_functions.hpp>

#include "chessboard_widget.hpp"
#include "figure.hpp"

using namespace chess;
using namespace cppurses;

Side_pane::Side_pane() {
    this->set_name("Chess - Side_pane");
    this->width_policy.type(Size_policy::Maximum);
    this->width_policy.max(26);
    this->width_policy.hint(26);

    // hl_status
    hl_status.set_name("hl_status in Chess - Side_pane");
    hl_status.height_policy.type(Size_policy::Fixed);
    hl_status.height_policy.hint(2);
    set_background_recursive(hl_status, Color::Dark_blue);
    set_foreground_recursive(hl_status, Color::Light_gray);

    // status bar
    status.set_name("status in Chess - Side_pane");
    status.width_policy.type(Size_policy::Fixed);
    status.width_policy.hint(4);
    status.height_policy.type(Size_policy::Fixed);
    status.height_policy.hint(2);

    status.border.enable();
    status.border.segments.disable_all();
    status.border.segments.west.enable();
    status.border.segments.west = Glyph{L'╮', foreground(Color::Blue)};
    status.border.segments.south.enable();
    status.border.segments.south.brush.set_foreground(Color::Blue);
    status.border.segments.south_west.enable();
    status.border.segments.south_west = Glyph{L'├', foreground(Color::Blue)};

    // settings_btn
    settings_btn.set_name("settings_btn in Chess - Side_pane");
    settings_btn.width_policy.type(Size_policy::Preferred);
    settings_btn.width_policy.hint(12);

    settings_btn.border.enable();
    settings_btn.border.segments.disable_all();

    settings_btn.border.segments.south.enable();
    settings_btn.border.segments.south.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.west.enable();
    settings_btn.border.segments.west.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.east.enable();
    settings_btn.border.segments.east.brush.set_foreground(Color::Blue);
    settings_btn.border.segments.south_west.enable();
    settings_btn.border.segments.south_west =
        Glyph{L'┴', foreground(Color::Blue)};
    settings_btn.border.segments.south_east.enable();
    settings_btn.border.segments.south_east =
        Glyph{L'┴', foreground(Color::Blue)};

    // Blank Space
    blank_space.set_name("blank_space in Chess - Side_pane");
    blank_space.border.enable();
    blank_space.border.segments.disable_all();
    blank_space.border.segments.south.enable();
    blank_space.border.segments.south.brush.set_foreground(Color::Blue);

    // log
    chess_log.set_name("chess_log in Chess - Side_pane");
    chess_log.cursor.disable();
    chess_log.focus_policy = Focus_policy::None;
    chess_log.height_policy.type(Size_policy::Fixed);
    chess_log.height_policy.hint(6);
    set_background(chess_log, Color::Dark_blue);
    set_foreground(chess_log, Color::Light_gray);

    chess_log.border.enable();
    chess_log.border.segments.disable_all();
    chess_log.border.segments.west.enable();
    chess_log.border.segments.west.brush.set_foreground(Color::Blue);

    // hl - bottom
    hl.set_name("hl in Chess - Side_pane");
    set_background_recursive(hl, Color::Dark_blue);
    hl.height_policy.type(Size_policy::Fixed);
    hl.height_policy.hint(2);

    // move_input
    move_input.set_name("move_input in Chess - Side_pane");

    move_input.border.enable();
    move_input.border.segments.disable_all();
    move_input.border.segments.west.enable();
    move_input.border.segments.west.brush.set_foreground(Color::Blue);
    move_input.border.segments.north.enable();
    move_input.border.segments.north.brush.set_foreground(Color::Blue);
    move_input.border.segments.north_west.enable();
    move_input.border.segments.north_west =
        Glyph{L'├', foreground(Color::Blue)};

    set_foreground(move_input, Color::Light_gray);
    move_input.height_policy.hint(2);
}

void Side_pane::toggle_status(const Chessboard_widget& board) {
    if (board.current_side() == Side::Black) {
        status.set_contents(
            Glyph_string{" B", Attribute::Bold, foreground(Color::Black)});
    } else {
        status.set_contents(
            Glyph_string{" W", Attribute::Bold, foreground(Color::White)});
    }
}

void Side_pane::post_move_message(const Move& m) {
    chess_log.post_message(move_to_text(m));
}

void Side_pane::post_capture_message(Piece p) {
    // temporary until checkmate counts moving king into check as checkmate
    if (p.figure == Figure::King) {
        this->post_checkmate_message(p.side);
        return;
    }
    Glyph_string side_str;
    if (p.side == Side::Black) {
        side_str = Glyph_string{"Black", foreground(Color::Black)};
    } else {
        side_str = Glyph_string{"White", foreground(Color::White)};
    }
    Glyph_string figure_str{figure_to_text(p.figure)};
    chess_log.post_message(side_str + " " + figure_str + " " + "Captured");
}

void Side_pane::post_invalid_move_message(const Move& m) {
    chess_log.post_message("Invalid Move " + move_to_text(m));
}

void Side_pane::post_checkmate_message(Side s) {
    Glyph_string side;
    if (s == Side::Black) {
        side = Glyph_string{"Black", foreground(Color::Black)};
    } else {
        side = Glyph_string{"White", foreground(Color::White)};
    }
    chess_log.post_message("Checkmate");
    chess_log.post_message(side + " Looses");
}

void Side_pane::post_check_message(Side s) {
    Glyph_string side;
    if (s == Side::Black) {
        side = Glyph_string{"Black", foreground(Color::Black)};
    } else {
        side = Glyph_string{"White", foreground(Color::White)};
    }
    chess_log.post_message(side + " in Check");
}

void Side_pane::post_stalemate_message() {
    chess_log.post_message("Stalemate");
}

namespace slot {

sig::Slot<void(Move)> post_move_message(Side_pane& sp) {
    sig::Slot<void(Move)> slot{[&sp](Move m) { sp.post_move_message(m); }};
    slot.track(sp.destroyed);
    return slot;
}

sig::Slot<void(Move)> toggle_status(Side_pane& sp,
                                    const Chessboard_widget& board) {
    sig::Slot<void(Move)> slot{
        [&sp, &board](Move) { sp.toggle_status(board); }};
    slot.track(sp.destroyed);
    slot.track(board.destroyed);
    return slot;
}

sig::Slot<void(Piece)> post_capture_message(Side_pane& sp) {
    sig::Slot<void(Piece)> slot{[&sp](Piece p) { sp.post_capture_message(p); }};
    slot.track(sp.destroyed);
    return slot;
}

sig::Slot<void(const Move&)> post_invalid_move_message(Side_pane& sp) {
    sig::Slot<void(const Move&)> slot{
        [&sp](const Move& m) { sp.post_invalid_move_message(m); }};
    slot.track(sp.destroyed);
    return slot;
}

sig::Slot<void(Side)> post_checkmate_message(Side_pane& sp) {
    sig::Slot<void(Side)> slot{[&sp](Side s) { sp.post_checkmate_message(s); }};
    slot.track(sp.destroyed);
    return slot;
}

sig::Slot<void(Side)> post_check_message(Side_pane& sp) {
    sig::Slot<void(Side)> slot{[&sp](Side s) { sp.post_check_message(s); }};
    slot.track(sp.destroyed);
    return slot;
}
}  // namespace slot
