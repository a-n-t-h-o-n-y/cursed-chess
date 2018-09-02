#include "lower_pane.hpp"

#include <cppurses/cppurses.hpp>

#include "chessboard_widget.hpp"
#include "side.hpp"

using namespace cppurses;

Lower_pane::Lower_pane() {
    this->width_policy.type(Size_policy::Fixed);
    this->width_policy.hint(26);

    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);

    set_background_recursive(*this, Color::Dark_blue);
    set_foreground_recursive(*this, Color::Light_gray);

    // move_input
    enable_border(move_input);
    move_input.border.west = Glyph{L'│', foreground(Color::Blue)};
    move_input.border.east = Glyph{L'│', foreground(Color::Blue)};

    // status
    enable_border(status);
    disable_corners(status.border);
    status.border.east_enabled = false;
    status.border.west = Glyph{L'╰', foreground(Color::Blue)};
    status.width_policy.type(Size_policy::Fixed);
    status.width_policy.hint(4);
}

void Lower_pane::toggle_status(const Chessboard_widget& board) {
    if (board.current_side() == chess::Side::Black) {
        status.set_text(
            Glyph_string{" B", Attribute::Bold, foreground(Color::Black)});
    } else {
        status.set_text(
            Glyph_string{" W", Attribute::Bold, foreground(Color::White)});
    }
}

namespace slot {

sig::Slot<void(Move)> toggle_status(Lower_pane& lp,
                                    const Chessboard_widget& board) {
    sig::Slot<void(Move)> slot{
        [&lp, &board](Move) { lp.toggle_status(board); }};
    slot.track(lp.destroyed);
    slot.track(board.destroyed);
    return slot;
}

}  // namespace slot
